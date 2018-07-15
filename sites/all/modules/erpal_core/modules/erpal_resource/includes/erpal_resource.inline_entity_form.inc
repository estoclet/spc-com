<?php

/**
 * @file
 * Defines the inline entity form controller for ERPAL Resource Entities.
 */

class ERPALResourceInlineEntityFormController extends EntityInlineEntityFormController {

  /**
   * Overrides EntityInlineEntityFormController::defaultLabels().
   */
  public function defaultLabels() {
    $labels = array(
      'singular' => t('resource'),
      'plural' => t('resources'),
    );
    return $labels;
  }

  /**
   * Overrides EntityInlineEntityFormController::entityForm().
   */
  public function entityForm($entity_form, &$form_state) {
    $resource = $entity_form['#entity'];
    $type = $resource->type;
    $extra_fields = field_info_extra_fields('erpal_resource', $resource->type, 'form');

    // Do some prep work on the erpal_resource, similarly to node_form().
    if (!isset($resource->title)) {
      $resource->title = NULL;
    }

    $entity_form['title'] = array(
      '#type' => 'textfield',
      '#title' => check_plain($type),
      '#required' => TRUE,
      '#default_value' => $resource->title,
      '#maxlength' => 255,
      // The label might be missing if the Title module has replaced it.
      '#weight' => !empty($extra_fields['title']) ? $extra_fields['title']['weight'] : -5,
    );

    $langcode = entity_language('erpal_resource', $resource);
    field_attach_form('erpal_resource', $resource, $entity_form, $form_state, $langcode);

    return $entity_form;
  }

}
