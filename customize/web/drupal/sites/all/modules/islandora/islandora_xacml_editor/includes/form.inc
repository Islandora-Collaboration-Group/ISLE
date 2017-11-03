<?php
/**
 * @file
 * Houses the forms used in the Islandora XACML Editor.
 */

/**
 * The XACML editing form.
 */
function islandora_xacml_editor_form($form, &$form_state, $object) {
  drupal_add_css(drupal_get_path('module', 'islandora_xacml_editor') . '/css/islandora_xacml_editor.css');
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/breadcrumb');

  drupal_set_title(t('Islandora XACML Editor'));

  if (!isset($form_state['islandora_xacml'])) {
    $form_state['islandora_xacml'] = array();
    $form_state['islandora_xacml']['pid'] = $object->id;
  }

  if (!islandora_is_valid_pid($object->id)) {
    drupal_not_found();
    exit();
  }

  if (!$object) {
    drupal_not_found();
    exit();
  }

  drupal_set_breadcrumb(islandora_get_breadcrumbs($object));

  // Get the user list.
  $users = array();
  $result = db_query('SELECT u.uid, u.name FROM {users} u');
  foreach ($result as $user) {
    $user->uid == 0 ? $users['anonymous'] = 'anonymous' : $users[$user->name] = $user->name;
    if ($user->uid == 1) {
      $admin_user = $user->name;
      $form_state['islandora_xacml']['admin_user'] = $user->name;
    }
  }

  // Current user.
  $form_state['islandora_xacml']['current_user'] = $GLOBALS['user']->name;

  // Get role list.
  $roles = array();
  $result = db_query('SELECT r.rid, r.name FROM {role} r');
  foreach ($result as $role) {
    $role->rid == 0 ? $roles['anonymous'] = 'anonymous' : $roles[$role->name] = $role->name;
  }

  $new_xacml = FALSE;

  if (isset($object['POLICY'])) {
    // Some basic error handling.
    try {
      $xacml = new Xacml($object['POLICY']->content);

      if ($xacml->managementRule->isPopulated() && !$xacml->managementRule->validateDefaultMethods()) {
        drupal_set_message(t('The management XACML policy is not valid.'), 'error');
      }

      if ($xacml->viewingRule->isPopulated() && !$xacml->viewingRule->validateDefaultMethods()) {
        drupal_set_message(t('The viewing XACML policy is not valid.'), 'error');
      }

      if ($xacml->datastreamRule->isPopulated() && !$xacml->datastreamRule->validateDefaultMethods()) {
        drupal_set_message(t('The datastream XACML policy is not valid.'), 'error');
      }
    }
    catch (XacmlException $e) {
      watchdog('islandora_xacml_editor', 'Exception in Islandora Xacml: @message', array('@message', $e->getMessage()), array(), WATCHDOG_ERROR);
      drupal_set_message($e->getMessage());
      drupal_set_message(t("Xacml Parser failed to parse @object_pid. It is likely this POLICY wasn't written by the islandora XACML editor, it will have to be modified by hand.",
                          array("@object_pid" => $object->id))
                        );
      drupal_not_found();
      exit();
    }
  }
  else {
    $new_xacml = TRUE;
    $xacml = new Xacml();
  }

  $form = array(
    '#tree' => TRUE,
  );

  $form['access_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable XACML Restrictions on Object Viewing'),
    '#default_value' => $xacml->viewingRule->isPopulated(),
  );

  $form['access'] = array(
    '#type' => 'fieldset',
    '#title' => t('Object Viewing'),
    '#tree' => TRUE,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="access_enabled"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['access']['users'] = array(
    '#type' => 'select',
    '#title' => t('Allowed Users'),
    '#default_value' => islandora_xacml_editor_retrieve_users($xacml, $new_xacml, 'viewing'),
    '#options' => $users,
    '#multiple' => TRUE,
    '#size' => 10,
    '#prefix' => '<div class="islandora_xacml_selects">',
  );

  $form['access']['roles'] = array(
    '#type' => 'select',
    '#title' => t('Allowed Roles'),
    '#options' => $roles,
    '#multiple' => TRUE,
    '#size' => 10,
    '#default_value' => islandora_xacml_editor_retrieve_roles($xacml, $new_xacml, 'viewing'),
    '#suffix' => '</div>',
  );

  // Grab original value used in comparisons.
  if (!isset($form_state['islandora_xacml']['access']['enabled'])) {
    $form_state['islandora_xacml']['access']['enabled'] = $form['access_enabled']['#default_value'];
  }

  $form['manage_enabled'] = array(
    '#weight' => -2,
    '#type' => 'checkbox',
    '#title' => t('Enable XACML Restrictions on Object Management'),
    '#default_value' => $xacml->managementRule->isPopulated(),
  );

  $form['manage'] = array(
    '#weight' => -1,
    '#type' => 'fieldset',
    '#title' => t('Object Management'),
    '#description' => 'Select the Users and Roles that are allowed to manage this object. These users will also be able to view the object even if not explicitly allowed to in the object access section. WARNING: If you unselect yourself you will be locked out of the object.',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="manage_enabled"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['manage']['users'] = array(
    '#type' => 'select',
    '#title' => t('Users'),
    '#options' => $users,
    '#default_value' => islandora_xacml_editor_retrieve_users($xacml, $new_xacml, 'management'),
    '#multiple' => TRUE,
    '#size' => 10,
    '#prefix' => '<div class="islandora_xacml_selects">',
  );

  $form['manage']['roles'] = array(
    '#type' => 'select',
    '#title' => t('Roles'),
    '#default_value' => islandora_xacml_editor_retrieve_roles($xacml, $new_xacml, 'management'),
    '#options' => $roles,
    '#multiple' => TRUE,
    '#size' => 10,
    '#suffix' => '</div>',
  );

  $form['dsid_mime_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable XACML Restrictions on DSIDs and MIME types'),
    '#default_value' => $xacml->datastreamRule->isPopulated(),
  );

  $form['dsid_mime'] = array(
    '#type' => 'fieldset',
    '#title' => t('Datastreams and MIME types'),
    '#collapsible' => TRUE,
    '#tree' => TRUE,
    '#collapsed' => FALSE,
    '#states' => array(
      'visible' => array(
        ':input[name="dsid_mime_enabled"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Grab the original value to be used in comparisons.
  if (!isset($form_state['islandora_xacml']['dsid_mime']['enabled'])) {
    $form_state['islandora_xacml']['dsid_mime']['enabled'] = $form['dsid_mime_enabled']['#default_value'];
  }

  // Call CModel oriented variants first.
  $query_choices = array();
  foreach (islandora_build_hook_list('islandora_xacml_editor_child_query', $object->models) as $hook) {
    $temp = module_invoke_all($hook, $object);
    if (!empty($temp)) {
      $query_choices = array_merge_recursive($query_choices, $temp);
      // We are doing this to handle the "flat" use case where we are not
      // recursing more than one level. This is a unique case so this is the
      // easiest way to handle it.
      if (isset($query_choices['all_children'])) {
        $query_choices['flat_collection'] = $query_choices['all_children'];
        $query_choices['flat_collection']['description'] = t('All immediate children of the collection (shallow traversal)');
        $query_choices['flat_collection']['restricted_cmodels'] = array('islandora:collectionCModel');
      }
    }
  }

  if (!empty($query_choices)) {
    // The "newchildren" option is applied automatically through ingest steps.
    $update_options = array('newchildren' => t('New children of this object.'));
    foreach ($query_choices as $key => $query) {
      $form_state['islandora_xacml']['query_choices'][$key] = $query;
      $update_options[$key] = $query['description'];
    }
    $form['update_options'] = array(
      '#type' => 'select',
      '#title' => t('What items would you like to apply this policy to?'),
      '#default_value' => 'newchildren',
      '#options' => $update_options,
    );

    if (isset($form_state['islandora_xacml']['child_option'])) {
      $form['update_options']['#value'] = $form_state['islandora_xacml']['child_option'];
    }

    $form['update_options_warning'] = array(
      '#type' => 'container',
      '#states' => array(
        'invisible' => array(
          ':input[name="update_options"]' => array('value' => 'newchildren'),
        ),
      ),
      'markup' => array(
        '#markup' => t('<strong>Warning:</strong> Overriding existing policies can break the expected behavoir of other modules which rely on full control of an objects POLICY. If any children have embargoes set by <em>Islandora Scholar Embargo</em> they will be lost when this action is committed.'),
      ),
    );

  }
  $form['dsid_mime']['users'] = array(
    '#type' => 'select',
    '#title' => t('Users'),
    '#options' => $users,
    '#default_value' => islandora_xacml_editor_retrieve_users($xacml, $new_xacml, 'datastream'),
    '#multiple' => TRUE,
    '#size' => 10,
    '#prefix' => '<div class="islandora_xacml_selects">',
  );

  $form['dsid_mime']['roles'] = array(
    '#type' => 'select',
    '#title' => t('Roles'),
    '#default_value' => islandora_xacml_editor_retrieve_roles($xacml, $new_xacml, 'datastream'),
    '#options' => $roles,
    '#multiple' => TRUE,
    '#size' => 10,
    '#suffix' => '</div>',
  );

  // AJAX callbacks processing.
  if (isset($form_state['triggering_element'])) {
    // Add DSID.
    if ($form_state['triggering_element']['#name'] == 'dsid_add_button' || $form_state['triggering_element']['#name'] == 'dsid_add_textfield') {
      $object = islandora_object_load($form_state['islandora_xacml']['pid']);

      if (!isset($form_state['islandora_xacml']['add_dsid'])) {
        $form_state['islandora_xacml']['add_dsid'] = array();
      }

      $add_text = trim($form_state['input']['dsid_add_textfield']);

      if (!empty($add_text) && !ctype_space($add_text)) {
        $restricted_dsids = variable_get('islandora_xacml_editor_restricted_dsids', '');
        $restricted_dsids = preg_split('/[\s,]+/', $restricted_dsids);

        if (!(isset($form_state['islandora_xacml']['add_dsid']) && in_array($add_text, $form_state['islandora_xacml']['add_dsid'])) &&
            !(isset($form_state['islandora_xacml']['selected_dsid']) && in_array($add_text, $form_state['islandora_xacml']['selected_dsid'])) &&
            !in_array($add_text, $restricted_dsids)) {
          $form_state['islandora_xacml']['add_dsid'][] = $add_text;
        }
        elseif (in_array($add_text, $restricted_dsids)) {
          drupal_set_message(t('The DSID @dsid was not added as it is restricted from the admin settings page!',
            array(
              '@dsid' => $add_text,
            )
          ), 'warning');
        }
        else {
          drupal_set_message(t('The DSID @dsid was not added as it already exists as a filter!',
            array(
              '@dsid' => $add_text,
            )
          ), 'warning');
        }
      }
      else {
        drupal_set_message(t('No DSID value entered!'), 'error');
      }
    }
    // ADD DSID Regex.
    elseif ($form_state['triggering_element']['#name'] == 'dsid_regex_add_button' || $form_state['triggering_element']['#name'] == 'dsid_regex_add_textfield') {
      if (!isset($form_state['islandora_xacml']['dsid_regexs'])) {
        $form_state['islandora_xacml']['dsid_regexs'] = array();
      }
      $add_text = trim($form_state['input']['dsid_regex_add_textfield']);

      // Check the additional dsids and the dsids from the XACML rules.
      if (!empty($add_text) && !ctype_space($add_text)) {
        if (!(isset($form_state['islandora_xacml']['dsid_regexs']) && in_array($add_text, $form_state['islandora_xacml']['dsid_regexs'])) &&
            !(isset($form_state['islandora_xacml']['selected_dsid_regexs']) && in_array($add_text, $form_state['islandora_xacml']['selected_dsid_regexs']))) {
          $form_state['islandora_xacml']['dsid_regexs'][] = $add_text;
        }
        else {
          drupal_set_message(t('The DSID regex @regex was not added as it already exists as a filter!',
            array(
              '@regex' => $add_text,
            )
          ), 'warning');
        }
      }
      else {
        drupal_set_message(t('No DSID regex value entered!'), 'error');
      }
    }
    // Add MIME Regex.
    elseif ($form_state['triggering_element']['#name'] == 'mime_regex_add_button' || $form_state['triggering_element']['#name'] == 'mime_regex_add_textfield') {
      // Store and checks.
      if (!isset($form_state['islandora_xacml']['mime_regexs'])) {
        $form_state['islandora_xacml']['mime_regexs'] = array();
      }

      $add_text = trim($form_state['input']['mime_regex_add_textfield']);

      if (!empty($add_text) && !ctype_space($add_text)) {
        if (!(isset($form_state['islandora_xacml']['mime_regexs']) && in_array($add_text, $form_state['islandora_xacml']['mime_regexs'])) &&
            !(isset($form_state['islandora_xacml']['selected_mime_regexs']) && in_array($add_text, $form_state['islandora_xacml']['selected_mime_regexs']))) {
          $form_state['islandora_xacml']['mime_regexs'][] = $add_text;
        }
        else {
          drupal_set_message(t('The MIME type regex @regex was not added as it already exists as a filter!',
            array(
              '@regex' => $add_text,
            )
          ), 'warning');
        }
      }
      else {
        drupal_set_message(t('No MIME type regex value entered!'), 'error');
      }
    }
    // Add MIME type.
    elseif ($form_state['triggering_element']['#name'] == 'mime_add_button' || $form_state['triggering_element']['#name'] == 'mime_add_textfield') {
      if (!isset($form_state['islandora_xacml']['add_mime'])) {
        $form_state['islandora_xacml']['add_mime'] = array();
      }

      $add_text = $form_state['input']['mime_add_textfield'];
      if (!empty($add_text) && !ctype_space($add_text)) {
        $restricted_mimes = variable_get('islandora_xacml_editor_restricted_mimes', '');
        $restricted_mimes = preg_split('/[\s,]+/', $restricted_mimes);

        if (!(isset($form_state['islandora_xacml']['add_mime']) && in_array($add_text, $form_state['islandora_xacml']['add_mime'])) &&
            !(isset($form_state['islandora_xacml']['selected_mime']) && in_array($add_text, $form_state['islandora_xacml']['selected_mime'])) &&
            !in_array($add_text, $restricted_mimes)) {
          $form_state['islandora_xacml']['add_mime'][] = $add_text;
        }
        elseif (in_array($add_text, $restricted_mimes)) {
          drupal_set_message(t('The MIME type @mime was not added as it is restricted from the admin settings page!',
            array(
              '@mime' => $add_text,
            )
          ), 'warning');
        }
        else {
          drupal_set_message(t('The MIME type @mime was not added as it already exists as a filter!',
            array(
              '@mime' => $add_text,
            )
          ), 'warning');
        }
      }
      else {
        drupal_set_message(t('No MIME type value entered!'), 'error');
      }
    }
    elseif ($form_state['triggering_element']['#name'] == 'islandora_xacml_editor_remove_all') {
      $remove_count = 0;

      foreach ($form_state['islandora_xacml']['rows'] as $key => $value) {
        $type = drupal_strtolower($value['Type']);
        $filter = $value['Filter'];

        if ($type == 'dsid') {
          $form_state['islandora_xacml']['remove_dsid'][] = $filter;
        }
        elseif ($type == 'mime type') {
          $form_state['islandora_xacml']['remove_mime'][] = $filter;
        }
        elseif ($type == 'mime type regex') {
          $form_state['islandora_xacml']['remove_mime_regex'][] = $filter;
        }
        elseif ($type == 'dsid regex') {
          $form_state['islandora_xacml']['remove_dsid_regex'][] = $filter;
        }
        $remove_count++;
      }

      $remove_output = format_plural($remove_count, '!filter_count applied filter was removed.',
              '!filter_count applied filters were removed.', array('!filter_count' => $remove_count));
      drupal_set_message($remove_output);
    }
    elseif ($form_state['triggering_element']['#name'] == 'islandora_xacml_editor_remove_selected') {
      $remove_row = array();
      foreach ($form_state['values']['dsid_mime']['rules']['table'] as $checkbox => $value) {
        if ($value !== 0) {
          $remove_row[] = $checkbox;
        }
      }
      if (count($remove_row) > 0) {
        $remove_count = 0;

        foreach ($remove_row as $row) {
          $remove_vals = explode('---', $row);
          $type = $remove_vals[0];
          $filter = $remove_vals[1];

          if ($type == 'dsid') {
            $form_state['islandora_xacml']['remove_dsid'][] = $filter;
          }
          elseif ($type == 'mime') {
            $form_state['islandora_xacml']['remove_mime'][] = $filter;
          }
          elseif ($type == 'mime_regexs') {
            $form_state['islandora_xacml']['remove_mime_regex'][] = $filter;
          }
          elseif ($type == 'dsid_regexs') {
            $form_state['islandora_xacml']['remove_dsid_regex'][] = $filter;
          }
          $remove_count++;
        }
        $remove_output = format_plural($remove_count, '!filter_count applied filter was removed.',
              '!filter_count applied filters were removed.', array('!filter_count' => $remove_count));
        drupal_set_message($remove_output);
      }
      else {
        drupal_set_message(t('Please select the filters you wish to remove.'), 'error');
      }
    }
  }

  // Grab these values to handle removal.
  $temp_mime = $xacml->datastreamRule->getMimetypes();
  $temp_dsid = $xacml->datastreamRule->getDsids();
  $temp_mime_regexs = $xacml->datastreamRule->getMimetypeRegexs();
  $temp_dsid_regexs = $xacml->datastreamRule->getDsidRegexs();

  if (isset($form_state['islandora_xacml']['remove_dsid'])) {
    foreach ($form_state['islandora_xacml']['remove_dsid'] as $value) {
      $key = array_search($value, $temp_dsid);

      // If the value is not one of our 'hidden DSIDs'
      if (is_numeric($key)) {
        $xacml->datastreamRule->removeDsid($temp_dsid[$key]);
        $form_state['islandora_xacml']['hidden_dsids'][] = $value;
      }

      if (isset($form_state['islandora_xacml']['add_dsid'])) {
        $search = array_search($value, $form_state['islandora_xacml']['add_dsid']);

        if (is_numeric($search)) {
          unset($form_state['islandora_xacml']['add_dsid'][$search]);
        }
      }
    }
    unset($form_state['islandora_xacml']['remove_dsid']);
  }

  if (isset($form_state['islandora_xacml']['remove_mime'])) {
    foreach ($form_state['islandora_xacml']['remove_mime'] as $value) {
      $key = array_search($value, $temp_mime);

      // If the value is not one of our 'hidden mimes'
      if (is_numeric($key)) {
        $xacml->datastreamRule->removeMimetype($temp_mime[$key]);
        $form_state['islandora_xacml']['hidden_mimes'][] = $value;
      }

      if (isset($form_state['islandora_xacml']['add_mime'])) {
        $search = array_search($value, $form_state['islandora_xacml']['add_mime']);

        if (is_numeric($search)) {
          unset($form_state['islandora_xacml']['add_mime'][$search]);
        }
      }
    }
    unset($form_state['islandora_xacml']['remove_mime']);
  }

  if (isset($form_state['islandora_xacml']['remove_mime_regex'])) {
    foreach ($form_state['islandora_xacml']['remove_mime_regex'] as $value) {
      $key = array_search($value, $temp_mime_regexs);

      // If the value is not one of our 'hidden mime regexs'
      if (is_numeric($key)) {
        $xacml->datastreamRule->removeMimetypeRegex($temp_mime_regexs[$key]);
        $form_state['islandora_xacml']['hidden_mime_regexs'][] = $value;
      }

      if (isset($form_state['islandora_xacml']['mime_regexs'])) {
        $search = array_search($value, $form_state['islandora_xacml']['mime_regexs']);

        if (is_numeric($search)) {
          unset($form_state['islandora_xacml']['mime_regexs'][$search]);
        }
      }
    }
    unset($form_state['islandora_xacml']['remove_mime_regex']);
  }

  if (isset($form_state['islandora_xacml']['remove_dsid_regex'])) {
    foreach ($form_state['islandora_xacml']['remove_dsid_regex'] as $value) {
      $key = array_search($value, $temp_dsid_regexs);

      // If the value is not one of our 'hidden mimes'
      if (is_numeric($key)) {
        $xacml->datastreamRule->removeDsidRegex($temp_dsid_regexs[$key]);
        $form_state['islandora_xacml']['hidden_dsid_regexs'][] = $value;
      }

      if (isset($form_state['islandora_xacml']['dsid_regexs'])) {
        $search = array_search($value, $form_state['islandora_xacml']['dsid_regexs']);

        if (is_numeric($search)) {
          unset($form_state['islandora_xacml']['dsid_regexs'][$search]);
        }
      }
    }
    unset($form_state['islandora_xacml']['remove_dsid_regex']);
  }

  // If we are carrying values from the original rule that need to be removed
  // remove them.
  if (isset($form_state['islandora_xacml']['hidden_mimes'])) {
    foreach ($form_state['islandora_xacml']['hidden_mimes'] as $key => $value) {
      $xacml->datastreamRule->removeMimetype($value);
    }
  }

  if (isset($form_state['islandora_xacml']['hidden_dsids'])) {
    foreach ($form_state['islandora_xacml']['hidden_dsids'] as $key => $value) {
      $xacml->datastreamRule->removeDsid($value);
    }
  }

  if (isset($form_state['islandora_xacml']['hidden_mime_regexs'])) {
    foreach ($form_state['islandora_xacml']['hidden_mime_regexs'] as $key => $value) {
      $xacml->datastreamRule->removeMimetypeRegex($value);
    }
  }

  if (isset($form_state['islandora_xacml']['hidden_dsid_regexs'])) {
    foreach ($form_state['islandora_xacml']['hidden_dsid_regexs'] as $key => $value) {
      $xacml->datastreamRule->removeDsidRegex($value);
    }
  }

  // Grab the updated values to handle addition of rules.
  $temp_mime = $xacml->datastreamRule->getMimetypes();
  $temp_dsid = $xacml->datastreamRule->getDsids();
  $temp_mime_regexs = $xacml->datastreamRule->getMimetypeRegexs();
  $temp_dsid_regexs = $xacml->datastreamRule->getDsidRegexs();

  // Add any values we are carrying in the form storage to the datastream rules.
  if (isset($form_state['islandora_xacml']['add_dsid'])) {
    foreach ($form_state['islandora_xacml']['add_dsid'] as $key => $value) {
      $search = array_search($value, $temp_dsid);

      if (!is_numeric($search)) {
        $xacml->datastreamRule->addDsid($value);

        if (isset($form_state['islandora_xacml']['hidden_sids'])) {
          $remove_dsid = array_search($value, $form_state['islandora_xacml']['hidden_dsids']);

          if (is_numeric($remove_dsid)) {
            unset($form_state['islandora_xacml']['hidesids'][$remove_dsid]);
          }
        }
      }
    }
  }

  if (isset($form_state['islandora_xacml']['add_mime'])) {
    foreach ($form_state['islandora_xacml']['add_mime'] as $key => $value) {
      $search = array_search($value, $temp_mime);

      if (!is_numeric($search)) {
        $xacml->datastreamRule->addMimetype($value);

        if (isset($form_state['islandora_xacml']['hidden_mimes'])) {
          $remove_mime = array_search($value, $form_state['islandora_xacml']['hidden_mimes']);

          if (is_numeric($remove_mime)) {
            unset($form_state['islandora_xacml']['hidden_mimes'][$remove_mime]);
          }
        }
      }
    }
  }

  if (isset($form_state['islandora_xacml']['dsid_regexs'])) {
    foreach ($form_state['islandora_xacml']['dsid_regexs'] as $key => $value) {
      $search = array_search($value, $temp_dsid_regexs);

      if (!is_numeric($search)) {
        $xacml->datastreamRule->addDsidRegex($value);

        if (isset($form_state['islandora_xacml']['hidden_dsid_regexs'])) {
          $remove_dsid = array_search($value, $form_state['islandora_xacml']['hidden_dsid_regexs']);

          if (is_numeric($remove_dsid)) {
            unset($form_state['islandora_xacml']['hidden_dsid_regexs'][$remove_dsid]);
          }
        }
      }
    }
  }

  if (isset($form_state['islandora_xacml']['mime_regexs'])) {
    foreach ($form_state['islandora_xacml']['mime_regexs'] as $key => $value) {
      $search = array_search($value, $temp_mime_regexs);

      if (!is_numeric($search)) {
        $xacml->datastreamRule->addMimetypeRegex($value);

        if (isset($form_state['islandora_xacml']['hidden_mime_regexs'])) {
          $remove_mime = array_search($value, $form_state['islandora_xacml']['hidden_mime_regexs']);

          if (is_numeric($remove_mime)) {
            unset($form_state['islandora_xacml']['hidden_mime_regexs'][$remove_mime]);
          }
        }
      }
    }
  }

  // Grab the values one last time for storage and use in constructing the
  // rules table.
  $selected_mime = $xacml->datastreamRule->getMimetypes();
  $selected_dsid = $xacml->datastreamRule->getDsids();
  $selected_mime_regexs = $xacml->datastreamRule->getMimetypeRegexs();
  $selected_dsid_regexs = $xacml->datastreamRule->getDsidRegexs();

  // We store these values for use in the AJAX callbacks.
  $form_state['islandora_xacml']['dsid_mime']['dsid'] = $selected_dsid;
  $form_state['islandora_xacml']['dsid_mime']['mime'] = $selected_mime;
  $form_state['islandora_xacml']['dsid_mime']['dsid_regexs'] = $selected_dsid_regexs;
  $form_state['islandora_xacml']['dsid_mime']['mime_regexs'] = $selected_mime_regexs;

  if (count($selected_mime) > 0) {
    $form_state['islandora_xacml']['selected_mime'] = array_combine($selected_mime, $selected_mime);
  }
  else {
    unset($form_state['islandora_xacml']['selected_mime']);
  }

  if (count($selected_dsid) > 0) {
    $form_state['islandora_xacml']['selected_dsid'] = array_combine($selected_dsid, $selected_dsid);
  }
  else {
    unset($form_state['islandora_xacml']['selected_dsid']);
  }

  if (count($selected_mime_regexs) > 0) {
    $form_state['islandora_xacml']['selected_mime_regexs'] = array_combine($selected_mime_regexs, $selected_mime_regexs);
  }
  else {
    unset($form_state['islandora_xacml']['selected_mime_regexs']);
  }

  if (count($selected_dsid_regexs) > 0) {
    $form_state['islandora_xacml']['selected_dsid_regexs'] = array_combine($selected_dsid_regexs, $selected_dsid_regexs);
  }
  else {
    unset($form_state['islandora_xacml']['selected_dsid_regexs']);
  }

  $rows = array();
  // Name the rows with a --- convention such that we can easily parse
  // for the selected row to remove down the road.
  if (!empty($selected_mime)) {
    foreach ($selected_mime as $mime) {
      $rows['mime---' . trim($mime)] = array(
        'Filter' => trim($mime),
        'Type' => 'MIME Type',
      );
    }
  }

  if (!empty($selected_mime_regexs)) {
    foreach ($selected_mime_regexs as $mime_regex) {
      $rows['mime_regexs---' . trim($mime_regex)] = array(
        'Filter' => trim($mime_regex),
        'Type' => 'MIME Type Regex',
      );
    }
  }

  if (!empty($selected_dsid)) {
    foreach ($selected_dsid as $dsid) {
      $rows['dsid---' . trim($dsid)] = array(
        'Filter' => trim($dsid),
        'Type' => 'DSID',
      );
    }
  }

  if (!empty($selected_dsid_regexs)) {
    foreach ($selected_dsid_regexs as $dsid_regex) {
      $rows['dsid_regexs---' . $dsid_regex] = array(
        'Filter' => trim($dsid_regex),
        'Type' => 'DSID Regex',
      );
    }
  }
  $form_state['islandora_xacml']['rows'] = $rows;
  $form['dsid_mime']['rules'] = array(
    '#prefix' => '<div id="islandora_xacml_dsid_mime">',
    '#suffix' => '</div>',
  );

  $form['dsid_mime']['rules']['label'] = array(
    '#type' => 'item',
    '#markup' => (count($rows) > 0) ? '<strong>Applied Rules:</strong>' : '<strong>No rules applied!</strong>',
  );
  if (count($rows) > 0) {
    $form['dsid_mime']['rules']['table'] = islandora_xacml_editor_form_table($rows);

    $form['dsid_mime']['rules']['remove_selected'] = array(
      '#type' => 'button',
      '#value' => t('Remove selected'),
      '#name' => 'islandora_xacml_editor_remove_selected',
      '#ajax' => array(
        'event' => 'click',
        'callback' => 'islandora_xacml_editor_remove_selected',
        'wrapper' => 'islandora_xacml_dsid_mime',
        'method' => 'replace',
      ),
    );
    $form['dsid_mime']['rules']['remove_all'] = array(
      '#type' => 'button',
      '#value' => t('Remove all'),
      '#name' => 'islandora_xacml_editor_remove_all',
      '#ajax' => array(
        'event' => 'click',
        'callback' => 'islandora_xacml_editor_remove_all',
        'wrapper' => 'islandora_xacml_dsid_mime',
      ),
    );
  }

  $form['dsid_mime']['new_dsid'] = array(
    '#type' => 'textfield',
    '#title' => t('DSID'),
    '#autocomplete_path' => 'islandora/xacml/dsidautocomplete/' . $object->id,
    '#size' => 35,
    '#description' => t('Type "*" to list all DSIDs.'),
    '#prefix' => '<div class="islandora_xacml_block_description">',
    '#name' => 'dsid_add_textfield',
    '#ajax' => array(
      'event' => 'error',
      'callback' => 'islandora_xacml_editor_add_dsid_js',
      'wrapper' => 'islandora_xacml_dsid_mime',
      'keypress' => TRUE,
    ),
  );
  $form['dsid_mime']['new_dsid_add'] = array(
    '#name' => 'dsid_add_button',
    '#type' => 'button',
    '#value' => t('Add'),
    '#suffix' => '</div>',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'islandora_xacml_editor_add_dsid_js',
      'wrapper' => 'islandora_xacml_dsid_mime',
    ),
  );

  if (variable_get('islandora_xacml_editor_show_dsidregex', 1)) {
    $form['dsid_mime']['dsid_regex'] = array(
      '#type' => 'textfield',
      '#title' => t('DSID Regex'),
      '#description' => l(t('XML regex'), 'http://www.w3.org/TR/xmlschema-0/#regexAppendix'),
      '#size' => 35,
      '#prefix' => '<div class="islandora_xacml_block">',
      '#name' => 'dsid_regex_add_textfield',
      '#ajax' => array(
        'event' => 'error',
        'callback' => 'islandora_xacml_editor_add_dsid_regex_js',
        'wrapper' => 'islandora_xacml_dsid_mime',
        'keypress' => TRUE,
      ),
    );
    $form['dsid_mime']['dsid_regex_add'] = array(
      '#name' => 'dsid_regex_add_button',
      '#type' => 'button',
      '#value' => t('Add'),
      '#suffix' => '</div>',
      '#ajax' => array(
        'event' => 'click',
        'callback' => 'islandora_xacml_editor_add_dsid_regex_js',
        'wrapper' => 'islandora_xacml_dsid_mime',
      ),
    );
  }
  $form['dsid_mime']['new_mime'] = array(
    '#type' => 'textfield',
    '#title' => t('MIME type'),
    '#autocomplete_path' => 'islandora/xacml/mimeautocomplete/' . $object->id,
    '#size' => 35,
    '#description' => t('Type "*" to list all MIME types.'),
    '#prefix' => '<div class="islandora_xacml_block_description">',
    '#name' => 'mime_add_textfield',
    '#ajax' => array(
      'event' => 'error',
      'callback' => 'islandora_xacml_editor_add_mime_js',
      'wrapper' => 'islandora_xacml_dsid_mime',
      'keypress' => TRUE,
    ),
  );
  $form['dsid_mime']['new_mime_add'] = array(
    '#name' => 'mime_add_button',
    '#type' => 'button',
    '#value' => t('Add'),
    '#suffix' => '</div>',
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'islandora_xacml_editor_add_mime_js',
      'wrapper' => 'islandora_xacml_dsid_mime',
    ),
  );

  if (variable_get('islandora_xacml_editor_show_mimeregex', 1)) {
    $form['dsid_mime']['mime_regex'] = array(
      '#type' => 'textfield',
      '#title' => t('MIME type Regex'),
      '#description' => l(t('XML regex'), 'http://www.w3.org/TR/xmlschema-0/#regexAppendix'),
      '#size' => 35,
      '#prefix' => '<div class="islandora_xacml_block">',
      '#name' => 'mime_regex_add_textfield',
      '#ajax' => array(
        'event' => 'error',
        'callback' => 'islandora_xacml_editor_add_mime_regex_js',
        'wrapper' => 'islandora_xacml_dsid_mime',
        'method' => 'replace',
        'keypress' => TRUE,
      ),
    );
    $form['dsid_mime']['mime_regex_add'] = array(
      '#name' => 'mime_regex_add_button',
      '#type' => 'button',
      '#value' => t('Add'),
      '#suffix' => '</div>',
      '#ajax' => array(
        'event' => 'click',
        'callback' => 'islandora_xacml_editor_add_mime_regex_js',
        'wrapper' => 'islandora_xacml_dsid_mime',
        'method' => 'replace',
      ),
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Set Permissions'),
  );
  return $form;
}

/**
 * Retrieves the users selected in each 'Users' portion for datastream rules.
 *
 * @param Xacml $xacml
 *   The XACML object.
 *
 * @param bool $new_xacml
 *   Whether there already exists an XACML policy or not.
 *
 * @param string $rule
 *   The rule which we are checking against.
 *
 * @return array
 *   An array of users that are selected for the specific rule.
 */
function islandora_xacml_editor_retrieve_users($xacml, $new_xacml, $rule) {
  if ($new_xacml == FALSE) {
    if ($rule == 'viewing' && $xacml->viewingRule->isPopulated()) {
      return $xacml->viewingRule->getUsers();
    }
    elseif ($rule == 'datastream' && $xacml->datastreamRule->isPopulated()) {
      return $xacml->datastreamRule->getUsers();
    }
    elseif ($rule == 'management' && $xacml->managementRule->isPopulated()) {
      return $xacml->managementRule->getUsers();
    }
    else {
      return variable_get('islandora_xacml_editor_default_users', 'admin');
    }
  }
  else {
    return variable_get('islandora_xacml_editor_default_users', 'admin');
  }
}

/**
 * Retrieves the roles selected in each 'Roles' portion for datastream rules.
 *
 * @param Xacml $xacml
 *   The XACML object.
 *
 * @param bool $new_xacml
 *   Whether there already exists an XACML policy or not.
 *
 * @param string $rule
 *   The rule which we are checking against.
 *
 * @return array
 *   An array of roles that are selected for the specific rule.
 */
function islandora_xacml_editor_retrieve_roles($xacml, $new_xacml, $rule) {
  if ($new_xacml == FALSE) {
    if ($rule == 'viewing' && $xacml->viewingRule->isPopulated()) {
      return $xacml->viewingRule->getRoles();
    }
    elseif ($rule == 'datastream' && $xacml->datastreamRule->isPopulated()) {
      return $xacml->datastreamRule->getRoles();
    }
    elseif ($rule == 'management' && $xacml->managementRule->isPopulated()) {
      return $xacml->managementRule->getRoles();
    }
    else {
      return variable_get('islandora_xacml_editor_default_roles', 'administrator');
    }
  }
  else {
    return variable_get('islandora_xacml_editor_default_roles', 'administrator');
  }
}

/**
 * Constructs the tableselect used in slandora_xacml_editor_form().
 *
 * @param array $passed_rows
 *   The rows containing the data that need to be rendered.
 *
 * @return array
 *   An array that represents the tableselect to be rendered.
 */
function islandora_xacml_editor_form_table(array $passed_rows) {
  $headers = array(
    'filter' => t('Filter'),
    'type' => t('Type'),
  );

  $rows = array();
  foreach ($passed_rows as $key => $value) {
    $filter = $value['Filter'];
    $type = $value['Type'];
    $rows[$key] = array(
      'filter' => $filter,
      'type' => $type,
    );
  }

  $table = array(
    '#type' => 'tableselect',
    '#header' => $headers,
    '#options' => $rows,
  );
  return $table;
}

/**
 * Implements hook_validate().
 */
function islandora_xacml_editor_form_validate(&$form, &$form_state) {
  drupal_add_css(drupal_get_path('module', 'islandora_xacml_editor') . '/css/islandora_xacml_editor.css');
  $button_trig = array(
    'dsid_add_button',
    'mime_add_button',
    'dsid_regex_add_button',
    'mime_regex_add_button',
  );
  // Test if the user is locking themselves or the admin out of the object.
  $admin_user = $form_state['islandora_xacml']['admin_user'];
  $current_user = $form_state['islandora_xacml']['current_user'];

  // Management functions.
  if ($form_state['values']['manage_enabled']) {
    if (!array_key_exists($admin_user, $form_state['values']['manage']['users']) ||
        !array_key_exists($current_user, $form_state['values']['manage']['users'])) {
      if ($admin_user == $current_user) {
        form_set_error('manage][users', "Please make sure that $admin_user is selected in the manage
          section to prevent locking yourself out of the object.");
      }
      else {
        form_set_error('manage][users', "Please make sure that $admin_user and $current_user are selected in the manage
          section to prevent locking yourself and the admin user out of the object.");
      }
    }
  }

  if ($form_state['values']['dsid_mime_enabled']) {
    if (!array_key_exists($current_user, $form_state['values']['dsid_mime']['users'])) {
      if ($admin_user == $current_user) {
        form_set_error('dsid_mime][users', "Please make sure that $admin_user is selected in the manage
        section to prevent locking yourself out of the object.");
      }
      else {
        form_set_error('dsid_mime][users', "Please make sure that $admin_user and $current_user are selected in the manage
        section to prevent locking yourself and the admin user out of the object.");
      }
    }

    if (count($form_state['islandora_xacml']['rows']) == 0 && (!in_array($form_state['triggering_element']['#name'], $button_trig))) {
      form_set_error('dsid_mime][rules', "There are no filters applied in the datastream and MIME type section.");
    }
  }
}

/**
 * The submit function where all the XACML magic happens. Abracadabra.
 */
function islandora_xacml_editor_form_submit(&$form, &$form_state) {
  $object = islandora_object_load($form_state['islandora_xacml']['pid']);
  $pid = $object->id;
  $xacml = new IslandoraXacml($object);

  // Check datastreams and mime.
  $values = $form_state['values']['dsid_mime'];

  $xacml->datastreamRule->clear();
  if ($form_state['values']['dsid_mime_enabled']) {
    if (array_key_exists('selected_mime', $form_state['islandora_xacml'])) {
      $xacml->datastreamRule->addMimetype($form_state['islandora_xacml']['selected_mime']);
    }

    if (array_key_exists('selected_dsid', $form_state['islandora_xacml'])) {
      $xacml->datastreamRule->addDsid($form_state['islandora_xacml']['selected_dsid']);
    }

    if (array_key_exists('selected_mime_regexs', $form_state['islandora_xacml'])) {
      $xacml->datastreamRule->addMimetypeRegex($form_state['islandora_xacml']['selected_mime_regexs']);
    }

    if (array_key_exists('selected_dsid_regexs', $form_state['islandora_xacml'])) {
      $xacml->datastreamRule->addDsidRegex($form_state['islandora_xacml']['selected_dsid_regexs']);
    }

    $xacml->datastreamRule->addUser($values['users']);
    $xacml->datastreamRule->addRole($values['roles']);
  }

  // Check admin (always have this rule).
  $values = $form_state['values']['manage'];
  $xacml->managementRule->clear();
  if ($form_state['values']['manage_enabled']) {
    $xacml->managementRule->addUser($values['users']);
    $xacml->managementRule->addRole($values['roles']);
  }

  // Check access.
  $values = $form_state['values']['access'];
  $xacml->viewingRule->clear();
  if ($form_state['values']['access_enabled']) {
    $xacml->viewingRule->addUser($values['users']);
    $xacml->viewingRule->addRole($values['roles']);
  }

  $xacml->writeBackToFedora();

  $form_state['redirect'] = array('islandora/object/' . $pid);
  if (isset($form_state['islandora_xacml']['query_choices']) && $form_state['values']['update_options'] != 'newchildren') {
    $option = $form_state['values']['update_options'];
    $query_array = $form_state['islandora_xacml']['query_choices'][$option];
    $xml = $xacml->getXmlString();
    $batch = array(
      'title' => t('Updating Policies'),
      'progress_message' => t('Please wait if many objects are being updated this could take a few minutes.'),
      'operations' => array(
        array(
          'islandora_xacml_editor_batch_function',
          array($xml, $pid, $query_array),
        ),
      ),
      'finished' => 'islandora_xacml_editor_batch_finished',
      'file' => drupal_get_path('module', 'islandora_xacml_editor') . '/includes/batch.inc',
    );
    batch_set($batch);
  }
  else {
    unset($form_state['islandora_xacml']);
    drupal_set_message(t('The configured POLICY datastream has been applied to @pid!', array(
      '@pid' => $pid,
    )));
  }
}

/**
 * Theme the xacml policy management table.
 *
 * @param array $variables
 *   Variables passed to this theme function.
 *
 * @return string
 *   Markup representing the table for rendering.
 */
function theme_islandora_xacml_editor_policy_management_table(array $variables) {
  // Manually add the table select javascript as we are using a custom table.
  drupal_add_js('misc/tableselect.js');
  $table = $variables['table'];
  $row_elements = $table['rows'];
  $rows = array();
  foreach (element_children($row_elements) as $key) {
    $columns = array();
    $row_element = $row_elements[$key];
    foreach (element_children($row_element) as $key) {
      $column_element = $row_element[$key];
      $columns[] = array(
        'data' => drupal_render($column_element),
        'class' => isset($cell_element['#attributes']['class']) ? $column_element['#attributes']['class'] : NULL,
      );
    }
    $rows[] = $columns;
  }
  $variables = array(
    'header' => $table['#header'],
    'rows' => $rows,
    'attributes' => $table['#attributes'],
    'caption' => NULL,
    'colgroups' => NULL,
    'sticky' => NULL,
    'empty' => t("No child collection(s)."),
  );
  return theme('table', $variables);
}
/**
 * AJAX callback to remove the selected filters from the rules table.
 */
function islandora_xacml_editor_remove_selected($form, $form_state) {
  return $form['dsid_mime']['rules'];
}

/**
 * AJAX callback to remove all filters from the rules table.
 */
function islandora_xacml_editor_remove_all($form, $form_state) {
  return $form['dsid_mime']['rules'];
}

/**
 * AJAX callback to add a DSID Regex to the rules table.
 */
function islandora_xacml_editor_add_dsid_regex_js($form, $form_state) {
  return $form['dsid_mime']['rules'];
}

/**
 * AJAX callback to add a DSID to the rules table.
 */
function islandora_xacml_editor_add_dsid_js($form, $form_state) {
  return $form['dsid_mime']['rules'];
}

/**
 * AJAX callback to add a MIME Regex to the rules table.
 */
function islandora_xacml_editor_add_mime_regex_js($form, $form_state) {
  return $form['dsid_mime']['rules'];
}

/**
 * AJAX callback to add a MIME Type to the rules table.
 */
function islandora_xacml_editor_add_mime_js($form, $form_state) {
  return $form['dsid_mime']['rules'];
}
