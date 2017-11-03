<?php

function webform_to_gdocs_oauth2callback() {

    if (!isset($_GET['code'])) {
        return drupal_not_found();
    }

    $client_id = variable_get('webform_to_gdocs_client_id', '');
    $client_secret = variable_get('webform_to_gdocs_client_secret', '');
    $app_name = variable_get('webform_to_gdocs_app_name', '');

    if (!$client_id || !$client_secret || !$app_name) {
        return drupal_not_found();
    }

    $client = new Google_Client();
    $client->setClientId($client_id);
    $client->setClientSecret($client_secret);
    $client->setRedirectUri(url('webform_to_gdocs/oauth2callback', array('absolute' => true)));
    $client->authenticate($_GET['code']);

    $access_token = $client->getAccessToken();
    variable_set('webform_to_gdocs_access_token', $access_token);

    drupal_set_message('Account authorized.');
    print '<script>window.opener.location = window.opener.location; window.close()</script>';
    drupal_exit();
}
