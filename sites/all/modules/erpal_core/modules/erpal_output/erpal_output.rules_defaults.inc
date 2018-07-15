<?php
/**
 * @file
 * erpal_output.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function erpal_output_default_rules_configuration() {
  $items = array();
  $items['rules_attach_budgets_to_output'] = entity_import('rules_config', '{ "rules_attach_budgets_to_output" : {
      "LABEL" : "Attach budgets to Output",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "Erpal Output" ],
      "REQUIRES" : [ "rules", "erpal_output", "rules_conditional" ],
      "USES VARIABLES" : { "erpal_output" : { "label" : "ERPAL Output", "type" : "erpal_output" } },
      "IF" : [
        { "entity_has_field" : { "entity" : [ "erpal-output" ], "field" : "field_output_budget" } }
      ],
      "DO" : [
        { "erpal_output_fetch_field_budgets" : {
            "USING" : { "erpal_output" : [ "erpal-output" ] },
            "PROVIDE" : { "fetched_budgets" : { "fetched_budgets" : "Fetched budgets" } }
          }
        },
        { "erpal_output_clear_budget_field" : { "erpal_output" : [ "erpal_output" ] } },
        { "erpal_output_attach_budgets" : { "erpal_output" : [ "erpal-output" ], "budgets" : [ "fetched-budgets" ] } },
        { "erpal_output_find_and_attach_resource" : { "erpal_output" : [ "erpal-output" ] } },
        { "CONDITIONAL" : [
            {
              "IF" : { "NOT data_is_empty" : { "data" : [ "erpal-output:saldo" ] } },
              "DO" : [
                { "erpal_output_fetch_budgets_from_hook" : {
                    "USING" : { "erpal_output" : [ "erpal-output" ] },
                    "PROVIDE" : { "fetched_budgets" : { "fetched_budgets_from_hook" : "Fetched budgets" } }
                  }
                },
                { "erpal_output_attach_budgets" : {
                    "erpal_output" : [ "erpal-output" ],
                    "budgets" : [ "fetched-budgets-from-hook" ]
                  }
                }
              ]
            }
          ]
        }
      ]
    }
  }');
  $items['rules_erpal_output_attach_budgets_output_added'] = entity_import('rules_config', '{ "rules_erpal_output_attach_budgets_output_added" : {
      "LABEL" : "ERPAL Output Attach budgets (Output added)",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Erpal Output" ],
      "REQUIRES" : [ "rules", "erpal_output" ],
      "ON" : { "erpal_output_presave" : [] },
      "IF" : [ { "entity_is_new" : { "entity" : [ "erpal-output" ] } } ],
      "DO" : [
        { "component_rules_attach_budgets_to_output" : { "erpal_output" : [ "erpal-output" ] } },
        { "erpal_output_find_and_attach_resource" : { "erpal_output" : [ "erpal-output" ] } }
      ]
    }
  }');
  $items['rules_erpal_output_attach_budgets_output_updated'] = entity_import('rules_config', '{ "rules_erpal_output_attach_budgets_output_updated" : {
      "LABEL" : "ERPAL Output Attach budgets (Output updated)",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Erpal Output" ],
      "REQUIRES" : [ "rules", "erpal_output" ],
      "ON" : { "erpal_output_presave" : [] },
      "IF" : [
        { "entity_has_field" : { "entity" : [ "erpal-output" ], "field" : "field_output_budget" } },
        { "entity_has_field" : { "entity" : [ "erpal-output" ], "field" : "field_output_unit" } },
        { "NOT entity_is_new" : { "entity" : [ "erpal-output" ] } },
        { "OR" : [
            { "NOT data_is" : {
                "data" : [ "erpal-output:field-output-budget" ],
                "value" : [ "erpal-output-unchanged:field-output-budget" ]
              }
            },
            { "NOT data_is" : {
                "data" : [ "erpal-output:field-output-unit" ],
                "value" : [ "erpal-output-unchanged:field-output-unit" ]
              }
            }
          ]
        }
      ],
      "DO" : [
        { "erpal_output_detach_budgets" : { "erpal_output" : [ "erpal-output-unchanged" ] } },
        { "component_rules_attach_budgets_to_output" : { "erpal_output" : [ "erpal-output" ] } },
        { "erpal_output_find_and_attach_resource" : { "erpal_output" : [ "erpal-output" ] } }
      ]
    }
  }');
  $items['rules_erpal_output_detach_budgets'] = entity_import('rules_config', '{ "rules_erpal_output_detach_budgets" : {
      "LABEL" : "ERPAL Output Detach budgets",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Erpal Output" ],
      "REQUIRES" : [ "rules", "erpal_output" ],
      "ON" : { "erpal_output_delete" : [] },
      "IF" : [
        { "entity_has_field" : { "entity" : [ "erpal-output" ], "field" : "field_output_budget" } },
        { "NOT data_is_empty" : { "data" : [ "erpal-output:field-output-budget" ] } }
      ],
      "DO" : [
        { "erpal_output_detach_budgets" : { "erpal_output" : [ "erpal-output" ] } }
      ]
    }
  }');
  return $items;
}
