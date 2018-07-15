<?php

/**
 * @file
 * Defines the inline entity form controller for ERPAL Budget Entities.
 */

class ERPALBudgetInlineEntityFormController extends EntityInlineEntityFormController {

  /**
   * Overrides EntityInlineEntityFormController::defaultLabels().
   */
  public function defaultLabels() {
    $labels = array(
      'singular' => t('budget'),
      'plural' => t('budgets'),
    );
    return $labels;
  }

  /**
   * Overrides EntityInlineEntityFormController::entityForm().
   */
  public function entityForm($entity_form, &$form_state) {
    $budget = $entity_form['#entity'];
    $type = $budget->type;
    $extra_fields = field_info_extra_fields('erpal_budget', $budget->type, 'form');

    // Do some prep work on the erpal_budget, similarly to node_form().
    if (!isset($budget->title)) {
      $budget->title = NULL;
    }

    $entity_form['title'] = array(
      '#type' => 'textfield',
      '#title' => check_plain($type),
      '#required' => TRUE,
      '#default_value' => $budget->title,
      '#maxlength' => 255,
      // The label might be missing if the Title module has replaced it.
      '#weight' => !empty($extra_fields['title']) ? $extra_fields['title']['weight'] : -5,
    );

    $langcode = entity_language('erpal_budget', $budget);
    field_attach_form('erpal_budget', $budget, $entity_form, $form_state, $langcode);

    return $entity_form;
  }

}
