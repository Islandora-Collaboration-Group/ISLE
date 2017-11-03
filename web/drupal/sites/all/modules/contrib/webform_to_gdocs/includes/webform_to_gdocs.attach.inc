<?php

function webform_to_gdocs_attach_form($form_state, $nodedata) {

  $node=$nodedata['build_info']['args'][0];

  $form['webform_nid'] = array(
    '#type' => 'hidden',
    '#value' => $node->nid,
  );

  $form['gdoc_type'] = array(
    '#type' => 'select',
    '#title' => t('Doc Type'),
    '#options' => array(
      'spreadsheet' => 'Spreadsheet',
    ),
    '#required' => TRUE,
  );

  // Get list of current spreadsheets.
  $spreadsheets = array();
  $result = array();

  try {
    $service = _webform_to_gdocs_google_drive_get_service();
    $files = $service->files->listFiles(array('q' => "mimeType='application/vnd.google-apps.spreadsheet'"));
    $result = $files->getItems();
  } catch (Exception $e) {
    drupal_set_message($e->getMessage(), 'error');
  }

  foreach ($result as $doc) {
    $spreadsheets[$doc->id] = $doc->title;
  }

  $form['gdoc_id'] = array(
    '#type' => 'select',
    '#title' => t('Doc Name'),
    '#description' => t('Select the document to insert Webform submissions into. To stop inserting Webform submissions inso an existing Google Doc, select "None".'),
    '#options' => array('_none' => '- None -') + $spreadsheets,
  );

  // Select active spreadsheet data if it exists.
  $result = db_query("SELECT * FROM {webform_to_gdocs_webforms} WHERE nid = :nid",array(':nid' =>  $node->nid));
  $gdoc_exists=$result->fetchAssoc();

  if ($gdoc_exists) {

    $form['gdoc_id']['#default_value'] = $gdoc_exists['gdoc_name'];
    $form['gdoc_type']['#default_value'] = $gdoc_exists['gdoc_type'];

    $form['gdoc_sheet'] = array(
      '#type' => 'select',
      '#title' => 'Worksheet Name',
      '#options' => array('_none' => '- None -'),
    );

    $form['gdoc_sheet']['#default_value'] = $gdoc_exists['gdoc_sheet'];

    // Get all worksheets for this doc.
    try {
      // Ensure we have a working access token.
      _webform_to_gdocs_google_drive_get_service();
      $access_token = variable_get('webform_to_gdocs_access_token');
      $access_token = @json_decode($access_token);

      $ch = curl_init();
      $url = 'https://spreadsheets.google.com/feeds/worksheets/' . $gdoc_exists['gdoc_name'] . '/private/full';

      curl_setopt($ch, CURLOPT_URL, $url);
      curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
      curl_setopt($ch, CURLOPT_HTTPGET, 1);
      curl_setopt($ch, CURLOPT_HTTPHEADER, array(
          'Authorization: Bearer ' . $access_token->access_token,
          'Content-type: application/atom+xml'
      ));

      $result = curl_exec($ch);
      $xml = simplexml_load_string($result);
      $obj = json_decode(json_encode($xml));

      if(is_array($obj->entry))
      {
        foreach ($obj->entry as $entry) {
          $id_parts = explode('/', $entry->id);
          $id = end($id_parts);
          $form['gdoc_sheet']['#options'][$id] = $entry->title;
        }
      }
      else
      {
        $entry=$obj->entry;
                $id_parts = explode('/', $entry->id);
        $id = end($id_parts);
        $form['gdoc_sheet']['#options'][$id] = $entry->title;
      }

      $httpcode = curl_getinfo($ch, CURLINFO_HTTP_CODE);
      if ($httpcode != 200) {
        throw new Exception("Invalid response code from Google Docs API.");
      }

      curl_close($ch);

    } catch (Exception $e) {
      drupal_set_message($e->getMessage(), 'error');
    }
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' =>  'Save Settings',
  );

  return $form;
}

function webform_to_gdocs_attach_form_validate($form, &$form_state) {
 // if ($form_state['values']['gdoc_id'] != '_none' && empty($form_state['values']['gdoc_sheet'])) {
  //  form_set_error('gdoc_sheet', 'Enter a Worksheet Name to be used with the document you selected.');
 // }
}

function webform_to_gdocs_attach_form_submit($form, &$form_state) {

  $result = db_query("SELECT nid FROM {webform_to_gdocs_webforms} WHERE nid = :nid", array(':nid' => $form_state['values']['webform_nid']));

  $gdoc_exists = $result->fetchAssoc();

  if ($form_state['values']['gdoc_id'] == '_none') {

    // If a relationship already exists, delete it.
    if ($gdoc_exists) {
      db_query("DELETE FROM {webform_to_gdocs_webforms} WHERE nid = %d", $form_state['values']['webform_nid']);
      drupal_set_message('Removed Google Doc attachment for this Webform.');
    }

    return;
  }

  // Insert record.
  if (!$gdoc_exists) {

    $insert = db_insert('webform_to_gdocs_webforms');

    $fields = array(
      'nid' => $form_state['values']['webform_nid'],
      'gdoc_type' => $form_state['values']['gdoc_type'],
      'gdoc_name' => $form_state['values']['gdoc_id'],
    );

    if (!empty($form_state['values']['gdoc_sheet'])) {
      $fields['gdoc_sheet'] = $form_state['values']['gdoc_sheet'];
    }

    $insert
      ->fields($fields)
      ->execute();
  }

  // Update record.
  else {

    $update = db_update('webform_to_gdocs_webforms')
      ->condition('nid', $form_state['values']['webform_nid']);

    $fields = array(
      'gdoc_type' => $form_state['values']['gdoc_type'],
      'gdoc_name' => $form_state['values']['gdoc_id'],
    );

    if (!empty($form_state['values']['gdoc_sheet'])) {
      $fields['gdoc_sheet'] = $form_state['values']['gdoc_sheet'];
    }

    $update
      ->fields($fields)
      ->execute();
  }

  drupal_set_message('Webform attached to Google Doc.');
}
