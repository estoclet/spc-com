<?php
function drupalexp_jollyness_form_install_configure_form_alter(&$form, $form_state) {
  $form['site_information']['site_name']['#default_value'] = ‘Jollyness - Responsive Multi-Purpose Drupal Theme';
}

/**
 * Implements hook_form_alter()
 */
if (!function_exists("system_form_install_select_profile_form_alter")) {
  function system_form_install_select_profile_form_alter(&$form, $form_state) {
    foreach ($form['profile'] as $key => $element) {
      $form['profile'][$key]['#value'] = 'drupalexp_evolve';
    }
  }
}
