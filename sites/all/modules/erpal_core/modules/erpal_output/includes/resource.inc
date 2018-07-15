<?php

/**
 * @file
 * Resource definitions for services module.
 */

/**
 * Determine whether the current user can access a erpal_output resource.
 *
 * @param string $op
 *   Operation, access for which should be checked.
 * @param mixed $arg
 *   output ID or output data array, in case $op is 'create'.
 *
 * @return bool
 *   Boolean.
 *
 * @see erpal_output_access()
 */
function _erpal_output_resource_access($op, $arg = NULL) {
  if (is_array($arg[0])) {
    $output_data = array_shift($arg);

    return erpal_output_access($op, $output_data['type']);
  }
  elseif (is_numeric($arg[0])) {
    $erpal_output = erpal_output_load($arg[0]);

    return erpal_output_access($op, $erpal_output);
  }
  elseif (!isset($arg)) {
    return (user_access('administer erpal_output entities') || user_access('view any erpal_output entity'));
  }

  return FALSE;
}

/**
 * Return an array of erpal_outputs.
 *
 * @param int $page
 *   Page number of results to return (in pages of 20).
 * @param int $page_size
 *   Integer number of items to be returned.
 *
 * @return array
 *   An array of node objects.
 */
function _erpal_output_resource_index($page, $page_size) {
  $aids = db_select('erpal_output', 'c')
    ->orderBy('created', 'DESC')
    ->fields('c', array('output_id'))
    ->range($page * $page_size, $page_size)
    ->execute()
    ->fetchCol();

  $outputs = erpal_output_load_multiple($aids);

  return services_resource_build_index_list($outputs, 'erpal_output', 'output_id');
}

/**
 * Callback for the 'create' method.
 */
function _erpal_output_resource_create($output_data) {
  try {
    $output = entity_create('erpal_output', $output_data);
    unset($output->is_new);
    erpal_output_save($output);

    return $output;
  }
  catch (Exception $exception) {
    watchdog_exception('erpal_output', $exception);
    return services_error($exception, 406, $output_data);
  }
}

/**
 * Callback for the 'update' method.
 */
function _erpal_output_resource_update($output_id, $output_data) {
  return _erpal_output_resource_create($output_data);
}

/**
 * Callback for the 'delete' method.
 */
function _erpal_output_resource_delete($output_id) {
  return erpal_output_delete_multiple(array($output_id));
}
