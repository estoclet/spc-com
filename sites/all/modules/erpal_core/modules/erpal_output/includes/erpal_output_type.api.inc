<?php

/**
 * @file
 * Provides main functions for erpal_output_type entity.
 */

/**
 * Implements hook_entity_info().
 */
function _erpal_output_type_entity_info() {
  return array(
    'erpal_output_type' => array(
      'label' => t('ERPAL Output Type'),
      'entity class' => 'ERPALOutputType',
      'controller class' => 'ERPALOutputTypeController',
      'base table' => 'erpal_output_type',
      'fieldable' => FALSE,
      'bundle of' => 'erpal_output',
      'exportable' => TRUE,
      'entity keys' => array(
        'id' => 'id',
        'name' => 'type',
        'label' => 'label',
      ),
      'module' => 'erpal_output',
      // Enable the entity API's admin UI.
      'admin ui' => array(
        'path' => ERPAL_OUTPUT_ADMIN_PATH,
        'file' => 'includes/erpal_output_type.form.inc',
        'controller class' => 'EntityDefaultUIController',
      ),
      'access callback' => 'erpal_output_type_access',
    ),
  );
}

/**
 * Load output Type.
 */
function erpal_output_type_load($output_type) {
  return erpal_output_types($output_type);
}

/**
 * List of output Types.
 */
function erpal_output_types($type_name = NULL) {
  $types = entity_load_multiple_by_name('erpal_output_type', isset($type_name) ? array($type_name) : FALSE);
  return isset($type_name) ? reset($types) : $types;
}

/**
 * Returns the human-readable names of an output types.
 *
 * @return array
 *   An array as used by hook_options_list().
 */
function erpal_output_type_get_names() {
  $names =& drupal_static(__FUNCTION__, array());
  if (empty($names)) {
    $types = erpal_output_types();
    foreach ($types as $type) {
      $names[$type->type] = check_plain($type->label);
    }
  }

  return $names;
}


/**
 * Returns the human-readable name of an output type.
 *
 * @param string $type_name
 *   The machine name for an output type.
 *
 * @return string
 *   A human readable name for the output type or FALSE.
 */
function erpal_output_type_get_name($type_name) {
  $names = erpal_output_type_get_names();
  if (isset($names[$type_name])) {
    return $names[$type_name];
  }
  else {
    return FALSE;
  }
}

/**
 * Creates or saves an output type.
 *
 * @param object $output_type
 *   The output type info to be saved
 */
function erpal_output_type_save($output_type) {
  return entity_get_controller('erpal_output_type')->save($output_type);
}

/**
 * Output type access callback.
 */
function erpal_output_type_access() {
  return user_access('administer output types');
}

/**
 * Implements hook_erpal_output_type_insert().
 */
function erpal_output_erpal_output_type_insert($output_type) {
  erpal_output_include('erpal_output_type.form');
  $unit_type = variable_get('output_' . $output_type->type, 'time');
  erpal_output_type_add_default_fields($output_type, $unit_type);
}

/**
 * Check if output type suitable for budget type.
 */
function erpal_output_type_suitable_budget_type($output_type, $budget_type) {
  $info_budget = field_info_instance('erpal_budget', 'field_budget_available', $budget_type);
  $info_output = field_info_instance('erpal_output', 'field_output_unit', $output_type);
  if ($info_budget['settings']['unit']['target_bundles'] == $info_output['settings']['unit']['target_bundles']) {
    return TRUE;
  }
  return FALSE;
}

