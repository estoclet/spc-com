<?php

/**
 * @file
 * Resource definitions for services module.
 */

/**
 * Determine whether the current user can access a erpal_resource resource.
 *
 * @param string $op
 *   Operation, access for which should be checked.
 * @param mixed $arg
 *   resource ID or resource data array, in case $op is 'create'.
 *
 * @return bool
 *   Boolean.
 *
 * @see erpal_resource_access()
 */
function _erpal_resource_resource_access($op, $arg = NULL) {
  if (is_array($arg[0])) {
    $resource_data = array_shift($arg);

    return erpal_resource_access($op, $resource_data['type']);
  }
  elseif (is_numeric($arg[0])) {
    $erpal_resource = erpal_resource_load($arg[0]);

    return erpal_resource_access($op, $erpal_resource);
  }
  elseif (!isset($arg)) {
    return (user_access('administer erpal_resource entities') || user_access('view any erpal_resource entity'));
  }

  return FALSE;
}

/**
 * Return an array of erpal_resources.
 *
 * @param int $page
 *   Page number of results to return (in pages of 20).
 * @param int $page_size
 *   Integer number of items to be returned.
 *
 * @return array
 *   An array of node objects.
 */
function _erpal_resource_resource_index($page, $page_size) {
  $aids = db_select('erpal_resource', 'c')
    ->orderBy('created', 'DESC')
    ->fields('c', array('resource_id'))
    ->range($page * $page_size, $page_size)
    ->execute()
    ->fetchCol();

  $resources = erpal_resource_load_multiple($aids);

  return services_resource_build_index_list($resources, 'erpal_resource', 'resource_id');
}

/**
 * Callback for the 'create' method.
 */
function _erpal_resource_resource_create($resource_data) {
  try {
    $resource = entity_create('erpal_resource', $resource_data);
    unset($resource->is_new);
    erpal_resource_save($resource);

    return $resource;
  }
  catch (Exception $exception) {
    watchdog_exception('erpal_resource', $exception);
    return services_error($exception, 406, $resource_data);
  }
}

/**
 * Callback for the 'update' method.
 */
function _erpal_resource_resource_update($resource_id, $resource_data) {
  return _erpal_resource_resource_create($resource_data);
}

/**
 * Callback for the 'delete' method.
 */
function _erpal_resource_resource_delete($resource_id) {
  return erpal_resource_delete_multiple(array($resource_id));
}
