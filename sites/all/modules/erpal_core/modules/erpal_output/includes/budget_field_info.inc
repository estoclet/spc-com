<?php

/**
 * @file
 * Provides an output budget field.
 */

/**
 * Implements hook_field_info().
 */
function erpal_output_field_info() {
  return array(
    'erpal_output_budget' => array(
      'label' => t('Output budget'),
      'description' => t('Budgets related to output'),
      'default_widget' => 'erpal_output_budget_field',
      'default_formatter' => 'erpal_output_budget_default',
      'module' => 'erpal_output',
      'settings' => array(
        'target_type' => 'erpal_budget',
        'handler' => 'base',
      ),
      'property_type' => 'erpal_output_budget',
      'property_callbacks' => array('erpal_output_budget_field_property_callback'),
    ),
  );
}

/**
 * Implements hook_field_info_alter().
 */
function erpal_output_field_info_alter(&$info) {
  foreach ($info as $field_type => &$field_type_info) {

    if ($field_type == 'erpal_output_budget') {

      // Enable entityreference_prepopulate behavior for budget field.
      $field_type_info += array('instance_settings' => array());
      $field_type_info['instance_settings'] += array(
        'behaviors' => array(
          'prepopulate' => array(
            'action' => 'hide',
            'action_on_edit' => 1,
            'fallback' => 'hide',
            'providers' => array(
              'og_context' => FALSE,
              'url' => 1,
            ),
            'skip_perm' => 0,
            'status' => 1,
          ),
        ),
      );
    }
  }
}

/**
 * Additional callback to adapt the property info for budget fields.
 *
 * @see entity_metadata_field_entity_property_info()
 */
function erpal_output_budget_field_property_callback(&$info, $entity_type, $field, $instance, $field_type) {
  if ($field['type'] == 'erpal_output_budget') {
    $property = &$info[$entity_type]['bundles'][$instance['bundle']]['properties'][$field['field_name']];

    $property['getter callback'] = 'entity_metadata_field_verbatim_get';
    $property['setter callback'] = 'entity_metadata_field_verbatim_set';
    unset($property['query callback']);

    $property['property info'] = erpal_output_budget_field_entity_property_info();
  }
}

/**
 * Defines info for the properties of the field_output_budget data structure.
 */
function erpal_output_budget_field_entity_property_info() {
  return array(
    'value' => array(
      'type' => 'decimal',
      'label' => t('Value'),
      'setter callback' => 'entity_property_verbatim_set',
    ),
    'budget' => array(
      'type' => 'erpal_budget',
      'label' => t('Budget'),
      'schema field' => 'target_id',
      'setter callback' => 'entity_property_verbatim_set',
    ),
  );
}

/**
 * Implements hook_field_widget_info().
 */
function erpal_output_field_widget_info() {
  return array(
    'erpal_output_budget_field' => array(
      'label' => t('Select list'),
      'field types' => array('erpal_output_budget'),
      'behaviors' => array(
        'multiple values' => FIELD_BEHAVIOR_CUSTOM,
      ),
    ),
  );
}

/**
 * Implements hook_field_access().
 *
 * @see entityreference_prepopulate_field_access().
 */
function erpal_output_field_access($op, $field, $entity_type, $entity, $account) {
  if ($op != 'edit' || $field['type'] != 'erpal_output_budget') {
    return;
  }

  if (empty($entity)) {
    // $entity might be NULL, so return early.
    // @see field_access().
    return;
  }

  list($id,,$bundle) = entity_extract_ids($entity_type, $entity);
  $instance = field_info_instance($entity_type, $field['field_name'], $bundle);
  if (empty($instance['settings']['behaviors']['prepopulate']['status'])) {
    return;
  }

  $settings = $instance['settings']['behaviors']['prepopulate'];

  if (!empty($settings['skip_perm']) && user_access($settings['skip_perm'])) {
    return;
  }

  if ($id && empty($entity->is_new)) {
    // The field need to be hidden when editing the entity.
    return $settings['action'] == 'hide' && !empty($settings['action_on_edit']) ? FALSE : NULL;
  }

  if ($settings['action'] == 'hide') {
    // If entity is already saved and not just inserted, deny access, otherwise
    // ignore.
    return $id && empty($entity->is_new) ? FALSE : NULL;
  }

  $ids = entityreference_prepopulate_get_values($field, $instance);

  if (!$ids && $settings['fallback'] == 'hide') {
    return FALSE;
  }
}

/**
 * Implements hook_field_widget_form().
 */
function erpal_output_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  switch ($instance['widget']['type']) {
    case 'erpal_output_budget_field':
      $instance['widget']['type'] = 'options_select';
      $element = options_field_widget_form($form, $form_state, $field, $instance, $langcode, $items, $delta, $element);

      // Replace default validate functions to custom.
      $element['#element_validate'] = array('erpal_output_field_widget_validate');
      break;
  }
  return $element;
}

/**
 * Form element validation handler for options element.
 */
function erpal_output_field_widget_validate($element, &$form_state) {
  if ($element['#required'] && $element['#value'] == '_none') {
    form_error($element, t('!name field is required.', array('!name' => $element['#title'])));
  }

  // Transpose selections from field => delta to delta => field, turning
  // multiple selected options into multiple parent elements.
  $items = _options_form_to_storage($element);

  if (!empty($form_state['erpal_output'])) {
    $budget_values = field_get_items('erpal_output', $form_state['erpal_output'], 'field_output_budget');
    foreach ($items as &$item) {
      $key = FALSE;
      if (is_array($budget_values) && !empty($budget_values)) {
        foreach ($budget_values as $delta => $budget_value) {
          if ($budget_value['target_id'] == $item['target_id']) {
            $key = $delta;
          }
        }
      }
      $item['value'] = $key !== FALSE ? $budget_values[$key]['value'] : 0;
    }
  }

  form_set_value($element, $items, $form_state);
}

