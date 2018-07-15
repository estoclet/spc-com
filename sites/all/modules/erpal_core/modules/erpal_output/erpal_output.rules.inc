<?php

/**
 * @file
 * Provides default rules actions.
 */

/**
 * Implements hook_rules_action_info().
 */
function erpal_output_rules_action_info() {

  return array(
    'erpal_output_attach_budgets' => array(
      'label' => t('Find and attach budgets to output'),
      'group' => t('ERPAL Output'),
      'parameter' => array(
        'erpal_output' => array(
          'type' => 'erpal_output',
          'label' => t('ERPAL Output'),
        ),
        'budgets' => array(
          'type' => 'list<integer>',
          'label' => t('ERPAL Budgets'),
          'optional' => TRUE,
        ),
      ),
    ),
    'erpal_output_clear_budget_field' => array(
      'label' => t('Clear budget field'),
      'group' => t('ERPAL Output'),
      'parameter' => array(
        'erpal_output' => array(
          'type' => 'erpal_output',
          'label' => t('ERPAL Output'),
        ),
      ),
    ),
    'erpal_output_detach_budgets' => array(
      'label' => t('Detach budgets from output'),
      'group' => t('ERPAL Output'),
      'parameter' => array(
        'erpal_output' => array(
          'type' => 'erpal_output',
          'label' => t('ERPAL Output'),
        ),
      ),
    ),
    'erpal_output_fetch_field_budgets' => array(
      'label' => t('Fetch assigned Output budgets'),
      'group' => t('ERPAL Output'),
      'parameter' => array(
        'erpal_output' => array(
          'type' => 'erpal_output',
          'label' => t('ERPAL Output'),
        ),
      ),
      'provides' => array(
        'fetched_budgets' => array(
          'type' => 'list<integer>',
          'name' => t('Fetched budgets'),
          'label' => t('Fetched budgets'),
        ),
      ),
    ),
    'erpal_output_fetch_budgets_from_hook' => array(
      'label' => t('Fetch Output budgets from hook'),
      'group' => t('ERPAL Output'),
      'parameter' => array(
        'erpal_output' => array(
          'type' => 'erpal_output',
          'label' => t('ERPAL Output'),
        ),
      ),
      'provides' => array(
        'fetched_budgets' => array(
          'type' => 'list<integer>',
          'name' => t('Fetched budgets'),
          'label' => t('Fetched budgets'),
        ),
      ),
    ),
    'erpal_output_find_and_attach_resource' => array(
      'label' => t('Find and attach resource to Output'),
      'group' => t('ERPAL Output'),
      'parameter' => array(
        'erpal_output' => array(
          'type' => 'erpal_output',
          'label' => t('ERPAL Output'),
        ),
      ),
    ),
  );
}

/**
 * Try to find budget and check if budget enough for current output.
 *
 * @param $output
 *   ERPAL Output entity.
 * @param array $budgets
 *   An array consists of budget's objects, ids or target_id(entityreference field format).
 */
function erpal_output_attach_budgets($output, $budgets = array()) {
  $output_wrapper = entity_metadata_wrapper('erpal_output', $output);

  foreach ($budgets as $budget) {
    if ($output_wrapper->saldo->raw() <= 0) {
      return;
    }

    if (is_array($budget) && !empty($budget['target_id'])) {
      $budget = intval($budget['target_id']);
    }

    if(!(is_object($budget) || is_integer($budget))) {
      return;
    }

    // Don't calculate If budget already assigned to output.
    if (_erpal_output_budget_field_get_key($output, $budget) !== FALSE) {
      return;
    }

    // Calculate budget value and add it to output.
    _erpal_output_calculate_budget_field_value($output, $budget);
  }
}

/**
 * Clear output_budget field.
 *
 * @param $output
 *   ERPAL Output entity.
 */
function erpal_output_clear_budget_field($output) {
  $output->field_output_budget = NULL;
}

/**
 * Get budget key from field_output_budget field.
 */
function _erpal_output_budget_field_get_key($output, $budget) {
  $output_wrapper = entity_metadata_wrapper('erpal_output', $output);
  $budget_id = is_object($budget) ? $budget->budget_id : $budget;
  $key = FALSE;

  $budgets = $output_wrapper->field_output_budget->value();
  if (!empty($budgets)) {
    $budgets = !empty($budgets[0]) ? $output_wrapper->field_output_budget : array($output_wrapper->field_output_budget);
    foreach ($budgets as $delta => $output_budget) {

      if ($output_budget->budget->raw() == $budget_id) {
        $key = $delta;
        break;
      }
    }
  }
  return $key;
}

