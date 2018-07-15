<?php

/**
 * @file
 * erpal_output_ui.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function erpal_output_ui_views_default_views() {
  $view = new view();
  $view->name = 'erpal_output';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'erpal_output';
  $view->human_name = 'ERPAL Output';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'ERPAL Outputs';
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
  $handler->display->display_options['empty']['area']['content'] = 'No Outputs available.';
  $handler->display->display_options['empty']['area']['format'] = 'full_html';
  /* Field: Bulk operations: ERPAL output */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'erpal_output';
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
  /* Field: ERPAL output: output ID */
  $handler->display->display_options['fields']['output_id']['id'] = 'output_id';
  $handler->display->display_options['fields']['output_id']['table'] = 'erpal_output';
  $handler->display->display_options['fields']['output_id']['field'] = 'output_id';
  /* Field: ERPAL output: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'erpal_output';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'erpal/output/[output_id]';
  /* Field: ERPAL output: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'erpal_output';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: ERPAL Output: Unit */
  $handler->display->display_options['fields']['field_output_unit']['id'] = 'field_output_unit';
  $handler->display->display_options['fields']['field_output_unit']['table'] = 'field_data_field_output_unit';
  $handler->display->display_options['fields']['field_output_unit']['field'] = 'field_output_unit';
  $handler->display->display_options['fields']['field_output_unit']['label'] = 'Value';
  $handler->display->display_options['fields']['field_output_unit']['settings'] = array(
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
  /* Field: ERPAL output: Date updated */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'erpal_output';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  /* Sort criterion: ERPAL output: output ID */
  $handler->display->display_options['sorts']['output_id']['id'] = 'output_id';
  $handler->display->display_options['sorts']['output_id']['table'] = 'erpal_output';
  $handler->display->display_options['sorts']['output_id']['field'] = 'output_id';
  $handler->display->display_options['sorts']['output_id']['order'] = 'DESC';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['footer'] = FALSE;
  /* Footer: Global: Result summary */
  $handler->display->display_options['footer']['result']['id'] = 'result';
  $handler->display->display_options['footer']['result']['table'] = 'views';
  $handler->display->display_options['footer']['result']['field'] = 'result';
  $handler->display->display_options['footer']['result']['content'] = '@total rows';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Bulk operations: ERPAL output */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'erpal_output';
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
  /* Field: ERPAL output: output ID */
  $handler->display->display_options['fields']['output_id']['id'] = 'output_id';
  $handler->display->display_options['fields']['output_id']['table'] = 'erpal_output';
  $handler->display->display_options['fields']['output_id']['field'] = 'output_id';
  /* Field: ERPAL output: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'erpal_output';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'erpal/output/[output_id]';
  /* Field: ERPAL output: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'erpal_output';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: ERPAL Output: Unit */
  $handler->display->display_options['fields']['field_output_unit']['id'] = 'field_output_unit';
  $handler->display->display_options['fields']['field_output_unit']['table'] = 'field_data_field_output_unit';
  $handler->display->display_options['fields']['field_output_unit']['field'] = 'field_output_unit';
  $handler->display->display_options['fields']['field_output_unit']['label'] = 'Value';
  $handler->display->display_options['fields']['field_output_unit']['settings'] = array(
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
  /* Field: ERPAL Output: Budget */
  $handler->display->display_options['fields']['field_output_budget']['id'] = 'field_output_budget';
  $handler->display->display_options['fields']['field_output_budget']['table'] = 'field_data_field_output_budget';
  $handler->display->display_options['fields']['field_output_budget']['field'] = 'field_output_budget';
  $handler->display->display_options['fields']['field_output_budget']['settings'] = array(
    'link' => 1,
  );
  /* Field: Field: Resource */
  $handler->display->display_options['fields']['field_resource']['id'] = 'field_resource';
  $handler->display->display_options['fields']['field_resource']['table'] = 'field_data_field_resource';
  $handler->display->display_options['fields']['field_resource']['field'] = 'field_resource';
  $handler->display->display_options['fields']['field_resource']['settings'] = array(
    'link' => 1,
  );
  /* Field: ERPAL output: Date updated */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'erpal_output';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  $handler->display->display_options['path'] = 'admin/erpal/content/output';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'ERPAL Outputs';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Budget */
  $handler = $view->new_display('panel_pane', 'Budget', 'panel_pane_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Outputs';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: ERPAL output: output ID */
  $handler->display->display_options['fields']['output_id']['id'] = 'output_id';
  $handler->display->display_options['fields']['output_id']['table'] = 'erpal_output';
  $handler->display->display_options['fields']['output_id']['field'] = 'output_id';
  /* Field: ERPAL output: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'erpal_output';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'erpal/output/[output_id]';
  /* Field: ERPAL output: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'erpal_output';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: ERPAL Output: Budget */
  $handler->display->display_options['fields']['field_output_budget']['id'] = 'field_output_budget';
  $handler->display->display_options['fields']['field_output_budget']['table'] = 'field_data_field_output_budget';
  $handler->display->display_options['fields']['field_output_budget']['field'] = 'field_output_budget';
  $handler->display->display_options['fields']['field_output_budget']['label'] = 'Value';
  $handler->display->display_options['fields']['field_output_budget']['settings'] = array(
    'separator' => ': ',
    'budget_title' => 0,
    'budget_filter' => 0,
    'link' => 1,
    'thousand_separator' => ' ',
    'decimal_separator' => '.',
    'scale' => '2',
    'prefix_suffix' => 1,
  );
  $handler->display->display_options['fields']['field_output_budget']['group_rows'] = FALSE;
  $handler->display->display_options['fields']['field_output_budget']['delta_offset'] = '0';
  /* Field: ERPAL output: Date updated */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'erpal_output';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  /* Field: Bulk operations: ERPAL output */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'erpal_output';
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
  $handler->display->display_options['defaults']['arguments'] = FALSE;
  /* Contextual filter: ERPAL Output: Budget (field_output_budget:target_id) */
  $handler->display->display_options['arguments']['field_output_budget_target_id']['id'] = 'field_output_budget_target_id';
  $handler->display->display_options['arguments']['field_output_budget_target_id']['table'] = 'field_data_field_output_budget';
  $handler->display->display_options['arguments']['field_output_budget_target_id']['field'] = 'field_output_budget_target_id';
  $handler->display->display_options['arguments']['field_output_budget_target_id']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['field_output_budget_target_id']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['field_output_budget_target_id']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['field_output_budget_target_id']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  $handler->display->display_options['argument_input'] = array(
    'field_output_budget_target_id' => array(
      'type' => 'context',
      'context' => 'entity:erpal_budget.budget-id',
      'context_optional' => 0,
      'panel' => '0',
      'fixed' => '',
      'label' => 'ERPAL Output: Budget (field_output_budget)',
    ),
  );

  /* Display: Resource */
  $handler = $view->new_display('panel_pane', 'Resource', 'panel_pane_2');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Outputs';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Bulk operations: ERPAL output */
  $handler->display->display_options['fields']['views_bulk_operations']['id'] = 'views_bulk_operations';
  $handler->display->display_options['fields']['views_bulk_operations']['table'] = 'erpal_output';
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
  /* Field: ERPAL output: output ID */
  $handler->display->display_options['fields']['output_id']['id'] = 'output_id';
  $handler->display->display_options['fields']['output_id']['table'] = 'erpal_output';
  $handler->display->display_options['fields']['output_id']['field'] = 'output_id';
  /* Field: ERPAL output: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'erpal_output';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['path'] = 'erpal/output/[output_id]';
  /* Field: ERPAL output: Type */
  $handler->display->display_options['fields']['type']['id'] = 'type';
  $handler->display->display_options['fields']['type']['table'] = 'erpal_output';
  $handler->display->display_options['fields']['type']['field'] = 'type';
  /* Field: ERPAL output: Saldo */
  $handler->display->display_options['fields']['saldo']['id'] = 'saldo';
  $handler->display->display_options['fields']['saldo']['table'] = 'views_entity_erpal_output';
  $handler->display->display_options['fields']['saldo']['field'] = 'saldo';
  $handler->display->display_options['fields']['saldo']['exclude'] = TRUE;
  $handler->display->display_options['fields']['saldo']['set_precision'] = TRUE;
  $handler->display->display_options['fields']['saldo']['precision'] = '2';
  $handler->display->display_options['fields']['saldo']['separator'] = ', ';
  $handler->display->display_options['fields']['saldo']['link_to_entity'] = 0;
  /* Field: ERPAL Output: Unit */
  $handler->display->display_options['fields']['field_output_unit']['id'] = 'field_output_unit';
  $handler->display->display_options['fields']['field_output_unit']['table'] = 'field_data_field_output_unit';
  $handler->display->display_options['fields']['field_output_unit']['field'] = 'field_output_unit';
  $handler->display->display_options['fields']['field_output_unit']['label'] = 'Saldo / Total';
  $handler->display->display_options['fields']['field_output_unit']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['field_output_unit']['alter']['text'] = '[saldo] / [field_output_unit]';
  $handler->display->display_options['fields']['field_output_unit']['settings'] = array(
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
  /* Field: ERPAL Output: Budget */
  $handler->display->display_options['fields']['field_output_budget']['id'] = 'field_output_budget';
  $handler->display->display_options['fields']['field_output_budget']['table'] = 'field_data_field_output_budget';
  $handler->display->display_options['fields']['field_output_budget']['field'] = 'field_output_budget';
  $handler->display->display_options['fields']['field_output_budget']['settings'] = array(
    'link' => 1,
  );
  /* Field: ERPAL output: Date updated */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'erpal_output';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
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
  $translatables['erpal_output'] = array(
    t('Master'),
    t('ERPAL Outputs'),
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
    t('No Outputs available.'),
    t('ERPAL output'),
    t('- Choose an operation -'),
    t('output ID'),
    t('.'),
    t(','),
    t('Title'),
    t('Type'),
    t('Value'),
    t('Date updated'),
    t('Page'),
    t('@total rows'),
    t('Budget'),
    t('Resource'),
    t('Outputs'),
    t('All'),
    t('View panes'),
    t('Saldo'),
    t(', '),
    t('Saldo / Total'),
    t('[saldo] / [field_output_unit]'),
  );
  $views['erpal_output'] = $view;

  return $views;
}
