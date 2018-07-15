<?php

/**
 * @file
 * Class definition of ERPALFeedsOutputProcessor.
 */

/**
 * Creates outputs from feed items.
 */
class ERPALFeedsOutputProcessor extends FeedsProcessor {
  /**
   * Define entity type.
   */
  public function entityType() {
    return 'erpal_output';
  }

  /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info['label plural'] = t('Outputs');
    return $info;
  }

  /**
   * Creates a new output in memory and returns it.
   */
  protected function newEntity(FeedsSource $source) {

    $values = array(
      'type' => $this->config['output_type'],
      'log' => t('Imported by feeds.'),
      'uid' => $this->config['author'],
    );

    return entity_create('erpal_output', $values);
  }

  /**
   * Loads an existing output.
   *
   * If the method is not FEEDS_UPDATE_EXISTING, only the output table will
   * be loaded, foregoing the erpal_output_load API for better performance.
   */
  protected function entityLoad(FeedsSource $source, $output_id) {
    if ($this->config['update_existing'] == FEEDS_UPDATE_EXISTING) {
      $output = erpal_output_load($output_id, TRUE);
    }
    else {
      // We're replacing the existing output. Only save the necessary.
      $output = db_query("SELECT created, output_id, type FROM {erpal_output} WHERE output_id = :output_id", array(':output_id' => $output_id))->fetchObject();
      $output->uid = $this->config['author'];
    }
    // Populate properties that are set by output_object_prepare().
    if ($this->config['update_existing'] == FEEDS_UPDATE_EXISTING) {
      $output->log = 'Updated by ERPALFeedsOutputProcessor';
    }
    else {
      $output->log = 'Replaced by ERPALFeedsOutputProcessor';
    }
    return $output;
  }

  /**
   * Save an output.
   */
  public function entitySave($entity) {
    erpal_output_save($entity);
  }

  /**
   * Delete a series of outputs.
   */
  protected function entityDeleteMultiple($output_ids) {
    erpal_output_delete_multiple($output_ids);
  }

  /**
   * Implement expire().
   *
   * @todo: move to processor stage?
   */
  public function expire($time = NULL) {
    if ($time === NULL) {
      $time = $this->expiryTime();
    }
    if ($time == FEEDS_EXPIRE_NEVER) {
      return;
    }
    $count = $this->getLimit();
    $outputs = db_query_range("SELECT a.output_id FROM {erpal_output} a JOIN {feeds_item} fi ON fi.entity_type = 'erpal_output' AND a.output_id = fi.entity_id WHERE fi.id = :id AND c.created < :created", 0, $count, array(':id' => $this->id, ':created' => REQUEST_TIME - $time));
    $output_ids = array();
    foreach ($outputs as $output) {
      $output_ids[$output->output_id] = $output->output_id;
    }
    $this->entityDeleteMultiple($output_ids);
    if (db_query_range("SELECT 1 FROM {erpal_output} a JOIN {feeds_item} fi ON fi.entity_type = 'erpal_output' AND a.output_id = fi.entity_id WHERE fi.id = :id AND a.created < :created", 0, 1, array(':id' => $this->id, ':created' => REQUEST_TIME - $time))->fetchField()) {
      return FEEDS_BATCH_ACTIVE;
    }
    return FEEDS_BATCH_COMPLETE;
  }

  /**
   * Return expiry time.
   */
  public function expiryTime() {
    return $this->config['expire'];
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    $types = erpal_output_type_get_names();
    $type = key($types);
    return array(
      'output_type' => $type,
      'expire' => FEEDS_EXPIRE_NEVER,
      'author' => 0,
    ) + parent::configDefaults();
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $types = erpal_output_type_get_names();
    $form = parent::configForm($form_state);
    $form['output_type'] = array(
      '#type' => 'select',
      '#title' => t('Output type'),
      '#description' => t('Select the type of outputs to be created.'),
      '#options' => $types,
      '#default_value' => $this->config['output_type'],
    );
    $author = user_load($this->config['author']);
    $form['author'] = array(
      '#type' => 'textfield',
      '#title' => t('Author'),
      '#description' => t('Select the author of the outputs to be created - leave empty to assign "anonymous".'),
      '#autocomplete_path' => 'user/autocomplete',
      '#default_value' => empty($author->name) ? 'anonymous' : check_plain($author->name),
    );
    $period = drupal_map_assoc(array(FEEDS_EXPIRE_NEVER, 3600, 10800, 21600,
      43200, 86400, 259200, 604800, 2592000, 2592000 * 3, 2592000 * 6, 31536000),
      'feeds_format_expire');
    $form['expire'] = array(
      '#type' => 'select',
      '#title' => t('Expire outputs'),
      '#options' => $period,
      '#description' => t("Select after how much time outputs should be deleted. The output's published date will be used for determining the output's age, see Mapping settings."),
      '#default_value' => $this->config['expire'],
    );
    $form['update_existing']['#options'] = array(
      FEEDS_SKIP_EXISTING => 'Do not update existing outputs',
      FEEDS_REPLACE_EXISTING => 'Replace existing outputs',
      FEEDS_UPDATE_EXISTING => 'Update existing outputs (slower than replacing them)',
    );
    return $form;
  }

  /**
   * Override parent::configFormValidate().
   */
  public function configFormValidate(&$values) {
    if ($author = user_load_by_name($values['author'])) {
      $values['author'] = $author->uid;
    }
    else {
      $values['author'] = 0;
    }
  }

  /**
   * Reschedule if expiry time changes.
   */
  public function configFormSubmit(&$values) {
    if ($this->config['expire'] != $values['expire']) {
      feeds_reschedule($this->id);
    }
    parent::configFormSubmit($values);
  }

  /**
   * Override setTargetElement to operate on a target item that is an output.
   */
  public function setTargetElement(FeedsSource $source, $target_output, $target_element, $value) {
    switch ($target_element) {
      case 'created':
        $target_output->created = feeds_to_unixtime($value, REQUEST_TIME);
        break;

      case 'feeds_source':
        // Get class of the feed output importer's fetcher and set the source
        // property. See feeds_output_update() for $output->feeds storage.
        if ($id = feeds_get_importer_id($this->config['output_type'])) {
          $class = get_class(feeds_importer($id)->fetcher);
          $target_output->feeds[$class]['source'] = $value;
          // This effectively suppresses 'import on submission' feature.
          // See feeds_output_insert().
          $target_output->feeds['suppress_import'] = TRUE;
        }
        break;

      default:
        parent::setTargetElement($source, $target_output, $target_element, $value);
        break;
    }
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $types = erpal_output_types();
    $type = $types[$this->config['output_type']];
    $targets = parent::getMappingTargets();

    $targets += array(
      'title' => array(
        'name' => t('Title'),
        'description' => t('The title of the ERPAL output.'),
        'optional_unique' => TRUE,
      ),
      'output_id' => array(
        'name' => t('Output ID'),
        'description' => t('The output_id of the output. NOTE: use this feature with care, output ids are usually assigned by Drupal.'),
        'optional_unique' => TRUE,
      ),
      'uid' => array(
        'name' => t('User ID'),
        'description' => t('The Drupal user ID of the output author.'),
      ),
      'created' => array(
        'name' => t('Created date'),
        'description' => t('The UNIX time when a output has been created.'),
      ),
    );

    // If the target content type is a Feed output, expose its source field.
    if ($id = feeds_get_importer_id($this->config['output_type'])) {
      $name = feeds_importer($id)->config['name'];
      $targets['feeds_source'] = array(
        'name' => t('Feed source'),
        'description' => t('The output type created by this processor is a Feed Node, it represents a source itself. Depending on the fetcher selected on the importer "@importer", this field is expected to be for example a URL or a path to a file.', array('@importer' => $name)),
        'optional_unique' => TRUE,
      );
    }

    // Let other modules expose mapping targets.
    self::loadMappers();
    feeds_alter('feeds_processor_targets', $targets, 'erpal_output', $this->config['output_type']);

    return $targets;
  }

  /**
   * Get output_id of an existing feed item output if available.
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result) {
    if ($output_id = parent::existingEntityId($source, $result)) {
      return $output_id;
    }

    // Iterate through all unique targets and test whether they do already
    // exist in the database.
    foreach ($this->uniqueTargets($source, $result) as $target => $value) {
      switch ($target) {
        case 'output_id':
          $output_id = db_query("SELECT output_id FROM {erpal_output} WHERE output_id = :output_id", array(':output_id' => $value))->fetchField();
          break;

        case 'feeds_source':
          if ($id = feeds_get_importer_id($this->config['output_type'])) {
            $output_id = db_query("SELECT fs.feed_output_id FROM {erpal_output} c JOIN {feeds_source} fs ON c.output_id = fs.feed_output_id WHERE fs.id = :id AND fs.source = :source", array(':id' => $id, ':source' => $value))->fetchField();
          }
          break;
      }
      if ($output_id) {
        // Return with the first output_id found.
        return $output_id;
      }
    }
    return 0;
  }
}