/**
 * Calculate budget value and add it to output.
 *
 * @param $output
 *   ERPAL Output entity.
 * @param array $budget
 *   ERPAL Budget object or ID.
 */
function _erpal_output_calculate_budget_field_value($output, $budget) {
  $output_wrapper = entity_metadata_wrapper('erpal_output', $output);
  $budget_wrapper = entity_metadata_wrapper('erpal_budget', $budget);

  $output_saldo = $output_wrapper->saldo->raw();
  $budget_available_value = $budget_wrapper->field_budget_available->value->value();
  $can_be_negative = $budget_wrapper->field_budget_negative->raw();

  // Convert to output's measure type. @todo check additionaly budget and output unit types
  $budget_available_value = units_convert($budget_available_value,
    $budget_wrapper->field_budget_available->unit->value()->machine_name,
    $output_wrapper->field_output_unit->unit->value()->machine_name);

  if ($budget_available_value <= 0 && !$can_be_negative) {
    return;
  }

  // Calculate new values.
  $new_available_budget = $budget_available_value - $output_saldo;
  $output_budget_value = $output_saldo;

  if ($new_available_budget < 0 && !$can_be_negative) {
    $new_available_budget = 0;
    $output_budget_value = $budget_available_value;
  }

  $field = field_info_field('field_output_budget');
  if ($field['cardinality'] == 1) {
    // Add calculated values.
    $output_wrapper->field_output_budget = array(
      'target_id' => $budget_wrapper->budget_id->raw(),
      'value' => $output_budget_value,
    );
  }
  else {
    // Add calculated values.
    $output_wrapper->field_output_budget[] = array(
      'target_id' => $budget_wrapper->budget_id->raw(),
      'value' => $output_budget_value,
    );
  }

  // Convert to budget's measure type back.
  $new_available_budget = units_convert($new_available_budget,
    $output_wrapper->field_output_unit->unit->value()->machine_name,
    $budget_wrapper->field_budget_available->unit->value()->machine_name);

  // Update budget available field.
  $budget_wrapper->field_budget_available->value = $new_available_budget;
  $budget_wrapper->save();
}

/**
 * Removes attached to output budgets and recalculate their available values.
 */
function erpal_output_detach_budgets($output) {
  $output_wrapper = entity_metadata_wrapper('erpal_output', $output);

  $budgets = $output_wrapper->field_output_budget->value();
  if (!empty($budgets)) {
    $budgets = !empty($budgets[0]) ? $output_wrapper->field_output_budget : array($output_wrapper->field_output_budget);
    foreach ($budgets as $budget_field) {
      $output_value = $budget_field->value->value();

      if (isset($output_value) && $output_value != 0) {
        $budget_wrapper = $budget_field->budget;

        // Convert to budget's measure type back.
        $output_value = units_convert($output_value,
          $output_wrapper->field_output_unit->unit->value()->machine_name,
          $budget_wrapper->field_budget_available->unit->value()->machine_name);

        // Return output value to budget.
        $budget_available_value = $budget_wrapper->field_budget_available->value->value();
        $budget_wrapper->field_budget_available->value = $budget_available_value + $output_value;
        $budget_wrapper->save();
      }
    }
  }
}

/**
 * Action: Fetch budgets from output's field_output_budget field.
 */
function erpal_output_fetch_field_budgets($output) {
  $budget_fields = field_get_items('erpal_output', $output, 'field_output_budget');
  return array('fetched_budgets' => $budget_fields);
}

/**
 * Action: Fetch budgets from output_budget_find hook.
 */
function erpal_output_fetch_budgets_from_hook($output) {
  $budgets = module_invoke_all('output_budget_find', $output);
  if (!is_array($budgets)) {
    $budgets = array();
  }
  drupal_alter('output_budget_find', $budgets, $output);
  return array('fetched_budgets' => $budgets);
}

/**
 * Action: Try to find and attach resource if empty.
 */
function erpal_output_find_and_attach_resource($output) {
  $output_wrapper = entity_metadata_wrapper('erpal_output', $output);
  $resource_id = $output_wrapper->field_resource->raw();
  if (empty($resource_id)) {
    module_invoke_all('output_resource_find', $output);
  }
}
