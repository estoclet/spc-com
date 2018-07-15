<?php
/**
 * @file
 * custom_order_number.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function custom_order_number_default_rules_configuration() {
  $items = array();
  $items['rules_commerce_billy_generate_invoice_number_on_order_insert'] = entity_import('rules_config', '{ "rules_custom_order_number" : {
      "LABEL" : "Custom order number",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "custom_order_number", "entity" ],
      "ON" : { "commerce_order_insert" : [] },
      "DO" : [ { "custom_order_number_nr" : { "order" : [ "commerce-order" ] } } ]
    }
  }');
  return $items;
}