/**
 * Implements hook_options_list().
 */
function erpal_output_options_list($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {

  $wrapper = entity_metadata_wrapper('erpal_output', $entity);
  $resource_id = !empty($wrapper->field_resource) ? $wrapper->field_resource->raw() : NULL;

  if (empty($resource_id)) {

    if (!empty($_REQUEST['field_resource'])) {
      $resource_id = intval($_REQUEST['field_resource']);
    }
    else {
      return array();
    }
  }

  // Get field options from custom selection handler.
  if (!$options = erpal_output_get_budgets_selection_handler($field, $instance, $entity_type, $entity)
      ->getReferencableEntities($resource_id, '=')) {

    return array();
  }

  // Rebuild the array, by changing the bundle key into the bundle label.
  $target_type = $field['settings']['target_type'];
  $entity_info = entity_get_info($target_type);

  $return = array();
  foreach ($options as $bundle => $entity_ids) {
    $bundle_label = check_plain($entity_info['bundles'][$bundle]['label']);
    $return[$bundle_label] = $entity_ids;
  }
  return count($return) == 1 ? reset($return) : $return;
}

/**
 * Get the selection handler for a given output_budgets field.
 */
function erpal_output_get_budgets_selection_handler($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
  $class = 'ERPALOutput_SelectionHandler_BudgetsField';

  if (class_exists($class)) {
    return call_user_func(array($class, 'getInstance'), $field, $instance, $entity_type, $entity);
  }
  else {
    return EntityReference_SelectionHandler_Broken::getInstance($field, $instance, $entity_type, $entity);
  }
}

/**
 * Implements hook_field_is_empty().
 */
function erpal_output_field_is_empty($item, $field) {
  if (empty($item['target_id']) || $item['target_id'] == '_none') {
    return TRUE;
  }
  return FALSE;
}

/**
 * Implements hook_field_formatter_info().
 */
function erpal_output_field_formatter_info() {
  return array(
    'erpal_output_budget_default' => array(
      'label' => t('Budget and value'),
      'field types' => array('erpal_output_budget'),
      'settings' => array(
        'budget_title' => TRUE,
        'link' => FALSE,
        'separator' => ' ',
        'thousand_separator' => ' ',
        'decimal_separator' => '.',
        'scale' => 2,
        'prefix_suffix' => TRUE,
      ),
    ),
  );
}

/**
 * Implements hook_field_formatter_prepare_view().
 */
function erpal_output_field_formatter_prepare_view($entity_type, $entities, $field, $instances, $langcode, &$items, $displays) {
  entityreference_field_formatter_prepare_view($entity_type, $entities, $field, $instances, $langcode, $items, $displays);
}

/**
 * Implements hook_field_formatter_view().
 */
function erpal_output_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $result = array();
  switch ($display['type']) {
    case 'erpal_output_budget_default':
      $display['type'] = 'entityreference_label';
      $entities = entityreference_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display);

      $display['type'] = 'number_decimal';
      $values = number_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display);

      // Take all values from output and prepare it.
      $field_output_unit = field_get_items('erpal_output', $entity, 'field_output_unit');
      $unit = units_unit_load($field_output_unit[0]['target_id']);

      foreach ($items as $delta => $item) {
        $output = '';
        if ($display['settings']['budget_title']) {
          $output .= $entities[$delta]['#markup'] . check_plain($display['settings']['separator']);
        }
        $output .= $values[$delta]['#markup'] . ' ' . $unit->label;
        $result[$delta] = array('#markup' => $output);
      }
      break;
  }

  return $result;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function erpal_output_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];
  $element = array();

  if ($display['type'] == 'erpal_output_budget_default') {
    $element['budget_title'] = array(
      '#title' => t('Show budget title'),
      '#type' => 'checkbox',
      '#default_value' => $settings['budget_title'],
    );
    $element['separator'] = array(
      '#type' => 'textfield',
      '#title' => t('Separator'),
      '#default_value' => $settings['separator'],
      '#states' => array(
        'visible' => array(
          ':input[name$="[settings][budget_title]"]' => array('checked' => TRUE),
        ),
      ),
    );

    // Settings for budget field.
    $instance['display'][$view_mode]['type'] = 'entityreference_label';
    $element += entityreference_field_formatter_settings_form($field, $instance, $view_mode, $form, $form_state);

    // Settings for value field.
    $instance['display'][$view_mode]['type'] = 'number_decimal';
    $element += number_field_formatter_settings_form($field, $instance, $view_mode, $form, $form_state);
  }

  return $element;
}

/**
 * Implements hook_field_formatter_settings_summary().
 */
function erpal_output_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];

  $summary = '';
  if ($display['type'] == 'erpal_output_budget_default') {

    // Output settings summary for budget field.
    $instance['display'][$view_mode]['type'] = 'entityreference_label';
    $summary .= entityreference_field_formatter_settings_summary($field, $instance, $view_mode);

    $summary .= '<br />';

    // Output settings summary for value field.
    $instance['display'][$view_mode]['type'] = 'number_decimal';
    $summary .= number_field_formatter_settings_summary($field, $instance, $view_mode);
  }

  return $summary;
}
