<?php

/**
 * @file
 * Class definition of ERPALFeedsBudgetProcessor.
 */

/**
 * Creates budgets from feed items.
 */
class ERPALFeedsBudgetProcessor extends FeedsProcessor {
  /**
   * Define entity type.
   */
  public function entityType() {
    return 'erpal_budget';
  }

  /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info['label plural'] = t('Budgets');
    return $info;
  }

  /**
   * Creates a new budget in memory and returns it.
   */
  protected function newEntity(FeedsSource $source) {

    $values = array(
      'type' => $this->config['budget_type'],
      'log' => t('Imported by feeds.'),
      'uid' => $this->config['author'],
    );

    return entity_create('erpal_budget', $values);
  }

  /**
   * Loads an existing budget.
   *
   * If the method is not FEEDS_UPDATE_EXISTING, only the budget table will
   * be loaded, foregoing the erpal_budget_load API for better performance.
   */
  protected function entityLoad(FeedsSource $source, $budget_id) {
    if ($this->config['update_existing'] == FEEDS_UPDATE_EXISTING) {
      $budget = erpal_budget_load($budget_id, TRUE);
    }
    else {
      // We're replacing the existing budget. Only save the necessary.
      $budget = db_query("SELECT created, budget_id, type FROM {erpal_budget} WHERE budget_id = :budget_id", array(':budget_id' => $budget_id))->fetchObject();
      $budget->uid = $this->config['author'];
    }
    // Populate properties that are set by budget_object_prepare().
    if ($this->config['update_existing'] == FEEDS_UPDATE_EXISTING) {
      $budget->log = 'Updated by ERPALFeedsBudgetProcessor';
    }
    else {
      $budget->log = 'Replaced by ERPALFeedsBudgetProcessor';
    }
    return $budget;
  }

  /**
   * Save an budget.
   */
  public function entitySave($entity) {
    erpal_budget_save($entity);
  }

  /**
   * Delete a series of budgets.
   */
  protected function entityDeleteMultiple($budget_ids) {
    erpal_budget_delete_multiple($budget_ids);
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
    $budgets = db_query_range("SELECT a.budget_id FROM {erpal_budget} a JOIN {feeds_item} fi ON fi.entity_type = 'erpal_budget' AND a.budget_id = fi.entity_id WHERE fi.id = :id AND c.created < :created", 0, $count, array(':id' => $this->id, ':created' => REQUEST_TIME - $time));
    $budget_ids = array();
    foreach ($budgets as $budget) {
      $budget_ids[$budget->budget_id] = $budget->budget_id;
    }
    $this->entityDeleteMultiple($budget_ids);
    if (db_query_range("SELECT 1 FROM {erpal_budget} a JOIN {feeds_item} fi ON fi.entity_type = 'erpal_budget' AND a.budget_id = fi.entity_id WHERE fi.id = :id AND a.created < :created", 0, 1, array(':id' => $this->id, ':created' => REQUEST_TIME - $time))->fetchField()) {
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
    $types = erpal_budget_type_get_names();
    $type = key($types);
    return array(
      'budget_type' => $type,
      'expire' => FEEDS_EXPIRE_NEVER,
      'author' => 0,
    ) + parent::configDefaults();
  }

  /**
   * Override parent::configForm().
   */
  public function configForm(&$form_state) {
    $types = erpal_budget_type_get_names();
    $form = parent::configForm($form_state);
    $form['budget_type'] = array(
      '#type' => 'select',
      '#title' => t('Budget type'),
      '#description' => t('Select the type of budgets to be created.'),
      '#options' => $types,
      '#default_value' => $this->config['budget_type'],
    );
    $author = user_load($this->config['author']);
    $form['author'] = array(
      '#type' => 'textfield',
      '#title' => t('Author'),
      '#description' => t('Select the author of the budgets to be created - leave empty to assign "anonymous".'),
      '#autocomplete_path' => 'user/autocomplete',
      '#default_value' => empty($author->name) ? 'anonymous' : check_plain($author->name),
    );
    $period = drupal_map_assoc(array(FEEDS_EXPIRE_NEVER, 3600, 10800, 21600,
      43200, 86400, 259200, 604800, 2592000, 2592000 * 3, 2592000 * 6, 31536000),
      'feeds_format_expire');
    $form['expire'] = array(
      '#type' => 'select',
      '#title' => t('Expire budgets'),
      '#options' => $period,
      '#description' => t("Select after how much time budgets should be deleted. The budget's published date will be used for determining the budget's age, see Mapping settings."),
      '#default_value' => $this->config['expire'],
    );
    $form['update_existing']['#options'] = array(
      FEEDS_SKIP_EXISTING => 'Do not update existing budgets',
      FEEDS_REPLACE_EXISTING => 'Replace existing budgets',
      FEEDS_UPDATE_EXISTING => 'Update existing budgets (slower than replacing them)',
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
   * Override setTargetElement to operate on a target item that is an budget.
   */
  public function setTargetElement(FeedsSource $source, $target_budget, $target_element, $value) {
    switch ($target_element) {
      case 'created':
        $target_budget->created = feeds_to_unixtime($value, REQUEST_TIME);
        break;

      case 'feeds_source':
        // Get class of the feed budget importer's fetcher and set the source
        // property. See feeds_budget_update() for $budget->feeds storage.
        if ($id = feeds_get_importer_id($this->config['budget_type'])) {
          $class = get_class(feeds_importer($id)->fetcher);
          $target_budget->feeds[$class]['source'] = $value;
          // This effectively suppresses 'import on submission' feature.
          // See feeds_budget_insert().
          $target_budget->feeds['suppress_import'] = TRUE;
        }
        break;

      default:
        parent::setTargetElement($source, $target_budget, $target_element, $value);
        break;
    }
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $types = erpal_budget_types();
    $type = $types[$this->config['budget_type']];
    $targets = parent::getMappingTargets();

    $targets += array(
      'title' => array(
        'name' => t('Title'),
        'description' => t('The title of the ERPAL budget.'),
        'optional_unique' => TRUE,
      ),
      'budget_id' => array(
        'name' => t('Budget ID'),
        'description' => t('The budget_id of the budget. NOTE: use this feature with care, budget ids are usually assigned by Drupal.'),
        'optional_unique' => TRUE,
      ),
      'uid' => array(
        'name' => t('User ID'),
        'description' => t('The Drupal user ID of the budget author.'),
      ),
      'created' => array(
        'name' => t('Created date'),
        'description' => t('The UNIX time when a budget has been created.'),
      ),
    );

    // If the target content type is a Feed budget, expose its source field.
    if ($id = feeds_get_importer_id($this->config['budget_type'])) {
      $name = feeds_importer($id)->config['name'];
      $targets['feeds_source'] = array(
        'name' => t('Feed source'),
        'description' => t('The budget type created by this processor is a Feed Node, it represents a source itself. Depending on the fetcher selected on the importer "@importer", this field is expected to be for example a URL or a path to a file.', array('@importer' => $name)),
        'optional_unique' => TRUE,
      );
    }

    // Let other modules expose mapping targets.
    self::loadMappers();
    feeds_alter('feeds_processor_targets', $targets, 'erpal_budget', $this->config['budget_type']);

    return $targets;
  }

  /**
   * Get budget_id of an existing feed item budget if available.
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result) {
    if ($budget_id = parent::existingEntityId($source, $result)) {
      return $budget_id;
    }

    // Iterate through all unique targets and test whether they do already
    // exist in the database.
    foreach ($this->uniqueTargets($source, $result) as $target => $value) {
      switch ($target) {
        case 'budget_id':
          $budget_id = db_query("SELECT budget_id FROM {erpal_budget} WHERE budget_id = :budget_id", array(':budget_id' => $value))->fetchField();
          break;

        case 'feeds_source':
          if ($id = feeds_get_importer_id($this->config['budget_type'])) {
            $budget_id = db_query("SELECT fs.feed_budget_id FROM {erpal_budget} c JOIN {feeds_source} fs ON c.budget_id = fs.feed_budget_id WHERE fs.id = :id AND fs.source = :source", array(':id' => $id, ':source' => $value))->fetchField();
          }
          break;
      }
      if ($budget_id) {
        // Return with the first budget_id found.
        return $budget_id;
      }
    }
    return 0;
  }
}
