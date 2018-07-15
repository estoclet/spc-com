<?php

/**
 * Selection handler for output budget select widget.
 */
class ERPALOutput_SelectionHandler_BudgetsField extends EntityReference_SelectionHandler_Generic {
  /**
   * Implements EntityReferenceHandler::getInstance().
   */
  public static function getInstance($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
    $target_entity_type = $field['settings']['target_type'];

    // Check if the entity type does exist and has a base table.
    $entity_info = entity_get_info($target_entity_type);
    if (empty($entity_info['base table'])) {
      return EntityReference_SelectionHandler_Broken::getInstance($field, $instance);
    }

    return new ERPALOutput_SelectionHandler_BudgetsField($field, $instance, $entity_type, $entity);
  }

  /**
   * Build an EntityFieldQuery to get referencable budgets.
   */
  protected function buildEntityFieldQuery($match = NULL, $match_operator = 'CONTAINS') {

    // Take all values from output and prepare it.
    $info_output = field_info_instance('erpal_output', 'field_output_unit', $this->entity->type);
    $unit_type = $info_output['settings']['unit']['target_bundles'];

    $efq = new EntityFieldQuery();
    $result = $efq->entityCondition('entity_type', 'units_unit')
      ->entityCondition('bundle', $unit_type)
      ->execute();
    $unit_ids = isset($result['units_unit']) ? array_keys($result['units_unit']) : array();

    // Prepare query.
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $this->field['settings']['target_type'])
      ->fieldCondition('field_budget_available', 'target_id', $unit_ids, 'IN')
      // @todo include already assigned budgets with availabale <= 0 here.
      //->fieldCondition('field_budget_available', 'value', 0, '>')
      ->fieldOrderBy('field_budget_available', 'value');

    if (isset($match)) {
      $query->fieldCondition('field_resource', 'target_id', $match, $match_operator);
    }

    $query->addTag($this->field['settings']['target_type'] . '_access');
    $query->addTag('entityreference');
    $query->addTag('erpal_output_budget_field');
    $query->addMetaData('field', $this->field);
    $query->addMetaData('entityreference_selection_handler', $this);

    return $query;
  }
}
