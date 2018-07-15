<?php

/**
 * @file
 * User page callbacks for the output UI module.
 */

/**
 * Create new ERPAL output entity and pass it to entity_ui_get_form().
 *
 * @param string $type
 *   output type.
 */
function erpal_output_entity_ui_get_form_wrapper($type) {
  $values = array('type' => $type);
  $output = entity_create('erpal_output', $values);
  return entity_ui_get_form('erpal_output', $output, 'add');
}

/**
 * Form builder for ERPAL output forms.
 */
function erpal_output_form($form, &$form_state, $output) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files'][] = drupal_get_path('module', 'erpal_output_ui') . '/erpal_output_ui.pages.inc';

  $form['title'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Title'),
    '#default_value' => $output->title,
  );

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $output->uid,
  );

  field_attach_form('erpal_output', $output, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 40,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save output'),
    '#submit' => $submit + array('erpal_output_form_submit'),
  );

  // Show delete button if entity exists and user has appropriate permission.
  if ($form_state['op'] == 'edit' && entity_access('delete', 'erpal_output', $output)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('erpal_output_form_submit_delete'),
    );
  }

  // Set title.
  if ($form_state['op'] == 'add') {
    $erpal_output_type = erpal_output_types($output->type);
    drupal_set_title(t('Add new output @label', array('@label' => $erpal_output_type->label)));
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'erpal_output_form_validate';

  return $form;
}

/**
 * Validation callback for erpal_output_form.
 */
function erpal_output_form_validate($form, &$form_state) {
  $output = $form_state['erpal_output'];

  // Notify field widgets to validate their data.
  field_attach_form_validate('erpal_output', $output, $form, $form_state);
}

/**
 * Submit callback for erpal_output_form.
 */
function erpal_output_form_submit($form, &$form_state) {
  $output = $form_state['erpal_output'];

  $output->title = $form_state['values']['title'];
  $output->uid = $form_state['values']['uid'];

  if (!isset($output->created) || empty($output->created)) {
    $output->created = REQUEST_TIME;
  }

  $output->changed = REQUEST_TIME;

  // Notify field widgets.
  field_attach_submit('erpal_output', $output, $form, $form_state);

  erpal_output_save($output);

  // Show message to user.
  if ($form_state['op'] == 'add') {
    drupal_set_message(t('output %title created.', array('%title' => $output->title)));
  }
  elseif ($form_state['op'] == 'edit') {
    drupal_set_message(t('output %title edited.', array('%title' => $output->title)));
  }

  $form_state['redirect'] = 'erpal/output/' . $output->output_id;
}

/**
 * Submit handler for Delete button.
 *
 * Redirect to delete confirmation page.
 */
function erpal_output_form_submit_delete($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $output = $form_state['erpal_output'];
  $form_state['redirect'] = array(
    'erpal/output/' . $output->output_id . '/delete',
    array(
      'query' => $destination,
    ),
  );
}

/**
 * Delete output form confirmation.
 */
function erpal_output_form_delete_confirm($form, &$form_state, $output) {
  $form_state['output'] = $output;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['output_id'] = array(
    '#type' => 'value',
    '#value' => $output->output_id,
  );

  return confirm_form($form,
    t('Are you sure you want to delete output %title?', array('%title' => $output->title)),
    'erpal/output/' . $output->output_id,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Execute node deletion.
 */
function erpal_output_form_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $output = $form_state['output'];
    erpal_output_delete($output);

    watchdog('erpal_output', '@type: deleted %title.', array(
      '@type' => $output->type,
      '%title' => $output->title,
    ));
    drupal_set_message(t('@type %title has been deleted.', array(
      '@type' => erpal_output_type_get_name($output->type),
      '%title' => $output->title,
    )));
  }
  $destination = drupal_get_destination();
  $current_path = drupal_get_path_alias();
  if ($destination['destination'] == $current_path) {
    drupal_goto('<front>');
  }
  else {
    drupal_goto($destination);
  }
}

/**
 * Add output page callback.
 *
 * @return string
 *   output types list or empty text.
 */
function erpal_output_ui_add_output() {

  global $user;
  $items = array();

  // Work around core issue where 'title' isn't set for MENU_LOCAL_ACTION,
  // see: http://drupal.org/node/891892
  $trail = menu_get_active_trail();
  drupal_set_title($trail[count($trail) - 1]['title']);

  foreach (erpal_output_types() as $info) {
    if (entity_access('create', 'erpal_output', $info->type, $user)) {
      $query = drupal_get_query_parameters();
      if (!empty($query['field_output_budget']) && is_numeric($query['field_output_budget'])) {
        $budget = erpal_budget_load($query['field_output_budget']);
        if (!erpal_output_type_suitable_budget_type($info->type, $budget->type)) {
          continue;
        }
      }
      $items[] = array(
        'title' => $info->label,
        'href' => current_path() . '/' . $info->type,
        'localized_options' => array('query' => $query),
        'description' => $info->description,
      );
    }
  }

  if (count($items) == 1) {
    drupal_goto($items[0]['href'], $items[0]['localized_options']);
  }

  return empty($items) ? t('It is not possible to create output entities yet.
    Please create an output type first.') : theme('erpal_core_ui_add_list', array('content' => $items));
}
