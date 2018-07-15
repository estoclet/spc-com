<?php

/**
 * @file
 * Adds fields to the output entity type used by ERPAL Core.
 */

/**
 * Define the default fields for the ERPAL output entity type.
 *
 * @return array
 *   Return default fields to be attached to newly created output type.
 *
 * @see field_create_field()
 */
function _erpal_output_type_default_fields($type, $unit_type) {
  $fields = array();

  // Delivery Date field.
  $fields[] = array(
    'active' => TRUE,
    'cardinality' => 1,
    'entity_types' => array('erpal_output'),
    'field_name' => 'field_output_delivered_date',
    'locked' => TRUE,
    'module' => 'date',
    'settings' => array(
      'repeat' => 0,
      'granularity' => array(
        'month' => 'month',
        'day' => 'day',
        'hour' => 'hour',
        'minute' => 'minute',
        'year' => 'year',
        'second' => 'second',
      ),
      'tz_handling' => 'site',
      'timezone_db' => 'UTC',
      'todate' => '',
    ),
    'translatable' => FALSE,
    'type' => 'datetime',
  );

  // Delivered field.
  $fields[] = array(
    'active' => TRUE,
    'cardinality' => 1,
    'entity_types' => array('erpal_output'),
    'field_name' => 'field_output_delivered',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => TRUE,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        0 => 'Not delivered',
        1 => 'Delivered',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => FALSE,
    'type' => 'list_boolean',
  );

  // Budget ref field.
  $fields[] = array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(
      0 => 'erpal_output',
    ),
    'field_name' => 'field_output_budget',
    'foreign keys' => array(
      'erpal_budget' => array(
        'columns' => array(
          'target_id' => 'budget_id',
        ),
        'table' => 'erpal_budget',
      ),
    ),
    'indexes' => array(
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 0,
    'module' => 'erpal_output',
    'settings' => array(
      'handler' => 'base',
      'target_type' => 'erpal_budget',
    ),
    'translatable' => 0,
    'type' => 'erpal_output_budget',
  );

  // Unit field 'field_output_unit'.
  $fields[] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_output_unit',
    'foreign keys' => array(
      'units_unit' => array(
        'columns' => array(
          'target_id' => 'umid',
        ),
        'table' => 'units_unit',
      ),
    ),
    'indexes' => array(
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 0,
    'module' => 'mvf',
    'settings' => array(
      'meta_info' => array(
        'unit' => array(
          'field_type' => 'entityreference',
          'formatter' => 'entityreference_label',
          'label' => 'Unit Measure',
          'module' => 'entityreference',
          'not_supported_widgets' => array(
            0 => 'entityreference_autocomplete',
            1 => 'entityreference_autocomplete_tags',
          ),
          'widget' => 'options_select',
        ),
        'value' => array(
          'field_type' => 'number_float',
          'formatter' => 'number_decimal',
          'label' => 'Value',
          'module' => 'number',
          'not_supported_widgets' => array(),
          'widget' => 'number',
        ),
      ),
      'unit' => array(
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
          'target_bundles' => array(
            $unit_type => $unit_type,
          ),
        ),
        'target_type' => 'units_unit',
      ),
      'value' => array(
        'decimal_separator' => '.',
      ),
    ),
    'translatable' => 0,
    'type' => 'mvf_number_float_entityreference',
  );

  return $fields;
}

/**
 * Defines the default field instances for the ERPAL output entity type.
 *
 * @param string $type
 *   output type to get instances for.
 *
 * @return array
 *   Field instances.
 */
