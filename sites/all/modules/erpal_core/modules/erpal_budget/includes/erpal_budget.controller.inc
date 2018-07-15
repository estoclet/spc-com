<?php

/**
 * @file
 * Extends EntityAPIControllerInterface with classes for budgets.
 */

/**
 * ERPAL budget Entity Class.
 */
class ERPALBudgetEntity extends Entity {
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
      'path' => 'erpal/budget/' . $this->identifier(),
      'options' => array(
        'absolute' => TRUE,
      ),
    );
  }

  /**
   * Set budget locked if at least one output assigned.
   */
  public function isLocked() {
    if (!empty($this->is_new)) {
      return FALSE;
    }

    // Check if budget has attached outputs.
    $query = new EntityFieldQuery();
    $budgets_count = $query
      ->entityCondition('entity_type', 'erpal_output')
      ->fieldCondition('field_output_budget', 'target_id', $this->budget_id)
      ->count()
      ->execute();

    return $budgets_count > 0;
  }
}

/**
 * Controller class for ERPAL budget.
 */
class ERPALBudgetController extends EntityAPIController {
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
   * Implements EntityAPIControllerInterface.
   */
  public function save($entity) {

    // Take available value for budget.
    $field_budget_available = field_get_items('erpal_budget', $entity, 'field_budget_available');

    // If available value is empty than this budget is new, and need take value
    // from total value.
    if (!$field_budget_available || !isset($field_budget_available[0]['value'])) {
      $field_budget_total = field_get_items('erpal_budget', $entity, 'field_budget_total');

      // If total is empty too, than this budget not valid.
      if (!$field_budget_total || empty($field_budget_total[0]['value'])) {
        return FALSE;
      }
      $language = field_language('erpal_budget', $entity, 'field_budget_available');
      $entity->field_budget_available[$language] = $field_budget_total;
    }
    
    // Prepare percentage of budget availability.
    $total = field_get_items('erpal_budget', $entity, 'field_budget_total');
    $available = field_get_items('erpal_budget', $entity, 'field_budget_available');
  
    $total_unit = units_unit_load($total[0]['target_id']);
    $available_unit = units_unit_load($available[0]['target_id']);
    $total_value = units_convert($total[0]['value'],
                $total_unit->machine_name, $available_unit->machine_name);
    if ($total_value == 0) {
      $entity->availability = 0;
    }
    else {
      $entity->availability = ($available[0]['value'] / $total_value) * 100;
    }

    return parent::save($entity);
  }

  /**
   * Update budget object before saving revision.
   */
  protected function saveRevision($entity) {
    $entity->is_new_revision = TRUE;

    return parent::saveRevision($entity);
  }
}

/**
 * Provides a class for budgets.
 *
 * Use a separate class so we can specify some defaults modules may be alter.
 */
class ERPALBudgetType extends Entity {
  public $type;
  public $label;
  public $weight = 0;

  /**
   * Creates a new budget type.
   *
   * @see entity_create()
   */
  public function __construct($values = array()) {
    parent::__construct($values, 'erpal_budget_type');
  }

  /**
   * Check budget status.
   */
  public function isLocked() {
    return isset($this->status) && empty($this->is_new) && (($this->status & ENTITY_IN_CODE) || ($this->status & ENTITY_FIXED));
  }
}

/**
 * Controller class for ERPAL budget Type.
 */
class ERPALBudgetTypeController extends EntityAPIControllerExportable {
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
      ->entityCondition('entity_type', 'erpal_budget')
      ->entityCondition('bundle', $ids, 'IN')
      ->execute();

    if (!empty($results)) {
      $budgets_to_remove = array_keys($results['erpal_budget']);
      erpal_budget_delete_multiple($budgets_to_remove);
      watchdog('erpal_budget', 'Delete !count budgets due to deletion of budget type.', array('!count' => count($budgets_to_remove)), WATCHDOG_INFO);
    }

    // Finally delete the type itself.
    return parent::delete($ids, $transaction);
  }
}
