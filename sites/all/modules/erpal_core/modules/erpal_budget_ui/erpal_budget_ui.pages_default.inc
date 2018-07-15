<?php

/**
 * @file
 * Contains default pages for ERPAL Budget UI module.
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function erpal_budget_ui_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'erpal_budget_view_template';
  $page->task = 'page';
  $page->admin_title = 'Erpal Budget view template';
  $page->admin_description = '';
  $page->path = 'erpal/budget/%erpal_budget';
  $page->access = array();
  $page->menu = array();
  $page->arguments = array(
    'erpal_budget' => array(
      'id' => 1,
      'identifier' => 'ERPAL Budget: ID',
      'name' => 'entity_id:erpal_budget',
      'settings' => array(),
    ),
  );
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_erpal_budget_view_template_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'erpal_budget_view_template';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'flexible';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => array(
        'region_title' => '',
        'region_title_tag' => 'none',
        'region_class' => '',
        'panel_tab_id_pattern' => 'region',
      ),
    ),
    'center' => array(
      'style' => 'tabs',
    ),
  );
  $display->cache = array();
  $display->title = '%erpal_budget:title';
  $display->uuid = '4c5a3d37-05f1-4438-8e9a-ec28d25b8b0a';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-3b8d049b-3bb7-4a7d-b103-790f44498bc2';
  $pane->panel = 'center';
  $pane->type = 'panels_mini';
  $pane->subtype = 'erpal_budget_main_view_tab';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => array(
      0 => 'argument_entity_id:erpal_budget_1',
    ),
    'override_title' => 1,
    'override_title_text' => 'Budget',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '3b8d049b-3bb7-4a7d-b103-790f44498bc2';
  $display->content['new-3b8d049b-3bb7-4a7d-b103-790f44498bc2'] = $pane;
  $display->panels['center'][0] = 'new-3b8d049b-3bb7-4a7d-b103-790f44498bc2';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;

  $pages['erpal_budget_view_template'] = $page;

  return $pages;
}
