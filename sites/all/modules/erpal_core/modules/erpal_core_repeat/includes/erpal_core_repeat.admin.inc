<?php

/**
 * @file
 * Alter ERPAL Budget and Output type forms.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Adds custom submit function to edit budget entity.
 */
function _erpal_core_repeat_type_form_alter(&$form, &$form_state) {
  form_load_include($form_state, 'inc', 'erpal_core_repeat', 'includes/erpal_core_repeat.admin');

  $form_entity_type = $form_state['entity_type'];
  $entity_bundle = $form_state[$form_entity_type]->type;
  $entity_info = entity_get_info($form_entity_type);
  $entity_type = $entity_info['bundle of'];
  $form_edit_id = $entity_type . '_edit_' . $entity_bundle . '_form';

  $enabled_types = variable_get('erpal_core_repeat_enabled_types', array());

  // Output checkbox
  $form['show_date_repeat'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use date repeat field'),
    '#default_value' => in_array($form_edit_id, $enabled_types),
  );

  $form['#submit'][] = 'erpal_core_repeat_type_form_submit';
}

/**
 * Submit callback for erpal_budget_form.
 *
 * Creates budget entity regarding repeatable date field.
 */
function erpal_core_repeat_type_form_submit($form, &$form_state) {
  module_load_include('inc', 'erpal_core_repeat', 'includes/erpal_core_repeat.fields');
  $form_entity_type = $form_state['entity_type'];
  $entity_info = entity_get_info($form_entity_type);
  $entity_type = $entity_info['bundle of'];
  $entity_bundle = $form_state[$form_entity_type]->type;
  $form_edit_id = $entity_type . '_edit_' . $entity_bundle . '_form';

  // Adds default date repeat field for current bundle if checkbox checked.
  if ($form_state['values']['show_date_repeat']) {
    $fields = _erpal_core_repeat_default_fields($entity_type, ERPAL_CORE_REPEAT_DATE_FIELD);
    erpal_core_add_fields($fields, 'field');

    // Adds default date repeat field.
    $instances = _erpal_core_repeat_default_field_instances($entity_type, $entity_bundle, ERPAL_CORE_REPEAT_DATE_FIELD);
    erpal_core_add_fields($instances, 'instance');

    // Adds current bundle form edit form_id to variable.
    $enabled_types = variable_get('erpal_core_repeat_enabled_types', array());
    if (!in_array($form_edit_id, $enabled_types)) {

      $enabled_types[] = $form_edit_id;
      variable_set('erpal_core_repeat_enabled_types', $enabled_types);
    }
  }
  else {
    // Removes default date field.
    $additional_params = array('include_inactive' => TRUE);
    $instances = field_read_instances(array('entity_type' => $entity_type, 'bundle' => $entity_bundle, 'field_name' => ERPAL_CORE_REPEAT_DATE_FIELD), $additional_params);
    foreach ($instances as $instance) {
      field_delete_instance($instance);
    }

    // Removes current bundle form edit form_id from variable.
    $enabled_types = variable_get('erpal_core_repeat_enabled_types', array());
    if (in_array($form_edit_id, $enabled_types)) {

      $key = array_search($form_edit_id, $enabled_types);
      unset($enabled_types[$key]);
      variable_set('erpal_core_repeat_enabled_types', $enabled_types);
    }
  }
}
