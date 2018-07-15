<?php
/**
 * @file
 *   Form generation functions for the Commerce bulk product creation module
 */

/**
 * Form constructor for the bulk creation form.
 *
 * Paths: admin/commerce/products/add-bulk/PRODUCT_TYPE
 *
 * @see commerce_bpc_menu()
 * @see commerce_bpc_create_bulk_form_validate()
 * @see commerce_bpc_create_bulk_form_submit()
 * @ingroup forms
 */
function commerce_bpc_create_bulk_form($form, &$form_state, $product_type) {

  $form['#parents'] = array();

  // Create a temporary product object attach fields.
  $new_product = commerce_product_new($product_type);
  $language = !empty($new_product->language) ? $new_product->language : LANGUAGE_NONE;

  // Store the product type for later use.
  $form['product_type'] = array(
    '#type' => 'value',
    '#value' => $product_type,
  );

  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Bulk Product Creation Form'),
  );

  $form['product'] = array(
    '#type' => 'fieldset',
    '#title' => t('Product Info'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,

  );
  $form['combinations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Combinations'),
    '#description' => t('A product will be created for every possible combination of the values you select here.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    );

  $form['static_values'] = array(
    '#type' => 'fieldset',
    '#title' => t('Static values'),
    '#description' => t('The values of these fields will be shared by all generated products.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
    '#parents' => array('static_values'),
  );

  $form['product']['sku_fragment'] = array(
    '#type' => 'textfield',
    '#title' => t('SKU'),
    '#required' => TRUE,
    '#description' => t("The part of the SKU that is common to all products to be generated. The SKU of each individual product will be composed of this value together with values of the selected options."),
    '#size' => 60,
    '#maxlength' => 255,
    '#weight' => 0,
    '#process' => array('commerce_bpc_process_fragment_field'),
  );

  $form['product']['title_fragment'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#required' => TRUE,
    '#description' => t("The part of the product title that is common to all products to be generated. The SKU of each individual product will be composed of this value together with values of the selected options."),
    '#size' => 60,
    '#maxlength' => 255,
    '#process' => array('commerce_bpc_process_fragment_field'),
  );


  // We simply attach all fields to the 'static_values' fieldset, and then
  // allow modules to act on each of the fields.
  field_attach_form('commerce_product', $new_product, $form['static_values'], $form_state, $language);
  commerce_bpc_process_field_form_elements($form, $form_state);

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create products'),
    '#submit' => array('commerce_bpc_create_bulk_form_submit'),
  );


   // If translation support is enabled, provide the suitable languages
  if (module_exists('entity_translation') && entity_translation_enabled('commerce_product')) {
    $handler = entity_translation_get_handler('commerce_product', $new_product);
    $translations = $handler->getTranslations();

    $form['static_values']['language'] = array(
      '#type' => 'select',
      '#title' => t('Language'),
      '#default_value' => isset($new_product->language) ? $new_product->language : '',
      '#options' => array(LANGUAGE_NONE => t('Language neutral')) + locale_language_list('name'),
      '#weight' => 190,
    );


    // Since this function may change the language of the submitted form values,
    // it has to be the first called.
    array_unshift($form['actions']['submit']['#submit'], 'commerce_bpc_create_bulk_form_translation_submit');


  }

  $form['static_values']['status'] = array(
    '#type' => 'radios',
    '#title' => t('Status'),
    '#description' => t('Disabled products cannot be added to shopping carts and may be hidden in administrative product lists.'),
    '#options' => array(
      '1' => t('Active'),
      '0' => t('Disabled'),
    ),
    '#default_value' => 1,
    '#required' => TRUE,
    '#weight' => 200,
  );

  return $form;
}

/**
 * Form validation handler for for commerce_bpc_create_bulk_form().
 *
 * @see commerce_bpc_create_bulk_form_submit()
 */
function commerce_bpc_create_bulk_form_validate($form, &$form_state) {
  // Check to make sure all of the SKUs to be generated are unique and not taken
  // yet.
  $skus = array();
  $data = array();
  $product_type = $form_state['values']['product_type'];

  $data['bulk_data'] = array(
    'product_type' => $product_type,
    'sku_fragment' => $form_state['values']['sku_fragment'],
    'title_fragment' => $form_state['values']['title_fragment'],
  );

  $combinations = commerce_bpc_get_combinations($form, $form_state);
  foreach ($combinations as $combination) {
    $data['bulk_data']['combination'] = $combination;
    $sku_pattern = commerce_bpc_setting('default', 'sku_pattern', $product_type);

    $sku = token_replace($sku_pattern, $data, array('sanitize' => FALSE));
    if (!commerce_product_validate_sku_unique($sku, '')) {
      form_set_error('sku_pattern', t('This pattern yielded one or more invalid SKUs, please use a different pattern. Check for token values that may have spaces.'));
      break;
    }
    elseif (in_array($sku, $skus)) {
      form_set_error('sku_pattern', t('This pattern yielded one or more duplicate SKUs, please use a different pattern.'));
      break;
    }
    else {
      $skus[] = $sku;
    }
  }
}

/**
 * Form submission handler for commerce_bpc_create_bulk_form().
 *
 * @see commerce_bpc_create_bulk_form_validate()
 */
function commerce_bpc_create_bulk_form_submit($form, &$form_state) {

  global $user;

  $product_type = $form_state['values']['product_type'];

  $combinations = commerce_bpc_get_combinations($form, $form_state);

  $data['bulk_data'] = array(
    'product_type' => $product_type,
    'sku_fragment' => $form_state['values']['sku_fragment'],
    'title_fragment' => $form_state['values']['title_fragment'],
  );

  $sku_pattern = commerce_bpc_setting('default', 'sku_pattern', $product_type);
  $sku_pattern = token_replace($sku_pattern, $data, array('sanitize' => FALSE));
  $title_pattern = commerce_bpc_setting('default', 'title_pattern', $product_type);
  $title_pattern = token_replace($title_pattern, $data, array('sanitize' => FALSE));
  $extras = array(
    'sku_pattern'   => $sku_pattern,
    'title_pattern' => $title_pattern,
    'uid' => $user->uid,
    'status' => $form_state['values']['static_values']['status'],
    'language' => $form_state['values']['static_values']['language'],
  );

  drupal_alter('commerce_bpc_submit', $form, $form_state);

  $static_values = $form_state['values']['static_values'];
  unset($static_values['status']);
  unset($static_values['language']);

  $product_ids = commerce_bpc_create_bulk_products($product_type, $combinations, $static_values, $extras);

  // Set a success message.
  if (count($product_ids) > 0) {
    drupal_set_message(t("Successfully generated @count new products.", array('@count' => count($product_ids))));
  }

  // Cannot use module_invoke_all(), since $form_state needs to be taken
  // by reference.
  foreach (module_implements('commerce_bpc_post_create') as $module) {
    $function = $module . '_commerce_bpc_post_create';
    $function($product_ids, $form_state);
  }

  $destinations = module_invoke_all('commerce_bpc_destinations', $form_state, $product_ids);
  drupal_alter('commerce_bpc_destinations', $destinations, $form_state, $product_ids);
  $_REQUEST['destinations'] = $destinations;
  $form_state['redirect'] = commerce_bpc_next_destination();
}

/**
 * Process the fields attached to the bulk creation form.
 *
 * This helper function for commerce_bpc_bulk_create_bulk_form() hides fields
 * that are not supposed to be shown on the bulk creation form and invokes
 * the hooks that operate on fields to make them combination fields.
 */
function commerce_bpc_process_field_form_elements(&$form, &$form_state) {
  $product_type = $form['product_type']['#value'];
  foreach (element_children($form['static_values']) as $field_key) {
    $element = $form['static_values'][$field_key];
    if (@empty($element[$element['#language']])) {
      continue;
    }
    $field_name = $element[$element['#language']]['#field_name'];
    $info = field_info_field($field_name);
    $instance_info = field_info_instance('commerce_product', $field_name, $product_type);
    // Filter out fields that are set to be hidden.
    if (!_commerce_bpc_get_value($instance_info, array('commerce_bpc', 'show_field'), TRUE)) {
      unset($form['static_values'][$field_key]);
    }
    else {
      $types = array();
      $types[] = 'commerce_bpc_' . $info['type'] . '_form_element';
      if ($info['type'] != $info['module']) {
        $types[] = 'commerce_bpc_' . $info['module'] . '_form_element';
      }
      $types[] = 'commerce_bpc_form_element';
      $path = array('static_values', $field_key);
      drupal_alter($types, $form, $form_state, $path);
    }
  }
}

/**
 * Render API callback: Adds js-driven preview to fragment fields.
 *
 * This function is assigned as a #render-callback in
 * commerce_bpc_create_bulk_form().
 */
function commerce_bpc_process_fragment_field($element, &$form_state, &$form) {
  $product_type = $form['product_type']['#value'];
  $tokens = commerce_bpc_token_info_by_product_type($product_type);
  $samples = commerce_bpc_token_sample_values($product_type);

  $data['bulk_data']['combination'] = $samples['values'];
  $data['bulk_data']['product_type'] = $product_type;

  switch ($element['#name']) {
    case 'sku_fragment':
      $pattern = commerce_bpc_setting('default', 'sku_pattern', $product_type);
      $token = '[bulk_defaults:entered_sku]';
      $target_id = 'sku_example';
      $label = t('Example variant SKU:');
      break;
    case 'title_fragment':
      $pattern = commerce_bpc_setting('default', 'title_pattern', $product_type);
      $token = '[bulk_defaults:entered_title]';
      $target_id = 'title_example';
      $label = t('Example variant title:');
      break;
  }
  $pattern = token_replace($pattern, $data, array('sanitize' => FALSE));
  $wrapper_id = $target_id . '_wrapper';
  $element['#field_suffix'] = '<small id="' . $wrapper_id . '">' . $label . ' <span id="' . $target_id . '"></span></small>';


  $js_settings = array(
    'type' => 'setting',
    'data' => array(
      'commerce_bpc' => array(
        '#' . $element['#id'] => array(
          'pattern' => $pattern,
          'target' => '#' . $target_id,
          'target_wrapper' => '#' . $wrapper_id,
          'token' => $token,
        ),
      ),
    ),
  );
  $element['#attached']['js'][] = drupal_get_path('module', 'commerce_bpc') . '/js/pattern.js';
  $element['#attached']['js'][] = $js_settings;

  return $element;
}
/**
 * Submit callback for commerce_bpc_create_bulk_form().
 *
 * Checks if translation is enabled for the product and handles language changes.
 * Since this handler may change the language of submitted form values it should
 * be the first submit handler called.
 *
 */
function commerce_bpc_create_bulk_form_translation_submit($form, &$form_state) {
  // Get an array of available languages.
  $available_languages = field_content_languages();
  $bundle = $form_state['values']['product_type'];

  $static_values = &$form_state['values']['static_values'];

  foreach (field_info_instances('commerce_product', $bundle) as $instance) {
    $field_name = $instance['field_name'];
    $field = field_info_field($field_name);

    // We only treat fields in 'static_values' for now.
    if (isset($static_values[$field_name]) && $field['translatable']){
      $previous_language = $form['static_values'][$field_name]['#language'];
      $current_language = $static_values['language'];

      // Handle a possible language change; new language values are inserted
      // and the previous values are deleted.
      if ($previous_language != $current_language) {
        $static_values[$field_name][$current_language] = $static_values[$field_name][$previous_language];
      $static_values[$field_name][$previous_language] = array();
      }
    }
  }
}