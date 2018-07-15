<?php

/**
 * @file
 * Settings for Custom Order Number.
 */

/**
 * Admin settings form for order number generation.
 */
function custom_order_number_admin_form($form, &$form_state, $type) {

  // Confirm form for changing the last number.
  if (isset($form_state['confirm_nr_change']) && !empty($form_state['confirm_nr_change'])) {
    $override = $form_state['values']['custom_order_number_' . $type . '_nr_override'];
    $form['nr_override'] = array('#type' => 'value', '#value' => $override);
    $form['type'] = array('#type' => 'value', '#value' => $type);
    return confirm_form($form,
      t('Are you sure you want to change the current @type id?', array('@type' => $type)),
      'admin/commerce/config/custom-order-number/' . $type,
      t('Changing the id might lead to gaps in your @type numbers, causing troubles in the accounting system.', array('@type' => $type)));
  }

  $form['#theme'] = 'system_settings_form';

  $last_number = custom_order_number_query_variable('custom_order_number_' . $type . '_nr_last');
  if (!isset($last_number['id'])) {
    $last_number = array('id' => 0);
  }

  $invoice_nr_options = array(
    CUSTOM_ORDER_NUMBER_METHOD_INFINITE => t('Infinite (one single number, that is never reset, and incremented at each order number generation)'),
    CUSTOM_ORDER_NUMBER_METHOD_YEARLY => t('Reset every year, with an id incremented at each order number generation (e.g. [date:custom:Y]-{id}, resulting in @order_number)', array('@order_number' => date('Y') . '-' . $last_number['id'])),
    CUSTOM_ORDER_NUMBER_METHOD_MONTHLY => t('Reset every month, with an id incremented at each order number generation (e.g. [date:custom:Y-m]-{id}, resulting in @order_number)', array('@order_number' => date('Y-m') . '-' . $last_number['id'])),
  );

  $form['custom_order_number_' . $type . '_nr_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable custom order number for this type?'),
    '#default_value' => variable_get('custom_order_number_' . $type . '_nr_enable', TRUE),
  );

  $form['custom_order_number_' . $type . '_nr_method'] = array(
    '#type' => 'radios',
    '#title' => t('Order number generation method'),
    '#default_value' => variable_get('custom_order_number_' . $type . '_nr_method', CUSTOM_ORDER_NUMBER_METHOD_YEARLY),
    '#options' => $invoice_nr_options,
  );

  $pattern_help_text = t('In addition to the generation method, a pattern for the
    order number can be set, e.g. to pre- or suffix the calculated number. The
    placeholder "{id}" is replaced with the generated number and *must* be
    included in the pattern. Tokens can be used, e.g. [date:custom:Y-m-d].
    If you are using the yearly pattern, a token for the current year must be
    included. For the montly pattern, tokens for the current month and year must
    be included.');
  $form['custom_order_number_' . $type . '_nr_pattern'] = array(
    '#type' => 'textfield',
    '#title' => t('Pattern'),
    '#required' => TRUE,
    '#default_value' => variable_get('custom_order_number_' . $type . '_nr_pattern', '[date:custom:Y]-{id}'),
    '#description' => $pattern_help_text,
  );

  $form['custom_order_number_' . $type . '_nr_override'] = array(
    '#type' => 'textfield',
    '#title' => t('Current order id'),
    '#description' => t('Use with caution! Only values bigger than the original value allowed. Next id being used: @next.', array('@next' => $last_number['id'] + 1)),
    '#size' => 10,
    '#default_value' => $last_number['id'],
  );
  $form['type'] = array('#type' => 'value', '#value' => $type);
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  return $form;
}

/**
 * Validation handler for Custom Order Number admin form.
 */
function custom_order_number_admin_form_validate($form, &$form_state) {
  if (empty($form_state['values']['nr_override'])) {
    $type = $form_state['values']['type'];
    $pattern = $form_state['values']['custom_order_number_' . $type . '_nr_pattern'];
    if (strpos($pattern, '{id}') === FALSE) {
      form_set_error('custom_order_number_' . $type . '_nr_pattern', t('Invalid pattern. {id} is missing.'));
    }
    elseif ($pattern === '{id}') {
      form_set_error('custom_order_number_' . $type . '_nr_pattern', t('You must include additional characters to {id}, as otherwise the order number might overlap with the order ID, leading the database exceptions.'));
    }
  
    // Check for valid characters.
    $test_nr = str_replace('{id}', 1, $pattern);
    $test_nr = token_replace($test_nr, array(), array('clear' => TRUE));
    if (!commerce_order_validate_number_characters($test_nr)) {
      form_set_error('custom_order_number_' . $type . '_nr_pattern', t('Invalid pattern. Only alphanumeric values, underscores and dashes are allowed.'));
    }
  }
}

/**
 * Submit handler for Custom Order Number admin form.
 */
function custom_order_number_admin_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  $type = $form_state['values']['type'];
  // Prepare confirmation form for order number override.
  if (isset($form_state['values']['custom_order_number_' . $type . '_nr_method'])) {
    $override = $form_state['values']['custom_order_number_' . $type . '_nr_override'];
    $last_number = custom_order_number_query_variable('custom_order_number_' . $type . '_nr_last');
    $last_number['id'] = isset($last_number['id']) ? $last_number['id'] : 0;
    if ($override && $override != $last_number['id'] && $override > $last_number['id']) {
      $form_state['rebuild'] = TRUE;
      $form_state['confirm_nr_change'] = TRUE;
    }
  }
  // Order number override has been confirmed.
  elseif (isset($form_state['values']['nr_override']) && isset($form_state['values']['confirm']) && !empty($form_state['values']['confirm'])) {
    $last_number = custom_order_number_query_variable('custom_order_number_' . $type . '_nr_last');
    $last_number['id'] = $form_state['values']['nr_override'];
    variable_set('custom_order_number_' . $type . '_nr_last', $last_number);
    drupal_set_message('Your order number has successfully been overridden.');
    return;
  }

  // Save all relevant settings.
  foreach ($form_state['values'] as $key => $value) {
    if ($key != 'custom_order_number_' . $type . '_nr_override') {
      variable_set($key, $value);
    }
  }

  if (!isset($form_state['confirm_nr_change'])) {
    // No number override, default success message.
    drupal_set_message(t('The configuration options for Сustom Order Number have been saved.'));
  }
  else {
    // Number override -> confirm form.
    drupal_set_message(t('Basic configuration options for Сustom Order Number have been saved. Change of order number requires confirmation.'));
  }
}
