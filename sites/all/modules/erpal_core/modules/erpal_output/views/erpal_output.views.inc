<?php

/**
 * @file
 * Creates views used by default for displaying output records.
 */

/**
 * Implements hook_views_data_alter().
 */
function erpal_output_views_data_alter(&$data) {
  $data['erpal_output']['table']['group'] = t('ERPAL output');

  $data['erpal_output']['table']['base'] = array(
    'field' => 'output_id',
    'title' => t('Output ID'),
    'help' => t('ERPAL Output ID.'),
  );
  $data['erpal_output']['table']['entity type'] = 'erpal_output';

  // View, Edit and Delete links of output.
  $data['erpal_output']['view_erpal_output'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a simple link to the output.'),
      'handler' => 'erpal_output_handler_field_link',
    ),
  );

  $data['erpal_output']['edit_erpal_output'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the output.'),
      'handler' => 'erpal_output_handler_field_link_edit',
    ),
  );
  $data['erpal_output']['delete_erpal_output'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the output.'),
      'handler' => 'erpal_output_handler_field_link_delete',
    ),
  );

  // View, Edit and Delete links of output.
  $data['erpal_output']['view_erpal_output'] = array(
    'field' => array(
      'title' => t('Links'),
      'help' => t('Provide a simple link to the output.'),
      'handler' => 'erpal_output_handler_field_links',
    ),
  );
}
