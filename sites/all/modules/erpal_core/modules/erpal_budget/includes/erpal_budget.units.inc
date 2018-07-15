<?php

/**
 * @file
 * Define functions for working with units module.
 */

/**
 * Import default units.
 */
function _erpal_budget_units_import() {
  // Get default units.
  $units = _erpal_budget_units_default();
  foreach ($units as $measure_name => $measure) {
    $query = new EntityFieldQuery();
    $result = $query->entityCondition('entity_type', 'units_measure')
      ->propertyCondition('measure', $measure_name)
      ->execute();

    // No such entity has been created in DB yet. So we create one.
    if (empty($result)) {
      $entity = entity_create('units_measure', array(
        'measure' => $measure_name,
      ));

      // Fill properties.
      foreach (array('label', 'description') as $k) {
        $entity->{$k} = $measure[$k];
      }
      entity_save('units_measure', $entity);

      // Create units for each measure
      foreach ($measure['units'] as $unit_name => $unit) {

        // No such entity has been created in DB yet. So we create one.
        $entity = entity_create('units_unit', array(
          'measure' => $measure_name,
          'machine_name' => $unit_name,
        ));

        // Fill properties.
        foreach (array('label', 'symbol', 'factor', 'description') as $k) {
          $entity->{$k} = $unit[$k];
        }
        entity_save('units_measure', $entity);
      }
    }
  }
}

/**
 * Returns default units
 */
function _erpal_budget_units_default() {
  return array(
    'time' => array(
      'label' => 'Time',
      'description' => 'Default time units',
      'units' => array(
        'hour' => array(
          'label' => 'Hour',
          'symbol' => 'h',
          'factor' => 3600,
          'description' => 'Hour',
        ),
        'day' => array(
          'label' => 'Day',
          'symbol' => 'd',
          'factor' => 28800,
          'description' => 'Day',
        ),
        'week' => array(
          'label' => 'Week',
          'symbol' => 'w',
          'factor' => 144000,
          'description' => 'Week',
        ),
        'month' => array(
          'label' => 'Month',
          'symbol' => 'm',
          'factor' => 633600,
          'description' => 'Month',
        ),
      ),
    ),
    'money' => array(
      'label' => 'Money',
      'description' => 'Default money units',
      'units' => array(
        'usd' => array(
          'label' => 'Dollar',
          'symbol' => 'usd',
          'factor' => 1,
          'description' => 'Dollar',
        ),
        'eur' => array(
          'label' => 'Euro',
          'symbol' => 'eur',
          'factor' => 1.1,
          'description' => 'Euro',
        ),
      ),
    ),
  );
}
