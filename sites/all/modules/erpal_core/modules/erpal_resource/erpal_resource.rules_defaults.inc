<?php

/**
 * @file
 * erpal_resource.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function erpal_resource_default_rules_configuration() {
  $items = array();
  $resource_bundles = variable_get('erpal_resource_bundles', array());
  if (!empty($resource_bundles)) {
    foreach ($resource_bundles as $resource_bundle) {
      list($entity_type, $entity_bundle) = explode(ERPAL_RESOURCE_DELIMITER, $resource_bundle);
      $entity = entity_get_info($entity_type);
      $items['rules_' . $resource_bundle] = entity_import('rules_config', '{ "rules_' . $resource_bundle . '" : {
          "LABEL" : "Auto create resource for ' . $entity['label'] . ' - ' . $entity['bundles'][$entity_bundle]['label'] . '",
          "PLUGIN" : "reaction rule",
          "WEIGHT" : "-5",
          "OWNER" : "rules",
          "TAGS" : [ "Erpal Resource" ],
          "REQUIRES" : [ "rules", "erpal_resource" ],
          "ON" : { "' . $entity_type . '_insert" : [] },
          "IF" : [
            { "entity_is_of_bundle" : {
                "entity" : [ "' . str_replace('_', '-', $entity_type) . '" ],
                "type" : "' . $entity_type . '",
                "bundle" : { "value" : { "' . $entity_bundle . '" : "' . $entity_bundle . '" } }
              }
            }
          ],
          "DO" : [{ "erpal_resource_create" : [] }]
        }
      }');

    }
  }
  return $items;
}
