<?php

/**
 * @file
 * Adds fields to the budget entity type used by ERPAL Core.
 */

/**
 * Define the default fields for the ERPAL budget entity type.
 *
 * @return array
 *   Return default fields to be attached to newly created budget type.
 *
 * @see field_create_field()
 */
function _erpal_budget_type_default_fields($type, $unit_type) {
  $fields = array();

  // Unit field 'field_budget_total'
  $fields[] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_budget_total',
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

  // Unit field 'field_budget_available'
  $fields[] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_budget_available',
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

  // Negative field.
  $fields[] = array(
    'active' => TRUE,
    'cardinality' => 1,
    'entity_types' => array('erpal_budget'),
    'field_name' => 'field_budget_negative',
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
        0 => 'No',
        1 => 'Yes',
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => FALSE,
    'type' => 'list_boolean',
  );

  $types = array_keys(erpal_resource_types());
  // Exported field_base: 'field_resource'
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
 * Defines the default field instances for the ERPAL budget entity type.
 *
 * @param string $type
 *   budget type to get instances for.
 *
 * @return array
 *   Field instances.
 */
function _erpal_budget_type_default_field_instances($type, $unit_type) {
  $instances = array();

  if ($unit_type == 'money') {
    $default_currency = variable_get('erpal_currency_settings', 'EUR');
    $efq = new EntityFieldQuery();
    $result = $efq->entityCondition('entity_type', 'units_unit')
      ->entityCondition('bundle', 'money')
      ->propertyCondition('symbol', $default_currency)
      ->execute();
    $units = $result['units_unit'];
  }
  if (empty($units)) {
    $units = units_unit_load_multiple(FALSE, array('measure' => $unit_type));
  }
  $unit_id = reset($units)->umid;

  // Unit field instance 'field_budget_total'
  $instances[] = array(
    'bundle' => $type,
    'default_value' => array(
      0 => array(
        'value' => '',
        'target_id' => $unit_id,
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
        'weight' => 1,
      ),
      'full' => array(
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
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'mvf',
        'settings' => array(
          'mvf' => array(
            'unit_suggesters_settings' => array(),
          ),
          'unit' => array(),
          'value' => array(),
        ),
        'type' => 'mvf_formatter_default',
        'weight' => 4,
      ),
    ),
    'entity_type' => 'erpal_budget',
    'field_name' => 'field_budget_total',
    'label' => 'Total',
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
      'weight' => 0,
    ),
  );

  // Unit field instance 'field_budget_available'
  $instances[] = array(
    'bundle' => $type,
    'default_value' => array(
      0 => array(
        'value' => '',
        'target_id' => $unit_id,
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
        'weight' => 0,
      ),
      'full' => array(
        'label' => 'hidden',
        'module' => 'erpal_budget',
        'settings' => array(
          'mvf' => array(
            'unit_suggesters_settings' => array(),
          ),
          'total_field' => 'field_budget_total',
          'unit' => array(),
          'value' => array(),
        ),
        'type' => 'erpal_budget_formatter_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'mvf',
        'settings' => array(
          'mvf' => array(
            'unit_suggesters_settings' => array(),
          ),
          'unit' => array(),
          'value' => array(),
        ),
        'type' => 'mvf_formatter_default',
        'weight' => 3,
      ),
    ),
    'entity_type' => 'erpal_budget',
    'field_name' => 'field_budget_available',
    'label' => 'Available',
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
      'weight' => 0,
    ),
  );

  // Negative field.
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
      'full' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 5,
      ),
    ),
    'entity_type' => 'erpal_budget',
    'field_name' => 'field_budget_negative',
    'label' => 'Allow exceeding',
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

  $instances[] = array(
    'bundle' => $type,
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
        'module' => 'entityreference',
        'settings' => array(
          'link' => 1,
        ),
        'type' => 'entityreference_label',
        'weight' => 16,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
      ),
    ),
    'entity_type' => 'erpal_budget',
    'field_name' => 'field_resource',
    'label' => 'Resource',
    'required' => 0,
    'settings' => array(
      'behaviors' => array(
        'prepopulate' => array(
          'action' => 'none',
          'action_on_edit' => 0,
          'fallback' => 'none',
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

  // Translatables
  // Included for use with string extractors like potx.
  t('Available');
  t('Total');

  return $instances;
}
