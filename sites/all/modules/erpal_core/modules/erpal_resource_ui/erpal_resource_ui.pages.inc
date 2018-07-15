<?php

/**
 * @file
 * User page callbacks for the resource UI module.
 */

/**
 * Create new ERPAL resource entity and pass it to entity_ui_get_form().
 *
 * @param string $type
 *   resource type.
 */
function erpal_resource_entity_ui_get_form_wrapper($type) {
  $values = array('type' => $type);
  $resource = entity_create('erpal_resource', $values);

  return entity_ui_get_form('erpal_resource', $resource, 'add');
}

/**
 * Form builder for ERPAL resource forms.
 */
function erpal_resource_form($form, &$form_state, $resource) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files'][] = drupal_get_path('module', 'erpal_resource_ui') . '/erpal_resource_ui.pages.inc';

  $form['title'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Title'),
    '#default_value' => $resource->title,
  );

  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $resource->uid,
  );

  field_attach_form('erpal_resource', $resource, $form, $form_state);

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
    '#value' => t('Save resource'),
    '#submit' => $submit + array('erpal_resource_form_submit'),
  );

  // Show delete button if entity exists and user has appropriate permission.
  if ($form_state['op'] == 'edit' && entity_access('delete', 'erpal_resource', $resource)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('erpal_resource_form_submit_delete'),
    );
  }

  // Set title.
  if ($form_state['op'] == 'add') {
    $erpal_resource_type = erpal_resource_types($resource->type);
    drupal_set_title(t('Add new resource @label', array('@label' => $erpal_resource_type->label)));
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'erpal_resource_form_validate';

  return $form;
}

/**
 * Validation callback for erpal_resource_form.
 */
function erpal_resource_form_validate($form, &$form_state) {
  $resource = $form_state['erpal_resource'];

  // Notify field widgets to validate their data.
  field_attach_form_validate('erpal_resource', $resource, $form, $form_state);
}

/**
 * Submit callback for crm_resource_form.
 */
function erpal_resource_form_submit($form, &$form_state) {
  $resource = $form_state['erpal_resource'];

  $resource->title = $form_state['values']['title'];
  $resource->uid = $form_state['values']['uid'];

  if (!isset($resource->created) || empty($resource->created)) {
    $resource->created = REQUEST_TIME;
  }

  $resource->changed = REQUEST_TIME;

  // Notify field widgets.
  field_attach_submit('erpal_resource', $resource, $form, $form_state);

  erpal_resource_save($resource);

  // Show message to user.
  if ($form_state['op'] == 'add') {
    drupal_set_message(t('resource %title created.', array('%title' => $resource->title)));
  }
  elseif ($form_state['op'] == 'edit') {
    drupal_set_message(t('resource %title edited.', array('%title' => $resource->title)));
  }

  $form_state['redirect'] = 'erpal/resource/' . $resource->resource_id;
}

/**
 * Submit handler for Delete button.
 *
 * Redirect to delete confirmation page.
 */
function erpal_resource_form_submit_delete($form, &$form_state) {
  $destination = array();
  if (isset($_GET['destination'])) {
    $destination = drupal_get_destination();
    unset($_GET['destination']);
  }
  $resource = $form_state['erpal_resource'];
  $form_state['redirect'] = array(
    'erpal/resource/' . $output->resource_id . '/delete',
    array(
      'query' => $destination,
    ),
  );
}

/**
 * Delete resource form confirmation.
 */
function erpal_resource_form_delete_confirm($form, &$form_state, $resource) {
  $form_state['resource'] = $resource;
  // Always provide entity id in the same form key as in the entity edit form.
  $form['resource_id'] = array(
    '#type' => 'value',
    '#value' => $resource->resource_id,
  );

  return confirm_form($form,
    t('Are you sure you want to delete resource %title?', array('%title' => $resource->title)),
    'erpal/resource/' . $resource->resource_id,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Execute node deletion.
 */
function erpal_resource_form_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $resource = $form_state['resource'];
    erpal_resource_delete($resource);

    watchdog('erpal_resource', '@type: deleted %title.', array(
      '@type' => $resource->type,
      '%title' => $resource->title,
    ));
    drupal_set_message(t('@type %title has been deleted.', array(
      '@type' => erpal_resource_type_get_name($resource->type),
      '%title' => $resource->title,
    )));
  }
  $destination = drupal_get_destination();
  $current_path = drupal_get_path_alias();
  if ($destination['destination'] == $current_path) {
    drupal_goto('erpal');
  }
  else {
    drupal_goto($destination);
  }
}

/**
 * Add resource page callback.
 *
 * @return string
 *   resource types list or empty text.
 */
function erpal_resource_ui_add_resource() {

  global $user;
  $items = array();

  // Work around core issue where 'title' isn't set for MENU_LOCAL_ACTION,
  // see: http://drupal.org/node/891892
  $trail = menu_get_active_trail();
  drupal_set_title($trail[count($trail) - 1]['title']);

  foreach (erpal_resource_types() as $info) {
    if (entity_access('create', 'erpal_resource', $info->type, $user)) {
      $items[] = array(
        'title' => $info->label,
        'href' => 'admin/erpal/add-content/resource/add/' . $info->type,
        'localized_options' => array('query' => drupal_get_query_parameters()),
        'description' => $info->description,
      );
    }
  }

  if (count($items) == 1) {
    drupal_goto($items[0]['href'], $items[0]);
  }

  return empty($items) ? t('It is not possible to create resource entities yet.
   Please create an resource type first.') : theme('erpal_core_ui_add_list', array('content' => $items));
}

/**
 * Form builder for ERPAL resource entities.
 */
function erpal_resource_settings_form($form, &$form_state) {
  drupal_set_message(t('After the changes you need to clear the cache.'));
  $form['erpal_resource_bundles'] = array(
    '#type' => 'select',
    '#title' => t('Bundles'),
    '#options' => array(),
    '#multiple' => TRUE,
    '#default_value' => variable_get('erpal_resource_bundles', array()),
    '#description' => t('Please select Bundles of entities for which need resource.'),
  );
  foreach (entity_get_info() as $entity_name => $entity_info) {

    // Skip entities to which shouldn't add resource.
    $skip = array(
      'erpal_resource',
      'erpal_resource_type',
      'erpal_budget',
      'erpal_budget_type',
      'erpal_output',
      'erpal_output_type',
    );
    if (!in_array($entity_name, $skip)) {
      if (!empty($entity_info['bundles'])) {
        foreach ($entity_info['bundles'] as $entity_bundle_name => $entity_bundle_info) {
          $form['erpal_resource_bundles']['#options'][$entity_info['label']][$entity_name . ERPAL_RESOURCE_DELIMITER . $entity_bundle_name] = $entity_bundle_info['label'];
        }
      }
    }
  }
  $form['erpal_resource_bundles']['#size'] = count($form['erpal_resource_bundles']['#options'], COUNT_RECURSIVE);

  return system_settings_form($form);
}
