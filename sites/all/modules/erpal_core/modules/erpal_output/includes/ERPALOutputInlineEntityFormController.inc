<?php

/**
 * @file
 * Defines the inline entity form controller for ERPAL Output Entities.
 */

class ERPALOutputInlineEntityFormController extends EntityInlineEntityFormController {

  /**
   * Overrides EntityInlineEntityFormController::defaultLabels().
   */
  public function defaultLabels() {
    $labels = array(
      'singular' => t('output'),
      'plural' => t('outputs'),
    );
    return $labels;
  }

  /**
   * Overrides EntityInlineEntityFormController::entityForm().
   */
  public function entityForm($entity_form, &$form_state) {
    $output = $entity_form['#entity'];
    $type = $output->type;
    $extra_fields = field_info_extra_fields('erpal_output', $output->type, 'form');

    // Do some prep work on the erpal_output, similarly to node_form().
    if (!isset($output->title)) {
      $output->title = NULL;
    }

    $entity_form['title'] = array(
      '#type' => 'textfield',
      '#title' => check_plain($type),
      '#required' => TRUE,
      '#default_value' => $output->title,
      '#maxlength' => 255,
      // The label might be missing if the Title module has replaced it.
      '#weight' => !empty($extra_fields['title']) ? $extra_fields['title']['weight'] : -5,
    );

    $langcode = entity_language('erpal_output', $output);
    field_attach_form('erpal_output', $output, $entity_form, $form_state, $langcode);

    return $entity_form;
  }
}
