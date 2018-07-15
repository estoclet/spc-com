<?php
/**
 * @file
 * erpal_budget_ui.panels_default.inc
 */

/**
 * Implements hook_default_panels_mini().
 */
function erpal_budget_ui_default_panels_mini() {
  $export = array();

  // added to erpal_layout.module
  $mini = new stdClass();
  $mini->disabled = FALSE; /* Edit this to true to make a default mini disabled initially */
  $mini->api_version = 1;
  $mini->name = 'erpal_budget_main_view_tab';
  $mini->category = 'Mini panels';
  $mini->admin_title = 'ERPAL budget main view tab';
  $mini->admin_description = '';
  $mini->requiredcontexts = array(
    0 => array(
      'identifier' => 'ERPAL Budget',
      'keyword' => 'erpal_budget',
      'name' => 'entity:erpal_budget',
      'entity_id' => '',
      'id' => 1,
    ),
  );
  $mini->contexts = array();
  $mini->relationships = array();
  $display = new panels_display();
  $display->layout = 'erpaltworow';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'middle' => NULL,
      'header' => NULL,
      'body' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '131eb135-fd0f-4504-8532-764b5b927ee6';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-ac287902-168b-4470-b6cb-8bcdd493ec49';
    $pane->panel = 'body';
    $pane->type = 'views_panes';
    $pane->subtype = 'erpal_output-panel_pane_1';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => array(
        0 => 'requiredcontext_entity:erpal_budget_1',
      ),
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'ac287902-168b-4470-b6cb-8bcdd493ec49';
    $display->content['new-ac287902-168b-4470-b6cb-8bcdd493ec49'] = $pane;
    $display->panels['body'][0] = 'new-ac287902-168b-4470-b6cb-8bcdd493ec49';
    $pane = new stdClass();
    $pane->pid = 'new-613761be-11dc-4c81-8022-355cd37d32bf';
    $pane->panel = 'header';
    $pane->type = 'entity_view';
    $pane->subtype = 'erpal_budget';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'view_mode' => 'full',
      'context' => 'requiredcontext_entity:erpal_budget_1',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '613761be-11dc-4c81-8022-355cd37d32bf';
    $display->content['new-613761be-11dc-4c81-8022-355cd37d32bf'] = $pane;
    $display->panels['header'][0] = 'new-613761be-11dc-4c81-8022-355cd37d32bf';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $mini->display = $display;
  $export['erpal_budget_main_view_tab'] = $mini;

  return $export;
}
