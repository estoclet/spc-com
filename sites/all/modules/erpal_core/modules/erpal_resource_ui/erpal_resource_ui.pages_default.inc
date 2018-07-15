<?php

/**
 * @file
 * Contains default pages for ERPAL Resource UI module.
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function erpal_resource_ui_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'erpal_resource_view_template';
  $page->task = 'page';
  $page->admin_title = 'Erpal Resource view template';
  $page->admin_description = '';
  $page->path = 'erpal/resource/%erpal_resource';
  $page->access = array();
  $page->menu = array();
  $page->arguments = array(
    'erpal_resource' => array(
      'id' => 1,
      'identifier' => 'ERPAL Resource: ID',
      'name' => 'entity_id:erpal_resource',
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
  $handler->name = 'page_erpal_resource_view_template_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'erpal_resource_view_template';
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
  $display->title = '%erpal_resource:title';
  $display->uuid = '7b333381-1909-4c3f-8979-89005828fcc5';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-4842d7e7-6bcb-450f-9e18-476d7681ff8c';
  $pane->panel = 'center';
  $pane->type = 'entity_view';
  $pane->subtype = 'erpal_resource';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'view_mode' => 'full',
    'context' => 'argument_entity_id:erpal_resource_1',
    'override_title' => 1,
    'override_title_text' => 'Resource',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '4842d7e7-6bcb-450f-9e18-476d7681ff8c';
  $display->content['new-4842d7e7-6bcb-450f-9e18-476d7681ff8c'] = $pane;
  $display->panels['center'][0] = 'new-4842d7e7-6bcb-450f-9e18-476d7681ff8c';
  $pane = new stdClass();
  $pane->pid = 'new-40aad655-9f7c-415f-b447-d70161f1dd79';
  $pane->panel = 'center';
  $pane->type = 'views_panes';
  $pane->subtype = 'erpal_budget-panel_pane_1';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => array(
      0 => 'argument_entity_id:erpal_resource_1',
    ),
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = '40aad655-9f7c-415f-b447-d70161f1dd79';
  $display->content['new-40aad655-9f7c-415f-b447-d70161f1dd79'] = $pane;
  $display->panels['center'][1] = 'new-40aad655-9f7c-415f-b447-d70161f1dd79';
  $pane = new stdClass();
  $pane->pid = 'new-5d8c8a26-3b2a-45bd-9f54-428244ae0048';
  $pane->panel = 'center';
  $pane->type = 'views_panes';
  $pane->subtype = 'erpal_output-panel_pane_2';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'context' => array(
      0 => 'argument_entity_id:erpal_resource_1',
    ),
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = '5d8c8a26-3b2a-45bd-9f54-428244ae0048';
  $display->content['new-5d8c8a26-3b2a-45bd-9f54-428244ae0048'] = $pane;
  $display->panels['center'][2] = 'new-5d8c8a26-3b2a-45bd-9f54-428244ae0048';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-4842d7e7-6bcb-450f-9e18-476d7681ff8c';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;

  $pages['erpal_resource_view_template'] = $page;

  return $pages;
}
