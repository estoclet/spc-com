<?php
/**
 * @file
 * This file provides theme part of MCD module
 */

/**
 * Implements of hook_theme_registry_alter().
 *
 * Make drupal to use this maintenance-countdown-page.tpl.php in
 * maintenance mode if timer is set
 *
 * @param $theme_registry
 * The entire cache of theme registry information, post-processing.
 */
function mcd_theme_registry_alter(&$theme_registry) {
  // If site doing process like install or update don't use this template
  if (isset($theme_registry['maintenance_page']) && !defined('MAINTENANCE_MODE')) {
    $theme_registry['maintenance_page']['path'] = drupal_get_path('module', 'mcd');
    $theme_registry['maintenance_page']['template'] = 'maintenance-countdown-page';
//  Type should be 'theme' to prevent fatal error like:
//  can't call undefined fnc system_rebuild_MODULE_data() <-- LOL?
    $theme_registry['maintenance_page']['type'] = 'theme';
  }
}


/**
 * Generate the jQuery output
 */
function mcd_js_countdown() {
//  get stoptime like 1315776380
  $stoptime = variable_get('mcd_stoptime');
  $mcd_message = variable_get('mcd_time_up_message', FALSE);
  $mcd_reload = variable_get('mcd_reload_button', 0);
//  jQuery script for countdown initialization
  $output = "jQuery(document).ready(function($) {\$('#countdown_dashboard').countDown({targetDate: {";
  $output .= "'day': " .
      format_date($stoptime, 'custom', "j") . ", 'month': " .
      format_date($stoptime, 'custom', "n") . ", 'year': " .
      format_date($stoptime, 'custom', "Y") . ", 'hour': " .
      format_date($stoptime, 'custom', "G") . ", 'min': " .
      format_date($stoptime, 'custom', "i") . ", 'sec': " .
      format_date($stoptime, 'custom', "s") . ", 'utc': '" .
      format_date($stoptime, 'custom', "O") . "'}";
  if ($mcd_message || $mcd_reload != 1) {
    $output .= ", onComplete: function() {";
//  slide down 'time is up' message or 'Reload' button
    if ($mcd_message || $mcd_reload == 0) {
      $output .= " \$('#complete_message').slideDown();";
    }
    if ($mcd_reload == 2) {
      $output .= " setTimeout(function() {location.reload();}, 15000)";
    }
    $output .= "}";
  }
// close countDown
  $output .= "});";
//  reload current page by clicking 'Reload' button
  if (variable_get('mcd_reload_button', 0) == 0) {
    $output .= "\$('#page_reload').click(function() {location.reload()});";
  }
// close function
  $output .= "\$('#countdown_dashboard2').startCountDown();";
  $output .= "});";
  return $output;
}


/**
 * Add some variables for maintenance-coundown-page.tpl.php file.
 * You can override this or core or add custom variables via your theme
 * just using the same TEMPLATE_preprocess_maintenance_page
 * @param $variables
 */
function mcd_preprocess_maintenance_page(&$variables) {
  $path = drupal_get_path('module', 'mcd');

//   CUT unneeded js files! Yay! Should work as magic...
  $js = drupal_add_js();
  array_splice($js, 4);
  $variables['js'] = drupal_get_js('header', $js);

  $variables['mcd_js'] = '<script src="' . $GLOBALS['base_url'] . '/' .
    drupal_get_path('module', 'mcd') . '/js/jquery.lwtCountdown-1.0-min.js' . "\"></script>\n";
  $variables['mcd_js_countdown'] = mcd_js_countdown();

  if (variable_get('mcd_site_logo') == 1) {
    unset($variables['logo']);
  }
  if (variable_get('mcd_site_name') == 1) {
    unset($variables['site_name']);
  }
  if (variable_get('mcd_site_slogan') == 1) {
    unset($variables['site_slogan']);
  }

//  add theme css
  drupal_add_css(drupal_get_path('module', 'mcd') . '/styles/' . variable_get('mcd_page_themes', 'light') . '.css', array(
    'type' => 'file',
    'group' => CSS_THEME,
    'every_page' => FALSE,
    'weight' => 99,
    'preprocess' => FALSE)
  );

  $variables['time_up_message'] = variable_get('mcd_time_up_message', FALSE);

  if (variable_get('mcd_reload_button', 0) == 0) {
    $variables['reload_button'] = l(t('♻ Reload'), $_GET['q'] , array(
        'attributes' => array(
            'id' => 'page_reload',
            'title' => t('Reload this page')
        )));
  }
  else {
    $variables['reload_button'] = FALSE;
  }

  if (mcd_check_simplenews() && variable_get('mcd_email_subscr', 0) != 0) {
    $block = module_invoke('simplenews', 'block_view', variable_get('mcd_email_subscr'));
    $variables['subsc'] = render($block['content']);
  }
}
