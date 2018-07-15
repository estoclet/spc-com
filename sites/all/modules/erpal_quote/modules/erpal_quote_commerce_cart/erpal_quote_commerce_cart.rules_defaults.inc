<?php

/**
 * @file
 * Provides default rules for ERPAL Quote Commerce Cart module.
 */

/**
 * Implements hook_default_rules_configuration().
 */
function erpal_quote_commerce_cart_default_rules_configuration() {
  $items = array();

  $items['rules_create_quote_from_order_in_commerce_cart'] = entity_import('rules_config', '{ "rules_create_quote_from_order_in_commerce_cart" : {
    "LABEL" : "Download quote from order in commerce cart",
    "PLUGIN" : "reaction rule",
    "OWNER" : "rules",
    "TAGS" : [ "ERPAL Quote Commerce Cart" ],
    "REQUIRES" : [ "rules", "rules_conditional", "erpal_quote_commerce_cart" ],
    "ON" : { "erpal_quote_commerce_cart_get_quote" : [] },
    "IF" : [
      { "entity_is_of_bundle" : {
          "entity" : [ "order" ],
          "type" : "commerce_order",
          "bundle" : { "value" : { "commerce_order" : "commerce_order" } }
        }
      }
    ],
    "DO" : [
      { "CONDITIONAL" : [
          {
            "IF" : { "data_is_empty" : { "data" : [ "order:field-quote" ] } },
            "DO" : [
              { "entity_create" : {
                  "USING" : {
                    "type" : "commerce_order",
                    "param_status" : "feedback",
                    "param_type" : "quote",
                    "param_owner" : [ "order:owner" ],
                    "param_commerce_order_total" : [ "order:commerce-order-total" ]
                  },
                  "PROVIDE" : { "entity_created" : { "quote" : "Quote" } }
                }
              },
              { "data_set" : { "data" : [ "quote:field-vendor" ], "value" : [ "order:field-vendor" ] } },
              { "data_set" : {
                  "data" : [ "quote:field-vendor-address" ],
                  "value" : [ "order:field-vendor-address" ]
                }
              },
              { "data_set" : {
                  "data" : [ "quote:field-customer" ],
                  "value" : [ "order:field-customer" ]
                }
              },
              { "entity_save" : { "data" : [ "quote" ], "immediate" : 1 } },
              { "data_set" : { "data" : [ "order:field-quote" ], "value" : [ "quote" ] } }
            ]
          },
          { "ELSE" : [
              { "entity_fetch" : {
                  "USING" : { "type" : "commerce_order", "id" : [ "order:field-quote:order-id" ] },
                  "PROVIDE" : { "entity_fetched" : { "quote" : "Quote" } }
                }
              },
              { "data_set" : { "data" : [ "quote:commerce-line-items" ], "value" : { "value" : [] } } }
            ]
          }
        ]
      },
      { "LOOP" : {
          "USING" : { "list" : [ "order:commerce-line-items" ] },
          "ITEM" : { "line_item" : "Current line item" },
          "DO" : [
            { "CONDITIONAL" : [
                {
                  "IF" : { "entity_is_of_bundle" : {
                      "entity" : [ "line-item" ],
                      "type" : "commerce_line_item",
                      "bundle" : { "value" : { "product" : "product" } }
                    }
                  },
                  "DO" : [
                    { "entity_create" : {
                        "USING" : {
                          "type" : "commerce_line_item",
                          "param_order" : [ "quote" ],
                          "param_type" : "product",
                          "param_line_item_label" : [ "line-item:line-item-label" ],
                          "param_quantity" : [ "line-item:quantity" ],
                          "param_commerce_unit_price" : [ "line-item:commerce-unit-price" ],
                          "param_commerce_total" : [ "line-item:commerce-total" ]
                        },
                        "PROVIDE" : { "entity_created" : { "created_line_item" : "Created line item" } }
                      }
                    },
                    { "data_set" : {
                        "data" : [ "created-line-item:field-payment-modality" ],
                        "value" : [ "line-item:field-payment-modality" ]
                      }
                    },
                    { "data_set" : {
                        "data" : [ "created-line-item:commerce-product" ],
                        "value" : [ "line-item:commerce-product" ]
                      }
                    },
                    { "entity_save" : { "data" : [ "created-line-item" ], "immediate" : 1 } },
                    { "list_add" : {
                        "list" : [ "quote:commerce-line-items" ],
                        "item" : [ "created-line-item" ],
                        "unique" : 1
                      }
                    }
                  ]
                }
              ]
            }
          ]
        }
      },
      { "redirect" : { "url" : "quote-pdf\/[quote:order-id]" } }
    ]
  }
}');

  return $items;
}
