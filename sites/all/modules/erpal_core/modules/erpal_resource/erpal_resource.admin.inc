<?php

/**
 * @file
 * Provides basic resource management functionality in ERPAL Core.
 */

/**
 * Generates the resource type editing form.
 */
function erpal_resource_type_form($form, &$form_state, $erpal_resource_type, $op = 'edit') {

  if ($op == 'clone') {
    $erpal_resource_type->label .= ' (cloned)';
    $erpal_resource_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $erpal_resource_type->label,
    '#description' => t('The human-readable name of this resource type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($erpal_resource_type->type) ? $erpal_resource_type->type : '',
    '#maxlength' => 32,
    '#disabled' => $erpal_resource_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'erpal_resource_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this resource type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($erpal_resource_type->description) ? $erpal_resource_type->description : '',
    '#description' => t('Description about the resource type.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save resource type'),
    '#weight' => 40,
  );

  if (!$erpal_resource_type->isLocked() && $op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete resource type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('erpal_resource_type_form_submit_delete'),
    );
  }

  return $form;
}

/**
 * Submit handler for creating/editing erpal_resource_type.
 */
function erpal_resource_type_form_submit(&$form, &$form_state) {
  $erpal_resource_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $erpal_resource_type->save();

  // If we create new resource type we need to add default fields to it.
  if ($form_state['op'] == 'add') {
    erpal_resource_type_add_default_fields($erpal_resource_type);
  }

  // Redirect user back to list of resource types.
  $form_state['redirect'] = 'admin/erpal/resource-types';
}

/**
 * Submit handler for deletion button for erpal_resource_type.
 */
function erpal_resource_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/erpal/resource-types/manage/' . $form_state['erpal_resource_type']->type . '/delete';
}

/**
 * Add default fields to newly created resource type.
 */
function erpal_resource_type_add_default_fields($resource_type) {
  $type = $resource_type->type;

  module_load_include('inc', 'erpal_resource', 'erpal_resource.fields');
  $fields = _erpal_resource_type_default_fields($type);
  $instances = _erpal_resource_type_default_field_instances($type);
  erpal_core_add_fields($fields, 'field');
  erpal_core_add_fields($instances, 'instance');
}
