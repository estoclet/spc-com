<?php

/**
 * @file
 * Adds fields to the resource entity type used by ERPAL Core.
 */

/**
 * Define the default fields for the ERPAL resource entity type.
 *
 * @return array
 *   Return default fields to be attached to newly created resource type.
 *
 * @see erpal_resource_type_add_default_fields()
 */
function _erpal_resource_type_default_fields($type) {
  $fields = array();
  $types = array_keys(erpal_resource_types());
  $types[] = $type;
  // Exported field_base: 'field_$type_source'
  $fields[] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_resource',
    'foreign keys' => array(
      'erpal_resource' => array(
        'columns' => array(
          'target_id' => 'resource_id',
        ),
        'table' => 'erpal_resource',
      ),
    ),
    'indexes' => array(
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 0,
    'module' => 'entityreference',
    'settings' => array(
      'handler' => 'base',
      'handler_settings' => array(
        'behaviors' => array(
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
        'sort' => array(
          'type' => 'none',
        ),
        'target_bundles' => drupal_map_assoc($types),
      ),
      'target_type' => 'erpal_resource',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  return $fields;
}

/**
 * Defines the default field instances for the ERPAL resource entity type.
 *
 * @param string $type
 *   resource type to get instances for.
 *
 * @return array
 *   Field instances.
 */
function _erpal_resource_type_default_field_instances($type = NULL) {
  $instances = array();
  if (strpos($type, ERPAL_RESOURCE_DELIMITER) !== FALSE) {
    list($entity_type, $entity_bundle) = explode(ERPAL_RESOURCE_DELIMITER, $type);
    // Location field.
    $instances[] = array(
      'bundle' => $entity_bundle,
      'default_value' => NULL,
      'default_value_function' => '',
      'deleted' => 0,
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'entityreference',
          'settings' => array(
            'link' => 1,
          ),
          'type' => 'entityreference_label',
          'weight' => 16,
        ),
        'full' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 5,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 5,
        ),
      ),
      'entity_type' => $entity_type,
      'field_name' => 'field_resource',
      'label' => 'Resource',
      'required' => 0,
      'settings' => array(
        'behaviors' => array(
          'prepopulate' => array(
            'status' => 0,
          ),
        ),
        'dfv' => array(
          'parent' => '',
          'status' => 0,
          'view' => '',
        ),
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'field_extrawidgets',
        'settings' => array(
          'match_operator' => 'CONTAINS',
          'path' => '',
          'references_dialog_add' => 1,
          'references_dialog_edit' => 1,
          'references_dialog_search' => 1,
          'references_dialog_search_view' => '',
          'size' => 60,
        ),
        'type' => 'field_extrawidgets_hidden',
        'weight' => 3,
      ),
    );
  }
  return $instances;
}
