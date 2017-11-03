<?php

/**
 * @file
 * Hooks provided by Islandora Webform Ingest.
 */

/**
 * Allow modules to alter an islandora xml metadata ingest preview form.
 *
 * @param array $form
 *   The drupal form, passed by reference
 *
 * @param array $form_state
 *   The drupal form state
 *
 * @param object $submission
 *   The webform submission record as retrieved from the database
 */
function hook_iwi_set_form_alter(&$form, $form_state, $submission) {

  // Use of this hook requires having a lot of specific understanding of both
  // the webform that is serving as the data source, and the MODS form, which
  // is the destination.
  //
  // The basic idea is that you inspect the $submission variable for the
  // raw user input, do whatever modifications you want to do to that input,
  // and then use iwi_set_form_element_byhash to insert that data into a field
  // in the MODS form.

  // Restrict to a particular webform
  if ($submission->nid == 14) {

    // Adding a class of 'submitted-value' results in the field we are populating
    // being highlighted on the form as a populated value.
    $attributes = array(
      'class' => array(
        'submitted-value',
      ),
    );

    // We're going to be re-creating the tabs in $form['nameTabs']. Here we get
    // a list of the initial nameTabs so we can remove them later.
    $old_name_tabs = element_children($form['nameTabs']);

    // We have two fields in our webform, one is a comma-separated list of names
    // that will be given a  nameType of "corporate", and another for names that
    // will be given a nameType of "personal".
    //
    // Corporations.
    if (!empty($submission->data[1][0])) {

      // Explode the comma-separated values.
      $names = array_map('trim', explode(',', $submission->data[1][0]));
      foreach ($names as $name) {

        // We're going to be creating a new name tab and we need to assign a unique
        // random offset (array index) so that when we populate other fields on
        // that tab, iwi_set_form_element_byhash will use the same tab as we are
        // using for the first field.
        $offset = md5(uniqid('offset', TRUE));

        // First we'll do the nameType.
        //
        // The $input_value argument is an array that must have, at a minimum,
        // a '#default_value'. We also give it the attributes, including the class
        // of 'submitted-value'.
        $input_value = array(
          '#default_value' => 'corporate',
          '#attributes' => $attributes,
        );
        // See the function definition for iwi_set_form_element_byhash for an
        // explanation of these arguments. To find the MODS form element path, you
        // can look at the paths shown for the fields on the webform ingest field
        // mappings form for this webform field.
        iwi_set_form_element_byhash($form, $form_state, 'nameTabs:namePanel:nameSet:nameType', $input_value, 'append', $offset);

        // Now we'll do the namePart (the name itself in this MODS form's schema).
        $input_value = array(
          '#default_value' => $name,
          '#attributes' => $attributes,
        );
        iwi_set_form_element_byhash($form, $form_state, 'nameTabs:namePanel:nameSet:namePart', $input_value, 'append', $offset);
      }
    }

    // Similar to the corporate names, but with a different component from the
    // webform submission to get the private names.
    //
    // Private individuals.
    if (!empty($submission->data[5][0])) {
      $names = array_map('trim', explode(',', $submission->data[5][0]));
      foreach ($names as $name) {
        $offset = md5(uniqid('offset', TRUE));
        $input_value = array(
          '#default_value' => 'personal',
          '#attributes' => $attributes,
        );
        iwi_set_form_element_byhash($form, $form_state, 'nameTabs:namePanel:nameSet:nameType', $input_value, 'append', $offset);
        $input_value = array(
          '#default_value' => $name,
          '#attributes' => $attributes,
        );
        iwi_set_form_element_byhash($form, $form_state, 'nameTabs:namePanel:nameSet:namePart', $input_value, 'append', $offset);
      }
    }

    // Now we have to remove the original nameTabs, which we saved above. We have
    // to remove it from both the form_state's objective_form_storage and from
    // the form itself. If we didn't remove them from the form_state, they would
    // reappear when the user clicks to add a tab.
    foreach ($old_name_tabs as $old_name_tab) {
      // Get the objective form hash from the drupal form element.
      $hash = $form['nameTabs'][$old_name_tab]['#hash'];
      // Find that element in the objective form storage and orphan (remove) it.
      $form_state['storage']['objective_form_storage']['root']->findElement($hash)->orphan();
      // Finally, we can safely delete the drupal form element.
      unset($form['nameTabs'][$old_name_tab]);
    }
  }
}

/**
 * Include TRANSCRIPT in IWI target datastreams.
 *
 * return an array with a minimum of 'dsid' and 'mime' values.
 */
function hook_iwi_ingestible_datastreams_alter(&$datastreams, $object, $mimetypes) {
  $datastreams['TRANSCRIPT (text/plain)'] = array(
    'dsid' => 'TRANSCRIPT',
    'mime' => 'text/plain',
  );
}

/**
 * Allow modules to respond to ingesting a webform submission.
 *
 * @param $submission
 * @param $object
 */
function apwa_iw_islandora_webform_submission_ingested($submission, $object) {

  // Set webform_workflow state to "ingested", if possible.
  if (module_exists('webform_workflow')) {
    $node = node_load($submission->nid);
    $states = webform_workflow_get_available_states($node);
    foreach($states as $state) {
      if (trim(strtolower($state->label)) == 'ingested') {
        webform_workflow_transition($submission, $state);
        break;
      }
    }
  }
}
