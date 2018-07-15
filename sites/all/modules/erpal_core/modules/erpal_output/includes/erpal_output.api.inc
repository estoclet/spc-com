<?php

/**
 * @file
 * Provides main functions for erpal_output entity.
 */

/**
 * Implements hook_entity_info().
 */
function _erpal_output_entity_info() {
  $return = array(
    'erpal_output' => array(
      'label' => t('ERPAL Output'),
      'entity class' => 'ERPALOutputEntity',
      'inline entity form' => array('controller' => 'ERPALOutputInlineEntityFormController'),
      'controller class' => 'ERPALOutputController',
      'base table' => 'erpal_output',
      'revision table' => 'erpal_output_revision',
      'fieldable' => TRUE,
      'entity keys' => array(
        'id' => 'output_id',
        'bundle' => 'type',
        'label' => 'title',
        'revision' => 'revision_id',
      ),
      'bundle keys' => array(
        'bundle' => 'type',
      ),
      'bundles' => array(),
      'load hook' => 'erpal_output_load',
      'view modes' => array(
        'full' => array(
          'label' => t('Full content'),
          'custom settings' => FALSE,
        ),
      ),
      'label callback' => 'entity_class_label',
      'uri callback' => 'entity_class_uri',
      'access callback' => 'erpal_output_access',
      'module' => 'erpal_output',
      'permission labels' => array(
        'singular' => t('output'),
        'plural' => t('output'),
      ),
    ),
  );

  if (module_exists('uuid')) {
    $return['erpal_output']['uuid'] = TRUE;
    $return['erpal_output']['entity keys']['uuid'] = 'uuid';
    $return['erpal_output']['entity keys']['revision uuid'] = 'vuuid';
  }

  return $return;
}

/**
 * Implements hook_entity_info_alter().
 *
 * Use this hook to specify output bundles to avoid a recursion, as loading
 * the output types needs the entity info too.
 */
function _erpal_output_entity_info_alter(&$entity_info) {
  foreach (erpal_output_types() as $type => $info) {
    $entity_info['erpal_output']['bundles'][$type] = array(
      'label' => $info->label,
      'admin' => array(
        'path' => ERPAL_OUTPUT_ADMIN_PATH . '/manage/%erpal_output_type',
        'real path' => ERPAL_OUTPUT_ADMIN_PATH . '/manage/' . $type,
        'bundle argument' => 4,
      ),
    );
  }
}

/**
 * Load a ERPAL output object.
 */
function erpal_output_load($output_id, $reset = FALSE) {
  if (empty($output_id)) {
    return array();
  }

  if ($output_id !== FALSE) {
    $output_id = array($output_id);
  }

  $outputs = erpal_output_load_multiple($output_id, array(), $reset);
  return reset($outputs);
}

/**
 * Load multiple outputs based on certain conditions.
 */
function erpal_output_load_multiple($output_ids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('erpal_output', $output_ids, $conditions, $reset);
}

/**
 * Save single output object to db. Wrapper for controller.
 *
 * @param object $output
 *   Entity object of erpal_output type.
 */
function erpal_output_save($output) {
  return entity_get_controller('erpal_output')->save($output);
}

/**
 * Delete single output.
 *
 * @todo This function should accept output_id, but output object.
 *
 * @param object $output
 *   Entity object of erpal_output type.
 */
function erpal_output_delete($output) {
  erpal_output_delete_multiple(array($output->output_id));
}

/**
 * Delete multiple outputs. Wrapper for controller.
 *
 * @param array $output_ids
 *   array of entity ids that should be deleted.
 */
function erpal_output_delete_multiple($output_ids) {
  entity_get_controller('erpal_output')->delete($output_ids);
}

/**
 * Output view callback.
 */
function erpal_output_view($output, $view_mode = 'full') {
  $langcode = $GLOBALS['language_content']->language;
  module_invoke_all('entity_view', $output, 'erpal_output', $view_mode, $langcode);
  $build = $output->view('full', $langcode);

  // Clone unit field and replace it's value and label to output Saldo field.
  $output_wrapper = entity_metadata_wrapper('erpal_output', $output);
  $saldo = $build['erpal_output'][$output->output_id]['field_output_unit'];
  $saldo[0]['value'][0]['#markup'] = number_format($output_wrapper->saldo->raw(), 2, '.', '');
  $saldo['#title'] = t('Saldo');
  $build['erpal_output'][$output->output_id]['saldo'] = $saldo;

  return $build;
}

/**
 * Implements hook_field_extra_fields().
 */
function erpal_output_field_extra_fields() {
  $extra = array();

  foreach (erpal_output_types() as $type) {
    $extra['erpal_output'][$type->type] = array(
      'form' => array(
        'title' => array(
          'label' => 'Title',
          'description' => t('ERPAL output module element'),
          'weight' => -5,
        ),
      ),
      'display' => array(
        'saldo' => array(
          'label' => t('Saldo'),
          'weight' => 18,
        ),
      ),
    );
  }

  return $extra;
}

/**
 * Implements hook_file_download_access().
 */
function erpal_output_file_download_access($file_item, $entity_type, $entity) {
  if ($entity_type == 'erpal_output') {
    return erpal_output_access('view', $entity);
  }
}

/**
 * Access callback for output.
 */
function erpal_output_access($op, $output, $account = NULL, $entity_type = NULL) {
  if (!isset($account)) {
    $account = $GLOBALS['user'];
  }
  if (is_object($output)) {
    $output_type = $output->type;
  }
  else {
    $output_type = $output;
  }

  if ($op != 'view') {
    // First grant access to the entity for the specified operation if no other
    // module denies it and at least one other module says to grant access.
    $access_results = module_invoke_all('erpal_entity_access', $op, $output, $account, $entity_type);

    if (in_array(FALSE, $access_results, TRUE)) {
      return FALSE;
    }
    elseif (in_array(TRUE, $access_results, TRUE)) {
      return TRUE;
    }
  }

  switch ($op) {
    case 'create':
      $access = (user_access('administer erpal_output entities', $account) ||
        user_access('create erpal_output entities', $account) ||
        user_access('create erpal_output entities of bundle ' . $output_type, $account));
      return $access;

    case 'view':
      $access = (user_access('administer erpal_output entities', $account) ||
        user_access('view any erpal_output entity', $account) ||
        user_access('view any erpal_output entity of bundle ' . $output_type, $account));
      return $access;

    case 'edit':
    case 'update':
      $access = (user_access('administer erpal_output entities', $account) ||
        user_access('edit any erpal_output entity', $account) ||
        user_access('edit any erpal_output entity of bundle ' . $output_type));
      return $access;

    case 'delete':
      $access = (user_access('administer erpal_output entities', $account) ||
        user_access('delete any erpal_output entity', $account) ||
        user_access('delete any erpal_output entity of bundle ' . $output_type));
      return $access;

    case 'create_view':
      // Check if user has access permission to a view with create output links.
      if (user_access('administer erpal_output entities', $account) ||
        user_access('create erpal_output entities', $account)) {
        return TRUE;
      }
      // Allow access if account has create permissions
      // for any ERPAL core output type.
      foreach (array_keys(erpal_output_types()) as $type) {
        if (entity_access('create', 'erpal_output', $type, $account)) {
          return TRUE;
        }
      }
      return FALSE;
  }
}
