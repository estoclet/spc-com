<?php

/**
 * @file
 * Entity support.
 */

/**
 * Implements hook_property_info().
 */
function erpal_resource_entity_property_info() {
  $info = array();

  // Add meta-data about erpal_resource properties.
  $properties = &$info['erpal_resource']['properties'];

  $properties['resource_id'] = array(
    'label' => t('resource ID'),
    'description' => t('The primary identifier for an resource.'),
    'type' => 'integer',
    'schema field' => 'resource_id',
  );
  $properties['type'] = array(
    'label' => t('Type'),
    'description' => t('The type (bundle) of the resource.'),
    'type' => 'token',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer site configuration',
    'options list' => 'erpal_resource_type_get_names',
    'required' => TRUE,
    'schema field' => 'type',
  );
  $properties['title'] = array(
    'label' => t('Title'),
    'description' => t('Title of the resource.'),
    'type' => 'text',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer site configuration',
    'required' => TRUE,
    'schema field' => 'title',
  );
  $properties['uid'] = array(
    'label' => t('User ID'),
    'type' => 'integer',
    'description' => t("The unique ID of the user created resource."),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer site configuration',
    'schema field' => 'uid',
  );
  $properties['source_id'] = array(
    'label' => t('source ID'),
    'type' => 'integer',
    'description' => t("The unique ID of the source entity."),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer site configuration',
    'schema field' => 'source_id',
  );
  $properties['created'] = array(
    'label' => t('Date created'),
    'description' => t('The date the resource was created.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'dminister site configuration',
    'schema field' => 'created',
  );
  $properties['changed'] = array(
    'label' => t('Date updated'),
    'description' => t('The date the resource was last updated.'),
    'type' => 'date',
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'Administer site configuration',
    'schema field' => 'changed',
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

  // Metadata of resource Type.
  $properties = &$info['erpal_resource_type']['properties'];

  $properties['id'] = array(
    'label' => t('Resource Type ID'),
    'description' => t('Primary Key: Unique resource type ID.'),
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
