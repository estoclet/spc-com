<?php

/**
 * @file
 * Creates views used by default for displaying resource records.
 */

/**
 * Implements hook_views_data_alter().
 */
function erpal_resource_views_data_alter(&$data) {
  $data['erpal_resource']['table']['group'] = t('ERPAL resource');

  $data['erpal_resource']['table']['base'] = array(
    'field' => 'resource_id',
    'title' => t('Resource ID'),
    'help' => t('ERPAL Resource ID.'),
  );
  $data['erpal_resource']['table']['entity type'] = 'erpal_resource';

  // View, Edit and Delete links of resource.
  $data['erpal_resource']['view_erpal_resource'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a simple link to the resource.'),
      'handler' => 'erpal_resource_handler_field_link',
    ),
  );

  $data['erpal_resource']['edit_erpal_resource'] = array(
    'field' => array(
      'title' => t('Edit link'),
      'help' => t('Provide a simple link to edit the resource.'),
      'handler' => 'erpal_resource_handler_field_link_edit',
    ),
  );
  $data['erpal_resource']['delete_erpal_resource'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the resource.'),
      'handler' => 'erpal_resource_handler_field_link_delete',
    ),
  );

  $data['erpal_resource_type']['table']['join'] = array(
    'erpal_resource' => array(
      'left_field' => 'type',
      'field' => 'type',
    ),
  );
}
