<?php

/**
 * @file
 * Provides basic form function for ERPAL output type administration.
 */

/**
 * Generates the output type editing form.
 */
function erpal_output_type_form($form, &$form_state, $erpal_output_type, $op = 'edit') {

  if ($op == 'clone') {
    $erpal_output_type->label .= ' (cloned)';
    $erpal_output_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $erpal_output_type->label,
    '#description' => t('The human-readable name of this output type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($erpal_output_type->type) ? $erpal_output_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $erpal_output_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'erpal_output_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this output type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($erpal_output_type->description) ? $erpal_output_type->description : '',
    '#description' => t('Description about the output type.'),
  );

  $units_info = units_measure_load_multiple(FALSE);
  $options = array();
  foreach ($units_info as $measure) {
    $options[$measure->measure] = $measure->label;
  }

  $form['unit'] = array(
    '#title' => t('Unit type'),
    '#type' => 'select',
    '#default_value' => variable_get('output_' . $erpal_output_type->type, 'time'),
    '#description' => t('Select unit type for output.'),
    '#required' => TRUE,
    '#options' => $options,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save output type'),
    '#weight' => 40,
  );

  if (!$erpal_output_type->isLocked() && $op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete output type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('erpal_output_type_form_submit_delete'),
    );
  }

  return $form;
}

/**
 * Submit handler for creating/editing erpal_output_type.
 */
function erpal_output_type_form_submit(&$form, &$form_state) {
  $erpal_output_type = entity_ui_form_submit_build_entity($form, $form_state);
  variable_set('output_' . $erpal_output_type->type, $form_state['values']['unit']);
  // Save and go back.
  $erpal_output_type->save();

  // If we create new output type we need to add default fields to it.
  if ($form_state['op'] == 'add') {
    erpal_output_type_add_default_fields($erpal_output_type, $form_state['values']['unit']);
  }

  // Redirect user back to list of output types.
  $form_state['redirect'] = ERPAL_OUTPUT_ADMIN_PATH;
}

/**
 * Submit handler for deletion button for erpal_output_type.
 */
function erpal_output_type_form_submit_delete(&$form, &$form_state) {
  variable_del('output_' . $form_state['erpal_output_type']->type);
  $form_state['redirect'] = ERPAL_OUTPUT_ADMIN_PATH . '/manage/' . $form_state['erpal_output_type']->type . '/delete';
}

/**
 * Add default fields to newly created output type.
 */
function erpal_output_type_add_default_fields($output_type, $unit_type) {
  $type = $output_type->type;

  erpal_output_include('fields.defaults');
  $fields = _erpal_output_type_default_fields($type, $unit_type);
  $instances = _erpal_output_type_default_field_instances($type, $unit_type);
  erpal_core_add_fields($fields, 'field');
  erpal_core_add_fields($instances, 'instance');
}
