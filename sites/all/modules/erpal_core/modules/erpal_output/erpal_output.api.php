<?php /*457563643457563643*/ ?><?php  ?><?php  ?><?php

/**
 * @file
 * Hooks provided by the ERPAL Output module.
 */

/**
 * Implements hook_output_resource_find().
 *
 * Attaches resource to output.
 */
function HOOK_output_resource_find($output) {
  $output_wrapper = entity_metadata_wrapper('erpal_output', $output);
  $resource_id = $output_wrapper->field_resource->raw();
  if (empty($resource_id) && !empty($output_wrapper->field_output_budget[0])) {

    $budget_wrapper = $output_wrapper->field_output_budget[0]->budget;
    $output_wrapper->field_resource = $budget_wrapper->field_resource->raw();
  }
}

/**
 * Implements hook_output_budget_find().
 *
 * Finds budgets what will be added to output.
 *
 * @param object $output
 *   ERPAL Output object.
 * @return array
 *   An array of Budgets objects or ids what should be added to output.
 */
function HOOK_output_budget_find($output) {
  $budget_fields = field_get_items('erpal_output', $output, 'field_output_budget');
  return $budget_fields ? $budget_fields : array();
}

/**
 * Implements hook_output_budget_find_alter().
 *
 * Alter budgets array from output_budget_find hook.
 */
function HOOK_output_budget_find_alter(&$budgets, $output) {

}
