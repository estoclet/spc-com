<?php

/**
 * @file
 * Provides theme functions for ERPAL Output.
 */

/**
 * Implements hook_theme().
 */
function erpal_output_theme($existing, $type, $theme, $path) {
  return array(
    'output' => array(
      'render element' => 'elements',
      'template' => 'output',
      'path' => $path . '/templates',
    ),
  );
}

/**
 * Implements hook_preprocess().
 */
function erpal_output_preprocess_output(&$variables) {
  $variables['view_mode'] = $variables['elements']['#view_mode'];
  $variables['output'] = $variables['elements']['#output'];
  $variables['content'] = entity_build_content('erpal_output', $variables['output']);

  // Add classes based upon output type.
  $variables['classes_array'][] = 'output';
  $variables['classes_array'][] = 'output-' . $variables['output']->type;

  $entity = $variables['output'];
  $variables['theme_hook_suggestions'][] = 'output__' . $entity->type;
  $variables['theme_hook_suggestions'][] = 'output__' . $entity->output_id;
}
