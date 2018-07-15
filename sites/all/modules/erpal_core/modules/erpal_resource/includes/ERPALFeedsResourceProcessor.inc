<?php

/**
 * @file
 * Class definition of ERPALFeedsResourceProcessor.
 */

/**
 * Creates resources from feed items.
 */
class ERPALFeedsResourceProcessor extends FeedsProcessor {
  /**
   * Define entity type.
   */
  public function entityType() {
    return 'erpal_resource';
  }

  /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info['label plural'] = t('Resources');
    return $info;
  }

  /**
   * Creates a new resource in memory and returns it.
   */
  protected function newEntity(FeedsSource $source) {

    $values = array(
      'type' => $this->config['resource_type'],
      'log' => t('Imported by feeds.'),
      'uid' => $this->config['author'],
    );

    return entity_create('erpal_resource', $values);
  }

  /**
   * Loads an existing resource.
   *
   * If the method is not FEEDS_UPDATE_EXISTING, only the resource table will
   * be loaded, foregoing the erpal_resource_load API for better performance.
   */
  protected function entityLoad(FeedsSource $source, $resource_id) {
    if ($this->config['update_existing'] == FEEDS_UPDATE_EXISTING) {
      $resource = erpal_resource_load($resource_id, TRUE);
    }
    else {
      // We're replacing the existing resource. Only save the necessary.
      $resource = db_query("SELECT created, resource_id, type FROM {erpal_resource} WHERE resource_id = :resource_id", array(':resource_id' => $resource_id))->fetchObject();
      $resource->uid = $this->config['author'];
    }
    // Populate properties that are set by resource_object_prepare().
    if ($this->config['update_existing'] == FEEDS_UPDATE_EXISTING) {
      $resource->log = 'Updated by ERPALFeedsResourceProcessor';
    }
    else {
      $resource->log = 'Replaced by ERPALFeedsResourceProcessor';
    }
    return $resource;
  }

  /**
   * Save an resource.
   */
  public function entitySave($entity) {
    erpal_resource_save($entity);
  }

  /**
   * Delete a series of resources.
   */
  protected function entityDeleteMultiple($resource_ids) {
    erpal_resource_delete_multiple($resource_ids);
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
    $resources = db_query_range("SELECT a.resource_id FROM {erpal_resource} a JOIN {feeds_item} fi ON fi.entity_type = 'erpal_resource' AND a.resource_id = fi.entity_id WHERE fi.id = :id AND c.created < :created", 0, $count, array(':id' => $this->id, ':created' => REQUEST_TIME - $time));
    $resource_ids = array();
    foreach ($resources as $resource) {
      $resource_ids[$resource->resource_id] = $resource->resource_id;
    }
    $this->entityDeleteMultiple($resource_ids);
    if (db_query_range("SELECT 1 FROM {erpal_resource} a JOIN {feeds_item} fi ON fi.entity_type = 'erpal_resource' AND a.resource_id = fi.entity_id WHERE fi.id = :id AND a.created < :created", 0, 1, array(':id' => $this->id, ':created' => REQUEST_TIME - $time))->fetchField()) {
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
    $types = erpal_resource_type_get_names();
    $type = key($types);
    return array(
      'resource_type' => $type,
      'expire' => FEEDS_EXPIRE_NEVER,
      'author' => 0,
    ) + parent::configDefaults();
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $types = erpal_resource_type_get_names();
    $form = parent::configForm($form_state);
    $form['resource_type'] = array(
      '#type' => 'select',
      '#title' => t('Resource type'),
      '#description' => t('Select the type of resources to be created.'),
      '#options' => $types,
      '#default_value' => $this->config['resource_type'],
    );
    $author = user_load($this->config['author']);
    $form['author'] = array(
      '#type' => 'textfield',
      '#title' => t('Author'),
      '#description' => t('Select the author of the resources to be created - leave empty to assign "anonymous".'),
      '#autocomplete_path' => 'user/autocomplete',
      '#default_value' => empty($author->name) ? 'anonymous' : check_plain($author->name),
    );
    $period = drupal_map_assoc(array(FEEDS_EXPIRE_NEVER, 3600, 10800, 21600,
      43200, 86400, 259200, 604800, 2592000, 2592000 * 3, 2592000 * 6, 31536000),
      'feeds_format_expire');
    $form['expire'] = array(
      '#type' => 'select',
      '#title' => t('Expire resources'),
      '#options' => $period,
      '#description' => t("Select after how much time resources should be deleted. The resource's published date will be used for determining the resource's age, see Mapping settings."),
      '#default_value' => $this->config['expire'],
    );
    $form['update_existing']['#options'] = array(
      FEEDS_SKIP_EXISTING => 'Do not update existing resources',
      FEEDS_REPLACE_EXISTING => 'Replace existing resources',
      FEEDS_UPDATE_EXISTING => 'Update existing resources (slower than replacing them)',
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
   * Override setTargetElement to operate on a target item that is an resource.
   */
  public function setTargetElement(FeedsSource $source, $target_resource, $target_element, $value) {
    switch ($target_element) {
      case 'created':
        $target_resource->created = feeds_to_unixtime($value, REQUEST_TIME);
        break;

      case 'feeds_source':
        // Get class of the feed resource importer's fetcher and set the source
        // property. See feeds_resource_update() for $resource->feeds storage.
        if ($id = feeds_get_importer_id($this->config['resource_type'])) {
          $class = get_class(feeds_importer($id)->fetcher);
          $target_resource->feeds[$class]['source'] = $value;
          // This effectively suppresses 'import on submission' feature.
          // See feeds_resource_insert().
          $target_resource->feeds['suppress_import'] = TRUE;
        }
        break;

      default:
        parent::setTargetElement($source, $target_resource, $target_element, $value);
        break;
    }
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $types = erpal_resource_types();
    $type = $types[$this->config['resource_type']];
    $targets = parent::getMappingTargets();

    $targets += array(
      'title' => array(
        'name' => t('Title'),
        'description' => t('The title of the ERPAL resource.'),
        'optional_unique' => TRUE,
      ),
      'resource_id' => array(
        'name' => t('Resource ID'),
        'description' => t('The resource_id of the resource. NOTE: use this feature with care, resource ids are usually assigned by Drupal.'),
        'optional_unique' => TRUE,
      ),
      'uid' => array(
        'name' => t('User ID'),
        'description' => t('The Drupal user ID of the resource author.'),
      ),
      'created' => array(
        'name' => t('Created date'),
        'description' => t('The UNIX time when a resource has been created.'),
      ),
    );

    // If the target content type is a Feed resource, expose its source field.
    if ($id = feeds_get_importer_id($this->config['resource_type'])) {
      $name = feeds_importer($id)->config['name'];
      $targets['feeds_source'] = array(
        'name' => t('Feed source'),
        'description' => t('The resource type created by this processor is a Feed Node, it represents a source itself. Depending on the fetcher selected on the importer "@importer", this field is expected to be for example a URL or a path to a file.', array('@importer' => $name)),
        'optional_unique' => TRUE,
      );
    }

    // Let other modules expose mapping targets.
    self::loadMappers();
    feeds_alter('feeds_processor_targets', $targets, 'erpal_resource', $this->config['resource_type']);

    return $targets;
  }

  /**
   * Get resource_id of an existing feed item resource if available.
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result) {
    if ($resource_id = parent::existingEntityId($source, $result)) {
      return $resource_id;
    }

    // Iterate through all unique targets and test whether they do already
    // exist in the database.
    foreach ($this->uniqueTargets($source, $result) as $target => $value) {
      switch ($target) {
        case 'resource_id':
          $resource_id = db_query("SELECT resource_id FROM {erpal_resource} WHERE resource_id = :resource_id", array(':resource_id' => $value))->fetchField();
          break;

        case 'feeds_source':
          if ($id = feeds_get_importer_id($this->config['resource_type'])) {
            $resource_id = db_query("SELECT fs.feed_resource_id FROM {erpal_resource} c JOIN {feeds_source} fs ON c.resource_id = fs.feed_resource_id WHERE fs.id = :id AND fs.source = :source", array(':id' => $id, ':source' => $value))->fetchField();
          }
          break;
      }
      if ($resource_id) {
        // Return with the first resource_id found.
        return $resource_id;
      }
    }
    return 0;
  }
}