function _erpal_output_type_default_field_instances($type, $unit_type) {
  $instances = array();

  // Delivery date field.
  $instances[] = array(
    'bundle' => $type,
    'display' => array(
      'default' => array(
        'label' => 'above',
        'type' => 'date_default',
        'settings' => array(
          'format_type' => 'long',
          'show_repeat_rule' => 'show',
          'multiple_number' => '',
          'multiple_from' => '',
          'multiple_to' => '',
          'fromto' => 'both',
        ),
        'module' => 'date',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'erpal_output',
    'field_name' => 'field_output_delivered_date',
    'label' => t('Delivery date'),
    'required' => FALSE,
    'settings' => array(
      'default_value' => 'now',
      'default_value_code' => '',
      'default_value2' => 'blank',
      'default_value_code2' => '',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'weight' => '1',
      'type' => 'date_popup',
      'module' => 'date',
      'active' => TRUE,
      'settings' => array(
        'input_format' => 'm/d/Y - H:i:s',
        'input_format_custom' => '',
        'year_range' => '-5:+5',
        'increment' => '1',
        'label_position' => 'above',
        'text_parts' => array(),
        'repeat_collapsed' => 0,
      ),
    ),
  );

  // Delivered field.
  $instances[] = array(
    'bundle' => $type,
    'default_value' => array(
      0 => array(
        'value' => 0,
      ),
    ),
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 15,
      ),
    ),
    'entity_type' => 'erpal_output',
    'field_name' => 'field_output_delivered',
    'label' => 'Delivered',
    'required' => FALSE,
    'widget' => array(
      'active' => TRUE,
      'module' => 'options',
      'settings' => array(
        'display_label' => TRUE,
      ),
      'type' => 'options_onoff',
      'weight' => 2,
    ),
  );

  // Budget ref field.
  $instances[] = array(
    'bundle' => $type,
    'default_value' => NULL,
    'default_value_function' => 'entityreference_prepopulate_field_default_value',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'erpal_output',
        'settings' => array(
          'budget_title' => 1,
          'decimal_separator' => '.',
          'link' => 1,
          'prefix_suffix' => 1,
          'scale' => 2,
          'separator' => ' ',
          'thousand_separator' => ' ',
        ),
        'type' => 'erpal_output_budget_default',
        'weight' => 16,
      ),
    ),
    'entity_type' => 'erpal_output',
    'field_name' => 'field_output_budget',
    'label' => 'Budget',
    'required' => 0,
    'settings' => array(
      'behaviors' => array(
        'prepopulate' => array(
          'action' => 'hide',
          'action_on_edit' => 1,
          'fallback' => 'hide',
          'providers' => array(
            'og_context' => FALSE,
            'url' => 1,
          ),
          'skip_perm' => 0,
          'status' => 1,
        ),
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'erpal_output',
      'settings' => array(),
      'type' => 'erpal_output_budget_field',
      'weight' => 3,
    ),
  );

  // Unit field 'field_output_unit'.
  $unit = units_unit_load_multiple(FALSE, array('measure' => $unit_type));
  $unit = reset($unit);
  $instances[] = array(
    'bundle' => $type,
    'default_value' => array(
      0 => array(
        'value' => '',
        'target_id' => $unit->umid,
        'default_value_widget' => TRUE,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'mvf',
        'settings' => array(
          'mvf' => array(
            'unit_suggesters_settings' => array(),
          ),
          'unit' => array(),
          'value' => array(),
        ),
        'type' => 'mvf_formatter_default',
        'weight' => 17,
      ),
    ),
    'entity_type' => 'erpal_output',
    'field_name' => 'field_output_unit',
    'label' => 'Quantity',
    'required' => 1,
    'settings' => array(
      'dfv' => array(
        'parent' => '',
        'status' => 0,
        'view' => '',
      ),
      'mvf' => array(
        'unit_suggesters_settings' => array(),
        'max' => array(
          'target_id' => '_none',
          'value' => '',
        ),
        'min' => array(
          'target_id' => '_none',
          'value' => '',
        ),
      ),
      'unit' => array(
        'target_bundles' => $unit_type,
      ),
      'user_register_form' => FALSE,
      'value' => array(
        'max' => '',
        'min' => '',
        'prefix' => '',
        'suffix' => '',
      ),
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'mvf',
      'settings' => array(
        'meta_info' => array(
          'unit' => array(
            'label_property' => 'label',
            'weight' => 1,
          ),
          'value' => array(
            'weight' => 0,
          ),
        ),
      ),
      'type' => 'mvf_widget_default',
      'weight' => 4,
    ),
  );

  $instances[] = array(
    'bundle' => $type,
    'default_value' => NULL,
    'default_value_function' => 'entityreference_prepopulate_field_default_value',
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
    ),
    'ds_extras_field_template' => '',
    'entity_type' => 'erpal_output',
    'field_name' => 'field_resource',
    'label' => 'Resource',
    'required' => 0,
    'settings' => array(
      'behaviors' => array(
        'prepopulate' => array(
          'action' => 'hide',
          'action_on_edit' => 1,
          'fallback' => 'hide',
          'providers' => array(
            'og_context' => FALSE,
            'url' => 1,
          ),
          'skip_perm' => 0,
          'status' => 1,
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
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 3,
    ),
  );

  return $instances;
}
