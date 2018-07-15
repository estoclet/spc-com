<?php

/**
 * @file
 * erpal_budget_ui.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function erpal_budget_ui_views_default_views() {
  $view = new view();
  $view->name = 'erpal_budget';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'erpal_budget';
  $view->human_name = 'ERPAL Budget';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'ERPAL Budgets';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'table';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No Budgets available.';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  $handler->display->display_options['empty']['area']['tokenize'] = TRUE;
  /* Field: Bulk operations: ERPAL budget */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'erpal_budget';
  $handler->display->display_options['fields']['views_bulk_operations']['field'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['display_type'] = '0';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['enable_select_all_pages'] = 1;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['force_single'] = 0;
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_settings']['entity_load_capacity'] = '10';
  $handler->display->display_options['fields']['views_bulk_operations']['vbo_operations'] = array(
    'action::views_bulk_operations_delete_item' => array(
      'selected' => 1,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_delete_revision' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_script_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
    'action::views_bulk_operations_modify_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'show_all_tokens' => 1,
        'display_values' => array(
          '_all_' => '_all_',
        ),
      ),
    ),
    'action::views_bulk_operations_argument_selector_action' => array(
      'selected' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
      'settings' => array(
        'url' => '',
      ),
    ),
    'action::system_send_email_action' => array(
      'selected' => 0,
      'postpone_processing' => 0,
      'skip_confirmation' => 0,
      'override_label' => 0,
      'label' => '',
    ),
  );
  /* Field: ERPAL budget: budget ID */
  $handler->display->display_options['fields']['budget_id']['id'] = 'budget_id';
  $handler->display->display_options['fields']['budget_id']['table'] = 'erpal_budget';
  $handler->display->display_options['fields']['budget_id']['field'] = 'budget_id';
  /* Field: ERPAL budget: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'erpal_budget';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'erpal/budget/[budget_id]';
  /* Field: ERPAL budget: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'erpal_budget';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: ERPAL Budget: Available */
  $handler->display->display_options['fields']['field_budget_available']['id'] = 'field_budget_available';
  $handler->display->display_options['fields']['field_budget_available']['table'] = 'field_data_field_budget_available';
  $handler->display->display_options['fields']['field_budget_available']['field'] = 'field_budget_available';
  $handler->display->display_options['fields']['field_budget_available']['settings'] = array(
    'unit' => array(
      'formatter' => 'entityreference_label',
      'link' => 0,
    ),
    'value' => array(
      'formatter' => 'number_decimal',
      'thousand_separator' => ' ',
      'decimal_separator' => '.',
      'scale' => '2',
      'prefix_suffix' => 1,
    ),
    'mvf' => array(
      'unit_suggester_settings' => array(
        'MVFUnitSuggesterLocale' => array(
          'en' => '-1',
          'weight' => '-10',
          'enable' => 0,
        ),
        'MVFUnitSuggesterHardCoded' => array(
          'unit' => '1',
          'weight' => '-10',
          'enable' => 0,
        ),
        'MVFUnitSuggesterOriginal' => array(
          'weight' => '-10',
          'enable' => 0,
        ),
      ),
    ),
  );
  /* Field: ERPAL Budget: Total */
  $handler->display->display_options['fields']['field_budget_total']['id'] = 'field_budget_total';
  $handler->display->display_options['fields']['field_budget_total']['table'] = 'field_data_field_budget_total';
  $handler->display->display_options['fields']['field_budget_total']['field'] = 'field_budget_total';
  $handler->display->display_options['fields']['field_budget_total']['settings'] = array(
    'unit' => array(
      'formatter' => 'entityreference_label',
      'link' => 1,
    ),
    'value' => array(
      'formatter' => 'number_decimal',
      'thousand_separator' => ' ',
      'decimal_separator' => '.',
      'scale' => '2',
      'prefix_suffix' => 1,
    ),
    'mvf' => array(
      'unit_suggester_settings' => array(
        'MVFUnitSuggesterLocale' => array(
          'en' => '-1',
          'weight' => '-10',
          'enable' => 0,
        ),
        'MVFUnitSuggesterHardCoded' => array(
          'unit' => '1',
          'weight' => '-10',
          'enable' => 0,
        ),
        'MVFUnitSuggesterOriginal' => array(
          'weight' => '-10',
          'enable' => 0,
        ),
      ),
    ),
  );
  /* Field: ERPAL budget: Date updated */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'erpal_budget';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  /* Sort criterion: ERPAL budget: budget ID */
  $handler->display->display_options['sorts']['budget_id']['id'] = 'budget_id';
  $handler->display->display_options['sorts']['budget_id']['table'] = 'erpal_budget';
  $handler->display->display_options['sorts']['budget_id']['field'] = 'budget_id';
  $handler->display->display_options['sorts']['budget_id']['order'] = 'DESC';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: Result summary */
  $handler->display->display_options['footer']['result']['id'] = 'result';
  $handler->display->display_options['footer']['result']['table'] = 'views';
  $handler->display->display_options['footer']['result']['field'] = 'result';
  $handler->display->display_options['footer']['result']['content'] = '@total rows';
  $handler->display->display_options['path'] = 'admin/erpal/content/budget';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'ERPAL Budgets';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Content pane */
  $handler = $view->new_display('panel_pane', 'Content pane', 'panel_pane_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Budgets';
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: Field: Resource (field_resource) */
  $handler->display->display_options['arguments']['field_resource_target_id']['id'] = 'field_resource_target_id';
  $handler->display->display_options['arguments']['field_resource_target_id']['table'] = 'field_data_field_resource';
  $handler->display->display_options['arguments']['field_resource_target_id']['field'] = 'field_resource_target_id';
  $handler->display->display_options['arguments']['field_resource_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_resource_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_resource_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_resource_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['argument_input'] = array(
    'field_resource_target_id' => array(
      'type' => 'context',
      'context' => 'entity:erpal_resource.resource-id',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'Field: Resource (field_resource)',
    ),
  );
  $translatables['erpal_budget'] = array(
    t('Master'),
    t('ERPAL Budgets'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('No Budgets available.'),
    t('ERPAL budget'),
    t('- Choose an operation -'),
    t('budget ID'),
    t('.'),
    t(','),
    t('Title'),
    t('Type'),
    t('Available'),
    t('Total'),
    t('Date updated'),
    t('Page'),
    t('@total rows'),
    t('Content pane'),
    t('Budgets'),
    t('All'),
    t('View panes'),
  );

  $views[$view->name] = $view;

  return $views;
}
