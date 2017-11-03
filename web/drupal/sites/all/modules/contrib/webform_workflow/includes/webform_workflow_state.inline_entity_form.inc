<?php

/**
 * @file
 * Defines the inline entity form controller for webform workflow states.
 */

class WebformWorkflowStateInlineEntityFormController extends EntityInlineEntityFormController {

  /**
   * Overrides EntityInlineEntityFormController::labels().
   */
  public function labels() {
    $labels = array(
      'singular' => t('state'),
      'plural' => t('states'),
    );
    return $labels;
  }

  /**
   * Overrides EntityInlineEntityFormController::entityForm().
   */
  public function entityForm($entity_form, &$form_state) {
    $entity_form = parent::entityForm($entity_form, $form_state);
    form_load_include($form_state, 'inc', 'webform_workflow', 'includes/webform_workflow_state.forms');
    webform_workflow_state_form_attach($entity_form, $form_state, $entity_form['#entity']);
    return $entity_form;
  }

  /**
   * Overrides EntityInlineEntityFormController::tableFields().
   */
  public function tableFields($bundles) {
    $fields = array();
    $fields['label'] = array(
      'type' => 'property',
      'label' => t('Name'),
      'weight' => 1,
    );
    $fields['color'] = array(
      'type' => 'property',
      'label' => t('Color'),
      'weight' => 2,
      // For the 'formatter callback' key to work, this patch to Inline Entity
      // Form is required: https://www.drupal.org/node/2097583#comment-8997683
      'formatter callback' => array($this, 'formatColor'),
    );
    $fields['ww_state_notify_users'] = array(
      'type' => 'field',
      'label' => t('Users to notify'),
      'weight' => 3,
    );
    $fields['ww_state_notify_os'] = array(
      'type' => 'field',
      'label' => t('Notify submitter'),
      'weight' => 4,
    );
    return $fields;
  }

  /**
   * Formatter callback for a state color.
   *
   * @param string $color
   *   The color (as a machine name).
   *
   * @return string
   *   An HTML string.
   */
  public function formatColor($color) {
    $color_labels = webform_workflow_state_color_options_list();
    return theme('webform_workflow_state_color', array(
      'content' => $color_labels[$color],
      'color' => $color,
    ));
  }

}
