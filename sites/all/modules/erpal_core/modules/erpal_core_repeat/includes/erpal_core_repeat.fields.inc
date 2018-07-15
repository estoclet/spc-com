<?php

/**
 * @file
 * Adds fields to the ERPAL entity type.
 */

/**
 * Define date field for the ERPAL Budget and Output entity types.
 *
 * @return array
 *   Return default fields to be attached to newly created types.
 *
 * @see field_create_field()
 */
function _erpal_core_repeat_default_fields($entity_type, $field_name) {
  $fields = array();

  // Exported field_base
  $fields[] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array('erpal_budget', 'erpal_output'),
    'field_name' => $field_name,
    'foreign keys' => array(),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'date',
    'settings' => array(
      'cache_count' => 4,
      'cache_enabled' => 0,
      'granularity' => array(
        'day' => 'day',
        'month' => 'month',
        'year' => 'year',
      ),
      'repeat' => 0,
      'timezone_db' => '',
      'todate' => 'required',
      'tz_handling' => 'none',
    ),
    'translatable' => 0,
    'type' => 'datetime',
  );

  return $fields;
}

/**
 * Defines the default field instances for given entity type and bundle.
 *
 * @return array
 *   Field instances.
 */
function _erpal_core_repeat_default_field_instances($entity_type, $bundle, $field_name) {
  $instances = array();

  // Date repeat field.
  $instances[] = array(
    'bundle' => $bundle,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'date',
        'settings' => array(
          'format_type' => 'short',
          'fromto' => 'both',
          'multiple_from' => '',
          'multiple_number' => '',
          'multiple_to' => '',
          'show_repeat_rule' => 'show',
        ),
        'type' => 'date_default',
        'weight' => 18,
      ),
    ),
    'entity_type' => $entity_type,
    'field_name' => $field_name,
    'label' => 'Date',
    'required' => 0,
    'settings' => array(
      'default_value' => 'blank',
      'default_value2' => 'same',
      'default_value_code' => '',
      'default_value_code2' => '',
      'dfv' => array(
        'parent' => '',
        'status' => 0,
        'view' => '',
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'date',
      'settings' => array(
        'increment' => 15,
        'input_format' => 'M j Y - H:i:s',
        'input_format_custom' => '',
        'label_position' => 'above',
        'repeat_collapsed' => 0,
        'text_parts' => array(),
        'year_range' => '-3:+3',
      ),
      'type' => 'date_popup',
      'weight' => 5,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Date');

  return $instances;
}
