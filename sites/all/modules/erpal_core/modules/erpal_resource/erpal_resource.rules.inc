<?php

/**
 * @file
 * Provides default rules actions.
 */

/**
 * Implements hook_rules_action_info().
 */
function erpal_resource_rules_action_info() {
  $actions = array();
  $actions = array(
    'erpal_resource_create' => array(
      'label' => t('Create resource'),
      'group' => t('ERPAL Resource'),
    ),
  );

  return $actions;
}

/**
 * Rules action: create resource.
 */
function erpal_resource_create($settings, $parameters) {
  global $user;

  foreach ($parameters->variables as $parameter) {
    if ($parameter instanceof EntityDrupalWrapper) {
      $resource = entity_create('erpal_resource', array(
        'type' => $parameter->type() . ERPAL_RESOURCE_DELIMITER . $parameter->getBundle(),
        'title' => $parameter->label(),
        'uid' => $user->uid,
        'source_id' => $parameter->getIdentifier(),
      ));
      erpal_resource_save($resource);
      $parameter->field_resource->set($resource->resource_id);
      $parameter->value();
      $object = $parameter->value();
      unset($object->is_new);
      $parameter->save();
      module_invoke_all('budget_check_exists', $parameter, $resource);
    }
  }
}
