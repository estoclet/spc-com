<?php

/**
 * @file
 * Resource definitions for services module.
 */

/**
 * Determine whether the current user can access a erpal_budget resource.
 *
 * @param string $op
 *   Operation, access for which should be checked.
 * @param mixed $arg
 *   budget ID or budget data array, in case $op is 'create'.
 *
 * @return bool
 *   Boolean.
 *
 * @see erpal_budget_access()
 */
function _erpal_budget_resource_access($op, $arg = NULL) {
  if (is_array($arg[0])) {
    $budget_data = array_shift($arg);

    return erpal_budget_access($op, $budget_data['type']);
  }
  elseif (is_numeric($arg[0])) {
    $erpal_budget = erpal_budget_load($arg[0]);

    return erpal_budget_access($op, $erpal_budget);
  }
  elseif (!isset($arg)) {
    return (user_access('administer erpal_budget entities') || user_access('view any erpal_budget entity'));
  }

  return FALSE;
}

/**
 * Return an array of erpal_budgets.
 *
 * @param int $page
 *   Page number of results to return (in pages of 20).
 * @param int $page_size
 *   Integer number of items to be returned.
 *
 * @return array
 *   An array of node objects.
 */
function _erpal_budget_resource_index($page, $page_size) {
  $aids = db_select('erpal_budget', 'c')
    ->orderBy('created', 'DESC')
    ->fields('c', array('budget_id'))
    ->range($page * $page_size, $page_size)
    ->execute()
    ->fetchCol();

  $budgets = erpal_budget_load_multiple($aids);

  return services_resource_build_index_list($budgets, 'erpal_budget', 'budget_id');
}

/**
 * Callback for the 'create' method.
 */
function _erpal_budget_resource_create($budget_data) {
  try {
    $budget = entity_create('erpal_budget', $budget_data);
    unset($budget->is_new);
    erpal_budget_save($budget);

    return $budget;
  }
  catch (Exception $exception) {
    watchdog_exception('erpal_budget', $exception);
    return services_error($exception, 406, $budget_data);
  }
}

/**
 * Callback for the 'update' method.
 */
function _erpal_budget_resource_update($budget_id, $budget_data) {
  return _erpal_budget_resource_create($budget_data);
}

/**
 * Callback for the 'delete' method.
 */
function _erpal_budget_resource_delete($budget_id) {
  return erpal_budget_delete_multiple(array($budget_id));
}
