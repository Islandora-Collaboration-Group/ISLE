<?php

/**
 * @file
 * Views filter handler for webform submission workflow states.
 */

class webform_workflow_views_handler_filter_state extends views_handler_filter_in_operator {

  protected $webform;

  /**
   * Overrides parent::get_value_options().
   */
  public function get_value_options() {
    if (isset($this->value_options)) {
      return $this->value_options;
    }
    $options = array(
      '[EMPTY]' => t('None'),
    );
    // Fetch the states for the specified webform, if possible.
    $node = $this->getWebform();
    if ($node) {
      $states = webform_workflow_get_available_states($node);
      foreach ($states as $state) {
        $options[$state->wsid] = $state->label;
      }
    }
    // If the view is being edited, and no webform node is specified, then
    // attempt to fetch a list of states from all forms.
    elseif (strpos(current_path(), 'admin/structure/views') === 0) {
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', 'webform_workflow_state')
        ->propertyOrderBy('wsid')
        ->range(0, 25);
      $result = $query->execute();
      if ($result && isset($result['webform_workflow_state'])) {
        $states = entity_load('webform_workflow_state', array_keys($result['webform_workflow_state']));
        foreach ($states as $state) {
          $options[$state->wsid] = $state->label;
        }
      }
    }
    $this->value_options = $options;
    return $options;
  }

  /**
   * Overrides parent::query().
   */
  public function query() {
    // If the selected option is 'None' (with the value '[EMPTY]'), change the
    // filter operator from 'in' to 'empty'.
    if (strpos($this->operator, 'in') !== FALSE
      && is_array($this->value)
      && count($this->value) == 1
      && reset($this->value) == '[EMPTY]') {
      $this->operator = str_replace('in', 'empty', $this->operator);
    }
    parent::query();
  }

  /**
   * Overrides parent::option_definition().
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['webform_nid'] = array('default' => NULL);
    return $options;
  }

  /**
   * Overrides parent::options_form().
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['webform_nid'] = array(
      '#type' => 'textfield',
      '#title' => t('Webform node ID'),
      '#description' => t('The webform to use to get the list of state options. If left blank, the webform could be automatically determined (in some circumstances).'),
      '#default_value' => $this->options['webform_nid'],
      '#element_validate' => array('element_validate_integer_positive'),
    );
  }

  /**
   * Overrides parent::value_form().
   */
  public function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);
    $colors = array();
    $wsids = array_keys($form['value']['#options']);
    if ($wsids) {
      foreach (entity_load('webform_workflow_state', $wsids) as $state) {
        if ($state->color) {
          $colors['state-' . $state->wsid] = $state->color;
        }
      }
    }
    if ($colors) {
      $form['value']['#attributes']['class'] = array('webform-workflow-state-select');
      $form['value']['#attached']['js'] = array(
        drupal_get_path('module', 'webform_workflow') . '/includes/webform_workflow.js',
        array(
          'data' => array('webformWorkflow' => array('stateColors' => $colors)),
          'type' => 'setting',
        ),
      );
    }
  }

  /**
   * Get the webform node this filter applies to.
   *
   * @return object|FALSE
   *   A node object, or FALSE if no node can be found.
   */
  protected function getWebform() {
    if (!isset($this->webform)) {
      if (!empty($this->options['webform_nid'])) {
        $this->webform = node_load($this->options['webform_nid']);
      }
      elseif (isset($this->view->argument['nid']) && !empty($this->view->args)) {
        $nid = $this->view->args[$this->view->argument['nid']->position];
        $this->webform = node_load($nid);
      }
      elseif ($menu_object = menu_get_object('webform_menu')) {
        $this->webform = $menu_object;
      }
      elseif (strpos($_GET['q'], 'admin/structure/views/ajax/preview/webform_workflow_submissions') === 0) {
        if (isset($_POST['view_args'])) {
          $this->webform = node_load($_POST['view_args'][0]);
        }
        elseif (arg(7)) {
          $this->webform = node_load(arg(7));
        }
      }
      elseif ($this->view->name == 'webform_workflow_submissions' && strpos(current_path(), '/ajax') && !empty($_POST['view_args'])) {
        $this->webform = node_load($_POST['view_args'][0]);
      }
    }
    return $this->webform ? $this->webform : FALSE;
  }

}
