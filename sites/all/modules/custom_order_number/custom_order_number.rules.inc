<?php


/**
 * Implements hook_rules_action_info().
 */
function custom_order_number_rules_action_info() {
  return array(
    'custom_order_number_nr' => array(
      'label' => t('Set order number on order'),
      'parameter' => array(
        'order' => array('type' => 'commerce_order', 'label' => t('Order')),
      ),
      'group' => t('Custom Order Number'),
    ),
  );
}
