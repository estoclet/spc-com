<?php

/**
 * @file
 * User page callbacks for the budget UI module.
 */

/**
 * Create new ERPAL budget entity and pass it to entity_ui_get_form().
 *
 * @param string $type
 *   budget type
 */
function erpal_budget_entity_ui_get_form_wrapper($type) {
  $values = array('type' => $type);
  $budget = entity_create('erpal_budget', $values);

  return entity_ui_get_form('erpal_budget', $budget, 'add');
}

/**
 * Form builder for ERPAL budget forms.
 */
function erpal_budget_form($form, &$form_state, $budget) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files'][] = drupal_get_path('module', 'erpal_budget_ui') . '/erpal_budget_ui.pages.inc';

  $form['title'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Title'),
    '#default_value' => $budget->title,
  );
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $budget->uid,
  );

  field_attach_form('erpal_budget', $budget, $form, $form_state);

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
    '#value' => t('Save budget'),
    '#submit' => $submit + array('erpal_budget_form_submit'),
  );

  // Show delete button if entity exists and user has appropriate permission.
  if ($form_state['op'] == 'edit' && entity_access('delete', 'erpal_budget', $budget)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('erpal_budget_form_submit_delete'),
    );
  }

  // Set title.
  if ($form_state['op'] == 'add') {
    $erpal_budget_type = erpal_budget_types($budget->type);
    drupal_set_title(t('Add new budget @label', array('@label' => $erpal_budget_type->label)));
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'erpal_budget_form_validate';

  return $form;
}

/**
 * Validation callback for erpal_budget_form.
 */
function erpal_budget_form_validate($form, &$form_state) {
  $budget = $form_state['erpal_budget'];

  // Notify field widgets to validate their data.
  field_attach_form_validate('erpal_budget', $budget, $form, $form_state);
}

/**
 * Submit callback for crm_budget_form.
 */
function erpal_budget_form_submit($form, &$form_state) {
  $budget = $form_state['erpal_budget'];

  $budget->title = $form_state['values']['title'];
  $budget->uid = $form_state['values']['uid'];

  if (!isset($budget->created) || empty($budget->created)) {
    $budget->created = REQUEST_TIME;
  }

  $budget->changed = REQUEST_TIME;

  // Notify field widgets.
  field_attach_submit('erpal_budget', $budget, $form, $form_state);

  erpal_budget_save($budget);

  // Show message to user.
  if ($form_state['op'] == 'add') {
    drupal_set_message(t('budget %title created.', array('%title' => $budget->title)));
  }
  elseif ($form_state['op'] == 'edit') {
    drupal_set_message(t('budget %title edited.', array('%title' => $budget->title)));
  }

  $form_state['redirect'] = 'erpal/budget/' . $budget->budget_id;
}

/**
 * Submit handler for Delete button.
 *
 * Redirect to delete confirmation page.
 */
function erpal_budget_form_submit_delete($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $budget = $form_state['erpal_budget'];
  $form_state['redirect'] = array(
    'erpal/budget/' . $budget->budget_id . '/delete',
    array(
      'query' => $destination,
    ),
  );
}

/**
 * Delete budget form confirmation.
 */
function erpal_budget_form_delete_confirm($form, &$form_state, $budget) {
  $form_state['budget'] = $budget;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['budget_id'] = array(
    '#type' => 'value',
    '#value' => $budget->budget_id,
  );

  return confirm_form($form,
    t('Are you sure you want to delete budget %title?', array('%title' => $budget->title)),
    'erpal/budget/' . $budget->budget_id,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Execute node deletion.
 */
function erpal_budget_form_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $budget = $form_state['budget'];
    erpal_budget_delete($budget);

    watchdog('erpal_budget', '@type: deleted %title.', array(
      '@type' => $budget->type,
      '%title' => $budget->title,
    ));
    drupal_set_message(t('@type %title has been deleted.', array(
      '@type' => erpal_budget_type_get_name($budget->type),
      '%title' => $budget->title,
    )));
  }
  $destination = drupal_get_destination();
  $current_path = drupal_get_path_alias();
  if ($destination['destination'] == $current_path) {
    drupal_goto('home');
  }
  else {
    drupal_goto($destination);
  }
}

/**
 * Add budget page callback.
 *
 * @return string
 *   budget types list or empty text.
 */
function erpal_budget_ui_add_budget() {

  global $user;
  $items = array();

  // Work around core issue where 'title' isn't set for MENU_LOCAL_ACTION,
  // see: http://drupal.org/node/891892
  $trail = menu_get_active_trail();
  drupal_set_title($trail[count($trail) - 1]['title']);

  foreach (erpal_budget_types() as $info) {
    if (entity_access('create', 'erpal_budget', $info->type, $user)) {
      $items[] = array(
        'title' => $info->label,
        'href' => current_path() . '/' . $info->type,
        'localized_options' => array('query' => drupal_get_query_parameters()),
        'description' => $info->description,
      );
    }
  }

  if (count($items) == 1) {
    drupal_goto($items[0]['href'], $items[0]['localized_options']);
  }

  return empty($items) ? t('It is not possible to create budget entities yet. Please create an budget type first.') : theme('erpal_core_ui_add_list', array('content' => $items));
}
