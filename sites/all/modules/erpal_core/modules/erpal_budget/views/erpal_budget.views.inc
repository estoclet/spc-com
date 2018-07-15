<?php

/**
 * @file
 * Creates views used by default for displaying budget records.
 */

/**
 * Implements hook_views_data_alter().
 */
function erpal_budget_views_data_alter(&$data) {
  $data['erpal_budget']['table']['group'] = t('ERPAL budget');

  $data['erpal_budget']['table']['base'] = array(
    'field' => 'budget_id',
    'title' => t('Budget ID'),
    'help' => t('ERPAL Budget ID.'),
  );
  $data['erpal_budget']['table']['entity type'] = 'erpal_budget';

  // View, Edit and Delete links of budget.
  $data['erpal_budget']['view_erpal_budget'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a simple link to the budget.'),
      'handler' => 'erpal_budget_handler_field_link',
    ),
  );

  $data['erpal_budget']['edit_erpal_budget'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the budget.'),
      'handler' => 'erpal_budget_handler_field_link_edit',
    ),
  );
  $data['erpal_budget']['delete_erpal_budget'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the budget.'),
      'handler' => 'erpal_budget_handler_field_link_delete',
    ),
  );

  $data['erpal_budget_type']['table']['join'] = array(
    'erpal_budget' => array(
      'left_field' => 'type',
      'field' => 'type',
    ),
  );
}
