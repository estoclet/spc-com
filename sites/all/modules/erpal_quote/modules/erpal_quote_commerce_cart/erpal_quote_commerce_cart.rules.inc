<?php

/**
 * @file
 * Provides integration with Rules.
 */

/**
 * Implements hook_rules_event_info().
 */
function erpal_quote_commerce_cart_rules_event_info() {
  return array(
    'erpal_quote_commerce_cart_get_quote' => array(
      'label' => t('Downloading quote from the commerce cart'),
      'group' => t('ERPAL Quote'),
      'variables' => array(
        'order' => array(
          'type' => 'commerce_order',
          'label' => t('Order from cart'),
        ),
      ),
    ),
  );
}
