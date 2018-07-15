<?php

/**
 * @file
 * Extends EntityAPIControllerInterface with classes for outputs.
 */

/**
 * ERPAL output Entity Class.
 */
class ERPALOutputEntity extends Entity {
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
      'path' => 'erpal/output/' . $this->identifier(),
      'options' => array(
        'absolute' => TRUE,
      ),
    );
  }
}

/**
 * Controller class for ERPAL output.
 */
class ERPALOutputController extends EntityAPIController {
  /**
   * Implements EntityAPIControllerInterface.
   */
  public function create(array $values = array()) {
    $values += array(
      'title' => '',
      'created' => REQUEST_TIME,
      'changed' => REQUEST_TIME,
      'uid' => $GLOBALS['user']->uid,
    );

    return parent::create($values);
  }

  /**
   * Update output object before saving revision.
   */
  protected function saveRevision($entity) {
    $entity->is_new_revision = TRUE;

    return parent::saveRevision($entity);
  }
}

/**
 * Provides a class for outputs.
 *
 * Use a separate class so we can specify some defaults modules may be alter.
 */
class ERPALOutputType extends Entity {
  public $type;
  public $label;
  public $weight = 0;

  /**
   * Creates a new output type.
   *
   * @see entity_create()
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'erpal_output_type');
  }

  /**
   * Check output status.
   */
  public function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }
}

/**
 * Controller class for ERPAL output Type.
 */
class ERPALOutputTypeController extends EntityAPIControllerExportable {
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
      ->entityCondition('entity_type', 'erpal_output')
      ->entityCondition('bundle', $ids, 'IN')
      ->execute();

    if (!empty($results)) {
      $outputs_to_remove = array_keys($results['erpal_output']);
      erpal_output_delete_multiple($outputs_to_remove);
      watchdog('erpal_output', 'Delete !count outputs due to deletion of output type.', array('!count' => count($outputs_to_remove)), WATCHDOG_INFO);
    }

    // Finally delete the type itself.
    return parent::delete($ids, $transaction);
  }
}
