<?php

/**
 * @file
 * Provides basic budget management functionality in ERPAL Core.
 */

/**
 * Generates the budget type editing form.
 */
function erpal_budget_type_form($form, &$form_state, $erpal_budget_type, $op = 'edit') {

  if ($op == 'clone') {
    $erpal_budget_type->label .= ' (cloned)';
    $erpal_budget_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $erpal_budget_type->label,
    '#description' => t('The human-readable name of this budget type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($erpal_budget_type->type) ? $erpal_budget_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $erpal_budget_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'erpal_budget_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this budget type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($erpal_budget_type->description) ? $erpal_budget_type->description : '',
    '#description' => t('Description about the budget type.'),
  );

  $units_info = units_measure_load_multiple(FALSE);
  $options = array();
  foreach ($units_info as $measure) {
    $options[$measure->measure] = $measure->label;
  }

  $form['unit'] = array(
    '#title' => t('Unit type'),
    '#type' => 'select',
    '#default_value' => variable_get('budget_' . $erpal_budget_type->type, 'time'),
    '#description' => t('Select unit type for output.'),
    '#required' => TRUE,
    '#options' => $options,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save budget type'),
    '#weight' => 40,
  );

  if (!$erpal_budget_type->isLocked() && $op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete budget type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('erpal_budget_type_form_submit_delete'),
    );
  }

  return $form;
}

/**
 * Submit handler for creating/editing erpal_budget_type.
 */
function erpal_budget_type_form_submit(&$form, &$form_state) {
  $erpal_budget_type = entity_ui_form_submit_build_entity($form, $form_state);
  variable_set('budget_' . $erpal_budget_type->type, $form_state['values']['unit']);
  // Save and go back.
  $erpal_budget_type->save();

  // If we create new budget type we need to add default fields to it.
  if ($form_state['op'] == 'add') {
    erpal_budget_type_add_default_fields($erpal_budget_type, $form_state['values']['unit']);
  }

  // Redirect user back to list of budget types.
  $form_state['redirect'] = 'admin/erpal/budget-types';
}

/**
 * Submit handler for deletion button for erpal_budget_type.
 */
function erpal_budget_type_form_submit_delete(&$form, &$form_state) {
  variable_del('budget_' . $form_state['erpal_budget_type']->type);
  $form_state['redirect'] = 'admin/erpal/budget-types/manage/' . $form_state['erpal_budget_type']->type . '/delete';
}

/**
 * Add default fields to newly created budget type.
 */
function erpal_budget_type_add_default_fields($budget_type, $unit_type) {
  $type = $budget_type->type;

  module_load_include('inc', 'erpal_budget', 'erpal_budget.fields');
  $fields = _erpal_budget_type_default_fields($type, $unit_type);
  $instances = _erpal_budget_type_default_field_instances($type, $unit_type);
  erpal_core_add_fields($fields, 'field');
  erpal_core_add_fields($instances, 'instance');
}
