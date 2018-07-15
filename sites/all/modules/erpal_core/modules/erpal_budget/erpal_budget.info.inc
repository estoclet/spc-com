<?php

/**
 * @file
 * Entity support.
 */

/**
 * Implements hook_property_info().
 */
function erpal_budget_entity_property_info() {
  $info = array();

  // Add meta-data about erpal_budget properties.
  $properties = &$info['erpal_budget']['properties'];

  $properties['budget_id'] = array(
    'label' => t('budget ID'),
    'description' => t('The primary identifier for an budget.'),
    'type' => 'integer',
    'schema field' => 'budget_id',
  );
  $properties['type'] = array(
    'label' => t('Type'),
    'description' => t('The type (bundle) of the budget.'),
    'type' => 'token',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer site configuration',
    'options list' => 'erpal_budget_type_get_names',
    'required' => TRUE,
    'schema field' => 'type',
  );
  $properties['title'] = array(
    'label' => t('Title'),
    'description' => t('Title of the budget.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer site configuration',
    'required' => TRUE,
    'schema field' => 'title',
  );
  $properties['uid'] = array(
    'label' => t('User ID'),
    'type' => 'integer',
    'description' => t("The unique ID of the user created budget."),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer site configuration',
    'schema field' => 'uid',
  );
  $properties['created'] = array(
    'label' => t('Date created'),
    'description' => t('The date the budget was created.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer site configuration',
    'schema field' => 'created',
  );
  $properties['changed'] = array(
    'label' => t('Date updated'),
    'description' => t('The date the budget was last updated.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer site configuration',
    'schema field' => 'changed',
  );
  $properties['available_percent'] = array(
    'label' => t('Available percent'),
    'description' => t('A percentage of current budget availability.'),
    'type' => 'integer',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer site configuration',
  );
  $properties['saldo'] = array(
    'label' => t('Saldo'),
    'type' => 'decimal',
    'description' => t("Budget saldo value. Subtraction between Total and available values."),
    'getter callback' => '_erpal_budget_saldo_metadata_get_property',
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

  // Metadata of budget Type.
  $properties = &$info['erpal_budget_type']['properties'];

  $properties['id'] = array(
    'label' => t('budget Type ID'),
    'description' => t('Primary Key: Unique budget type ID.'),
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
 * Callback for getting budget's saldo value property.
 *
 * Subtraction between Total and available values.
 *
 * @see erpal_budget_entity_property_info()
 */
function _erpal_budget_saldo_metadata_get_property($budget, array $options, $name, $entity_type) {
  $budget_wrapper = entity_metadata_wrapper('erpal_budget', $budget);

  $total = $budget_wrapper->field_budget_total->value->value();
  $available = $budget_wrapper->field_budget_available->value->value();

  return $total - $available;
}