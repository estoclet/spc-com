<?php

/**
 * @file
 * Provides views handlers for View, Edit, Delete links.
 */

class erpal_output_handler_field_link extends views_handler_field {
  /**
   * Constructor function.
   */
  public function construct() {
    parent::construct();

    $this->additional_fields['output_id'] = 'output_id';
  }

  /**
   * Option definition function.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    return $options;
  }

  /**
   * Options form function.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }

  /**
   * Query function.
   */
  public function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * Render function.
   */
  public function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('view');
    $output_id = $this->get_value($values, 'output_id');

    return l($text, 'erpal/output/' . $output_id);
  }
}

class erpal_output_handler_field_link_edit extends erpal_output_handler_field_link {
  /**
   * Option definition function.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['redirect'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Options form function.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['redirect'] = array(
      '#type' => 'checkbox',
      '#title' => t('Redirect user'),
      '#description' => t('Add "destination" key to the link so user will be redirected on the page where he clicked on the link'),
      '#default_value' => $this->options['redirect'],
    );
  }

  /**
   * Render function.
   */
  public function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $output_id = $this->get_value($values, 'output_id');

    if ($this->options['redirect']) {
      return l($text, 'erpal/output/' . $output_id . '/edit', array('query' => array('destination' => $_GET['q'])));
    }
    else {
      return l($text, 'erpal/output/' . $output_id . '/edit');
    }
  }
}

class erpal_output_handler_field_link_delete extends erpal_output_handler_field_link_edit {
  /**
   * Render function.
   */
  public function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $output_id = $this->get_value($values, 'output_id');

    if ($this->options['redirect']) {
      return l($text, 'erpal/output/' . $output_id . '/delete', array('query' => array('destination' => $_GET['q'])));
    }
    else {
      return l($text, 'erpal/output/' . $output_id . '/delete');
    }
  }
}
