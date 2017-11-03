<?php

/**
 * Configure the webform_to_gdocs module.
 */
function webform_to_gdocs_configure_form($form_state) {

  $form['client_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Client ID'),
    '#default_value' => variable_get('webform_to_gdocs_client_id', ''),
  );

  $form['client_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Client Secret'),
    '#default_value' => variable_get('webform_to_gdocs_client_secret', ''),
  );

  $form['app_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Application Name'),
    '#default_value' => variable_get('webform_to_gdocs_app_name', ''),
  );

  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Drive Account Email Address'),
    '#default_value' => variable_get('webform_to_gdocs_email', ''),
  );

  $form['error_email_address'] = array(
    '#type' => 'textfield',
    '#title' => t('Error Email Address'),
    '#description' => t('Send an email to this address if an error is encountered while attempting to insert a Google Doc during a webform submission.'),
    '#default_value' => variable_get('webform_to_gdocs_error_email_address', ''),
  );

  if (strlen($form['client_id']['#default_value']) > 0 && strlen($form['client_secret']['#default_value']) > 0) {

    $form['oauth_markup'] = array('#markup' => '');
    $form['oauth_markup']['#markup'] .= '<script type="text/javascript" src="https://apis.google.com/js/client.js"></script>';
    $form['oauth_markup']['#markup'] .= '<script>
      function handleClientLoad() {
        gapi.auth.authorize({
          "client_id": "' . $form['client_id']['#default_value'] . '",
          "scope": ' . json_encode(_webform_to_gdocs_google_api_scopes()) . ',
          "approval_prompt": "force",
          "access_type": "offline",
          "response_type": "code",
          "redirect_uri": "' . url('webform_to_gdocs/oauth2callback', array('absolute' => true)) . '"
        });
      }
    </script>';

    $form['oauth'] = array(
      '#type' => 'button',
      '#value' => 'Authorize Google Drive Account',
      '#default_value' => variable_get('webform_to_gdocs_access_token', ''),
      '#attributes' => array('onclick' => 'handleClientLoad(); return false;'),
    );

    if (strlen($form['oauth']['#default_value']) > 0) {
      $form['oauth']['#value'] = "Re-Authorize Google Drive Account";
      $form['oauth']['#description'] = "You have previously authorized an account.";
    }

  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' =>  'Save Settings',
  );

  return $form;
}

function webform_to_gdocs_configure_form_submit($form, &$form_state) {
  variable_set('webform_to_gdocs_client_id', $form_state['values']['client_id']);
  variable_set('webform_to_gdocs_client_secret', $form_state['values']['client_secret']);
  variable_set('webform_to_gdocs_app_name', $form_state['values']['app_name']);
  variable_set('webform_to_gdocs_email', $form_state['values']['email']);
  variable_set('webform_to_gdocs_error_email_address', $form_state['values']['error_email_address']);
  drupal_set_message('Settings updated.');
}
