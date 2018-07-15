<?php

/**
 * @file
 * Extends EntityAPIControllerInterface with classes for resources.
 */

/**
 * ERPAL resource Entity Class.
 */
class ERPALResourceEntity extends Entity {
  /**
   * Defines the entity label if the 'entity_class_label' callback is used.
   */
  protected function defaultLabel() {
    return $this->title;
  }

  /**
   * Override this in order to implement a custom default URI.
   */
  protected function defaultUri() {
    return array(
      'path' => 'erpal/resource/' . $this->identifier(),
      'options' => array(
        'absolute' => TRUE,
      ),
    );
  }
}

/**
 * Controller class for ERPAL resource.
 */
class ERPALResourceController extends EntityAPIController {
  /**
   * Implements EntityAPIControllerInterface.
   */
  public function create(array $values = array()) {
    $values += array(
      'title' => '',
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'uid' => $GLOBALS['user']->uid,
      'source_id' => 0,
    );

    return parent::create($values);
  }

  /**
   * Update resource object before saving revision.
   */
  protected function saveRevision($entity) {
    $entity->is_new_revision = TRUE;

    return parent::saveRevision($entity);
  }
}

/**
 * Provides a class for resources.
 *
 * Use a separate class so we can specify some defaults modules may be alter.
 */
class ERPALResourceType extends Entity {
  public $type;
  public $label;
  public $weight = 0;

  /**
   * Creates a new budget type.
   *
   * @see entity_create()
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'erpal_resource_type');
  }

  /**
   * Check budget status.
   */
  public function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }
}

/**
 * Controller class for ERPAL resource Type.
 */
class ERPALResourceTypeController extends EntityAPIControllerExportable {
  /**
   * Implements EntityAPIControllerInterface.
   */
  public function create(array $values = array()) {
    $values += array(
      'type' => '',
      'label' => '',
      'description' => '',
      'is_new' => TRUE,
    );

    return parent::create($values);
  }

  /**
   * Implements EntityAPIControllerInterface.
   *
   * @param array $ids
   *   List of IDs.
   * @param object $transaction
   *   Optionally a DatabaseTransaction object to use. Allows overrides to pass
   *   in their transaction object.
   */
  public function delete($ids, DatabaseTransaction $transaction = NULL) {
    // Delete all instances of the given type.
    $query = new EntityFieldQuery();
    $results = $query
      ->entityCondition('entity_type', 'erpal_resource')
      ->entityCondition('bundle', $ids, 'IN')
      ->execute();

    if (!empty($results)) {
      $resources_to_remove = array_keys($results['erpal_resource']);
      erpal_resource_delete_multiple($resources_to_remove);
      watchdog('erpal_resource', 'Delete !count resources due to deletion of resource type.', array('!count' => count($resources_to_remove)), WATCHDOG_INFO);
    }

    // Finally delete the type itself.
    return parent::delete($ids, $transaction);
  }
}
