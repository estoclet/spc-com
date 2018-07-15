<?php

/**
 * @file
 * Alter ERPAL Budget and Output entity edit forms.
 */

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Adds custom submit function to edit entity.
 */
function _erpal_core_repeat_entity_form_alter(&$form, &$form_state) {
  form_load_include($form_state, 'inc', 'erpal_core_repeat', 'includes/erpal_core_repeat.pages');

  if ($form_state['entity_type'] == 'erpal_budget') {
    $form['actions']['submit']['#submit'][] = 'erpal_core_repeat_budget_form_submit';
  }
  elseif ($form_state['entity_type'] == 'erpal_output') {
    $form['actions']['submit']['#submit'][] = 'erpal_core_repeat_output_form_submit';
  }
}

/**
 * Submit callback for erpal_budget_form.
 *
 * Creates budget entities regarding repeatable date field.
 */
function erpal_core_repeat_budget_form_submit($form, &$form_state) {
  if (empty($form_state['erpal_repeat_dates'])) {
    return;
  }
  // Get repeatable dates.
  $repeat_dates = $form_state['erpal_repeat_dates'];

  // Remove already created budget entity.
  unset($repeat_dates[0]);

  $form_entity_type = $form_state['entity_type'];
  $entity = $form_state[$form_entity_type];
  $field_language = field_language($form_entity_type, $entity, ERPAL_CORE_REPEAT_DATE_FIELD);

  // Create a new budget for each date.
  foreach ($repeat_dates as $date_value) {

    // Clone entity.
    $clone_entity = _erpal_core_repeat_clone_entity($entity);
    $clone_entity->budget_id = NULL;

    // Replace date field to repeatable.
    $clone_entity->{ERPAL_CORE_REPEAT_DATE_FIELD}[$field_language][0] = $date_value;

    // Save the cloned entity.
    entity_save($form_entity_type, $clone_entity);
  }
}

/**
 * Submit callback for erpal_budget_form.
 *
 * Creates output entities regarding repeatable date field.
 */
function erpal_core_repeat_output_form_submit($form, &$form_state) {
  if (empty($form_state['erpal_repeat_dates'])) {
    return;
  }
  // Get repeatable dates.
  $repeat_dates = $form_state['erpal_repeat_dates'];

  // Remove already created entity.
  unset($repeat_dates[0]);

  $form_entity_type = $form_state['entity_type'];
  $entity = $form_state[$form_entity_type];
  $field_language = field_language($form_entity_type, $entity, ERPAL_CORE_REPEAT_DATE_FIELD);

  // Create a new entity for each date.
  foreach ($repeat_dates as $date_value) {

    // Clone entity.
    $clone_entity = _erpal_core_repeat_clone_entity($entity);
    $clone_entity->output_id = NULL;

    // Replace date field to repeatable.
    $clone_entity->{ERPAL_CORE_REPEAT_DATE_FIELD}[$field_language][0] = $date_value;

    // Find budget for current output.
    if (!empty($clone_entity->field_output_budget)) {

      $clone_entity->field_output_budget = NULL;
      _erpal_core_repeat_attach_budget($clone_entity);
    }

    // Save the cloned entity.
    entity_save($form_entity_type, $clone_entity);
  }
}

/**
 * Find and attach budget for given output.
 */
function _erpal_core_repeat_attach_budget(&$output) {
  $wrapper = entity_metadata_wrapper('erpal_output', $output);
  $resource_id = $wrapper->field_resource->raw();
  $unit_language = field_language('erpal_output', $output, 'field_output_budget');
  $unit_id = $output->field_output_unit[$unit_language][0]['target_id'];

  if (!empty($resource_id)) {
    // Prepare query.
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'erpal_budget')
      ->fieldCondition('field_resource', 'target_id', $resource_id)
      ->fieldCondition('field_budget_available', 'target_id', $unit_id);

    if (!empty($wrapper->field_erpal_core_repeat_date->value()['value'])) {
      $query->fieldCondition('field_erpal_core_repeat_date', 'value', $wrapper->field_erpal_core_repeat_date->value()['value'], '>=');
    }

    if (!empty($wrapper->field_erpal_core_repeat_date->value()['value2'])) {
      $query->fieldCondition('field_erpal_core_repeat_date', 'value2', $wrapper->field_erpal_core_repeat_date->value()['value2'], '<=');
    }

    $query->fieldCondition('field_budget_available', 'value', 0, '!=')->fieldOrderBy('field_budget_available', 'value');
    $budgets = $query->execute();

    // Use core function to find budgets.
    if (!empty($budgets)) {
      erpal_output_attach_budgets($output, array_keys($budgets['erpal_budget']));
    }
  }
}
