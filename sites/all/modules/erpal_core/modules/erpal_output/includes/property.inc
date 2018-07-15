<?php

/**
 * @file
 * Entity support.
 */

/**
 * Implements hook_property_info().
 */
function erpal_output_entity_property_info() {
  $info = array();

  // Add meta-data about erpal_output properties.
  $properties = &$info['erpal_output']['properties'];

  $properties['output_id'] = array(
    'label' => t('output ID'),
    'description' => t('The primary identifier for an output.'),
    'type' => 'integer',
    'schema field' => 'output_id',
  );
  $properties['type'] = array(
    'label' => t('Type'),
    'description' => t('The type (bundle) of the output.'),
    'type' => 'token',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer site configuration',
    'options list' => 'erpal_output_type_get_names',
    'required' => TRUE,
    'schema field' => 'type',
  );
  $properties['title'] = array(
    'label' => t('Title'),
    'description' => t('Title of the output.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer site configuration',
    'required' => TRUE,
    'schema field' => 'title',
  );
  $properties['uid'] = array(
    'label' => t('User ID'),
    'type' => 'integer',
    'description' => t("The unique ID of the user created output."),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer site configuration',
    'schema field' => 'uid',
  );
  $properties['created'] = array(
    'label' => t('Date created'),
    'description' => t('The date the output was created.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'dminister site configuration',
    'schema field' => 'created',
  );
  $properties['changed'] = array(
    'label' => t('Date updated'),
    'description' => t('The date the output was last updated.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'Administer site configuration',
    'schema field' => 'changed',
  );
  $properties['saldo'] = array(
    'label' => t('Saldo'),
    'type' => 'decimal',
    'description' => t("Output saldo value. Subtraction between Total unit and assigned budgets values."),
    'getter callback' => '_erpal_output_saldo_metadata_get_property',
    'computed' => TRUE,
    'entity views field' => TRUE,
  );

  if (module_exists('uuid')) {
    $properties['uuid'] = array(
      'label' => t('UUID'),
      'type' => 'text',
      'description' => t('The universally unique ID.'),
      'schema field' => 'uuid',
    );

    $properties['vuuid'] = array(
      'label' => t('Revision UUID'),
      'type' => 'text',
      'description' => t("The revision's universally unique ID."),
      'schema field' => 'vuuid',
    );
  }

  // Metadata of output Type.
  $properties = &$info['erpal_output_type']['properties'];

  $properties['id'] = array(
    'label' => t('Output Type ID'),
    'description' => t('Primary Key: Unique output type ID.'),
    'type' => 'integer',
    'schema field' => 'id',
  );
  $properties['type'] = array(
    'label' => t('Type'),
    'description' => t('The machine-readable name of this type.'),
    'type' => 'token',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer site configuration',
    'required' => TRUE,
    'schema field' => 'type',
  );
  $properties['label'] = array(
    'label' => t('Label'),
    'description' => t('The human-readable name of this type.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer site configuration',
    'required' => TRUE,
    'schema field' => 'label',
  );
  $properties['description'] = array(
    'label' => t('Description'),
    'description' => t('A brief description of this type.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer site configuration',
    'schema field' => 'description',
  );

  return $info;
}

/**
 * Callback for getting output's saldo value property.
 *
 * Subtraction between Total unit and assigned budgets values.
 *
 * @see erpal_output_entity_property_info()
 */
function _erpal_output_saldo_metadata_get_property($output, array $options, $name, $entity_type) {
  $output_wrapper = entity_metadata_wrapper('erpal_output', $output);
  $budgets = $output_wrapper->field_output_budget->value();

  $total_budget_values = 0;
  if (!empty($budgets)) {
    $budgets = !empty($budgets[0]) ? $output_wrapper->field_output_budget : array($output_wrapper->field_output_budget);
    foreach ($budgets as $output_budget) {
      $total_budget_values += $output_budget->value->value();
    }
  }

  return $output_wrapper->field_output_unit->value->value() - $total_budget_values;
}
