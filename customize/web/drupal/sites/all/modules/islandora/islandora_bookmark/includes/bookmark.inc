<?php

/**
 * @file
 * Definition of a Bookmark object.
 */

/**
 * Interface defining the base operations required to implement a backend.
 */
interface BookmarkInterface {

  /**
   * Adds the specified PID to the given list.
   *
   * @param string $pid
   *   The PID of an object.
   */
  public function addPid($pid);

  /**
   * Get PIDs associated with this list.
   *
   * @param int $offset
   *   The starting offset of pids to retrieve.
   *
   * @param int $limit
   *   The number of pids to retrieve.
   *
   * @return array
   *   An array of string, each representing an ID in a list.
   */
  public function getPids($offset = 0, $limit = -1);

  /**
   * Removes the specified PID from the given list.
   *
   * @param string $pid
   *   The PID of an object.
   */
  public function removePid($pid);

  /**
   * Shares a list with the given user.
   *
   * @param int $user
   *   A user ID.
   */
  public function addUser($user);

  /**
   * Get a list of all the users who a list is shared with.
   *
   * @return array
   *   An array of integers, representing user IDs.
   */
  public function getUsers();

  /**
   * Removes the given user from the object.
   *
   * @param int $user
   *   A user ID.
   */
  public function removeUser($user);

  /**
   * Delete the given list.
   *
   * Removes the list from the backend; attempting to get the same list again
   * should fail.
   */
  public function delete();
}

/**
 * Class that defines a Bookmark object. A Bookmark object needs only to be
 * constructed such that it contains a listname and listid for uniqueness.
 */
abstract class Bookmark implements BookmarkInterface {

  /**
   * The name of the object.
   *
   * @var string
   */
  public $bookmarkName;

  /**
   * The description of the object.
   *
   * @var string
   */
  public $bookmarkDescription;

  /**
   * The owner of the object.
   *
   * @var int
   */
  public $bookmarkOwner;

  /**
   * The ID of the given list.
   *
   * @var mixed
   */
  protected $bookmarkId;

  /**
   * Flag whether or not the given backend is persistent.
   *
   * @var bool
   */
  protected $isPersistent;

  /**
   * Flag whether the object can be deleted or not.
   *
   * @var bool
   */
  protected $isDeletable;

  /**
   * Constructor for the Bookmark object.
   */
  public function __construct($id, $name, $owner, $desc = "") {
    $this->bookmarkId = $id;
    $this->bookmarkName = $name;
    $this->bookmarkOwner = $owner;
    $this->bookmarkDescription = $desc;
    $this->isDeletable = TRUE;
  }

  /**
   * Accessor for the list ID.
   *
   * @return mixed
   *   The value of $this->bookmarkId.
   */
  public function getId() {
    return $this->bookmarkId;
  }

  /**
   * Accessor for the persistence.
   *
   * @return mixed
   *   The value of $this->isPersistent.
   */
  public function getIsPersistent() {
    return $this->isPersistent;
  }

  /**
   * Accessor for whether the object is deletable.
   *
   * @return mixed
   *   The value of $this->isDeletable.
   */
  public function getIsDeletable() {
    return $this->isDeletable;
  }

  /**
   * Get the given (existing) list.
   *
   * @param mixed $id
   *   The ID of the list to attempt to load.
   * @param bool $force_session_attempt
   *   Force examination of the session, to handle the hook_user_login() stuff.
   *
   * @return Bookmark|NULL
   *   Returns either an instantiated Bookmark object if the list exists, or
   *   NULL if we could not get it.
   */
  public static function getList($id, $force_session_attempt = FALSE) {
    global $user;

    $bookmark = NULL;
    if ($user->uid === 0 || $force_session_attempt) {
      $bookmark = BookmarkSession::getList($id, TRUE);
    }

    if ($bookmark === NULL) {
      // Pass off to BookmarkDatabase.
      $bookmark = BookmarkDatabase::getList($id, FALSE);
    }
    return $bookmark;
  }

  /**
   * Get the number of PIDs in the current list.
   *
   * Individual backends could probably do this more efficiently, so feel free
   * to override.
   *
   * @return int
   *   An integer representing the number of items in the current list.
   */
  public function getPidCount() {
    return count($this->getPids());
  }

  /**
   * Remove multiple PIDs from the list.
   *
   * Individual backends could probably do this more efficiently, so feel free
   * to override.
   *
   * @param array $pids
   *   An array of pids.
   */
  public function removePids($pids) {
    foreach ($pids as $pid) {
      $this->removePid($pid);
    }
  }

  /**
   * Remove all PIDs from the list.
   *
   * Individual backends could probably do this more efficiently, so feel free
   * to override.
   */
  public function removeAllPids() {
    $this->removePids($this->getPids());
  }

  /**
   * Remove multiple users from the list.
   *
   * Individual backends could probably do this more efficiently, so feel free
   * to override.
   *
   * @param array $users
   *   An array of integers, each representing a user ID.
   */
  public function removeUsers($users) {
    foreach ($users as $user) {
      $this->removeUser($user);
    }
  }

  /**
   * Adds the specified pids to the object and either the database or session.
   *
   * Individual backends could probably do this more efficiently, so feel free
   * to override.
   *
   * @param array $pids
   *   An array of strings, representing object PIDs.
   */
  public function addPids($pids) {
    foreach ($pids as $pid) {
      $this->addPid($pid);
    }
  }

  /**
   * Adds the specified users to the object and either the database or session.
   *
   * Individual backends could probably do this more efficiently, so feel free
   * to override.
   *
   * @param array $users
   *   An array of integers, representings user IDs.
   */
  public function addUsers($users) {
    foreach ($users as $user) {
      $this->addUser($user);
    }
  }

  /**
   * Create a new list for the current user.
   *
   * @param string $name
   *   A name for the newly created list.
   * @param string $desc
   *   An optional description for the list.
   *
   * @return BookmarkSession|BookmarkDatabase
   *   The BookmarkSession or BookmarkDatabase object that is created.
   */
  public static function createNewList($name, $desc = "") {
    global $user;

    if ($user->uid === 0) {
      return BookmarkSession::createNewList($name, NULL, $desc);
    }
    else {
      return BookmarkDatabase::createNewList($name, 'bookmark', NULL, $desc);
    }
  }

  /**
   * Removes the list from the backend.
   *
   * @deprecated Just here to conform with the old API.
   */
  public function removeList() {
    $this->delete();
  }

  /**
   * Changes the name of the object in the session or the database.
   *
   * @param string $name
   *   The new name of the object.
   */
  public function changeListName($name) {
    $this->bookmarkName = $name;
  }

  /**
   * Changes the description of the object in the session or the database.
   *
   * @param string $description
   *   The new description of the object.
   */
  public function changeDescription($description) {
    $this->bookmarkDescription = $description;
  }

  /**
   * Check if user can execute management functions on the current Bookmark.
   *
   * @return bool
   *   TRUE if they can execute management functions, FALSE otherwise.
   */
  public function managementAccess() {
    global $user;
    if ($user->uid === $this->bookmarkOwner) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Get the definition for the table.
   *
   * @param array $pids
   *   An array of PIDs, for which to build a tableselect.
   *
   * @return array
   *   An associative array defining a tableselect element.
   */
  protected function getTable($pids) {
    return array(
      '#type' => 'tableselect',
      '#header' => $this->getTableHeader(),
      '#options' => array_map(array($this, 'getTableRow'), drupal_map_assoc($pids)),
    );
  }

  /**
   * Build the header for the table.
   *
   * @return array
   *   An array representing the header for a tableselect, as used by
   *   theme_tableselect().
   */
  protected function getTableHeader() {
    $header = array('markup' => array('data' => t('Label')));
    if ($this->managementAccess()) {
      $header['remove'] = array('data' => t('Operations'));
    }
    return $header;
  }

  /**
   * Build the row for the given PID.
   *
   * @param string $pid
   *   A PID for which to generate a row in the tableselect.
   *
   * @return array
   *   An associative array keyed according to what is provided in the header.
   */
  protected function getTableRow($pid) {
    $row = array(
      'markup' => islandora_bookmark_generate_markup($pid),
    );

    if ($this->managementAccess()) {
      $remove_button = array(
        '#type' => 'submit',
        '#name' => 'remove_pid_' . $pid,
        '#id' => 'remove_pid_' . $pid,
        '#button_type' => 'submit',
        '#pid' => $pid,
        '#value' => t('Remove'),
      );
      $row['remove'] = array(
        'data' => drupal_render($remove_button),
        'class' => 'operations',
      );
    }

    return $row;
  }

  /**
   * Form building method.
   */
  public function formGet($form, &$form_state) {
    if (module_exists('citation_exporter')) {
      if (CitationExporter::ReadyToExport()) {
        $citation_export_head = array(
          '#type' => 'html_tag',
          '#tag' => 'meta',
          '#attributes' => array(
            'http-equiv' => 'refresh',
            'content' => '1.0;' . url(CITATION_EXPORTER_EXPORT_MENU, array('absolute' => TRUE)),
          ),
        );
        drupal_add_html_head($citation_export_head, 'citation_exporter_head');
      }
    }

    $form = array(
      '#tree' => TRUE,
      '#attached' => array(
        'css' => array(
          drupal_get_path('module', 'islandora_bookmark') . '/css/islandora_bookmark.css',
        ),
      ),
      '#prefix' => '<div id="islandora-bookmark-detailed-overview">',
      '#suffix' => '</div>',
      'bookmarks' => array(
        '#weight' => 0,
        'fieldset' => array(
          '#type' => 'fieldset',
          '#title' => 'Detailed List',
          '#prefix' => '<div id="islandora-bookmark-detailed-list">',
          '#suffix' => '</div>',
        ),
      ),
    );

    // This is the list links.
    $form['bookmark_list_links'] = theme(
      'islandora_bookmark_list_links',
      array('bookmark_id' => $this->bookmarkId, 'current' => 'manage')
    );

    // Do pager calcuations to limit the number of objects displayed per page.
    $num_eles = variable_get('islandora_bookmark_detailed_page_elements', 10);
    $num_pids = $this->getPidCount();
    $form_state['islandora_bookmark_pager_element'] = $pager_element = 0;

    $page = pager_default_initialize($num_pids, $num_eles, $pager_element);
    $offset = $page * $num_eles;

    $pids = $this->getPids($offset, $num_eles);

    $table = $this->getTable($pids);
    $form['bookmarks']['fieldset'] += array(
      'table' => $table += array(
        '#empty' => t('@type list is empty.', array(
      '@type' => ucwords(variable_get('islandora_bookmark_type', 'bookmark')),
    )),
        '#weight' => -2,
      ),
      'pager' => array(
        '#markup' => theme('pager', array(
          'element' => $pager_element,
        )),
        '#weight' => -1,
      ),
    );
    $row_count = count($form['bookmarks']['fieldset']['table']['#options']);

    if ($row_count > 0) {
      $this->formAddActions($form['bookmarks']['fieldset'], $form_state);
    }

    $this->formAddSecondaryActions($form, $form_state);

    return $form;
  }

  /**
   * Add actions to the given section of the form.
   *
   * @param array $container
   *   A reference to a part of the form, where the actions should be added.
   * @param array $form_state
   *   The Drupal form state.
   */
  protected function formAddActions(&$container, &$form_state) {
    $groups = array();

    if ($this->managementAccess()) {
      $groups['remove'] = array(
        'remove_selected' => array(
          '#type' => 'submit',
          '#value' => t('Remove selected'),
          '#action_validate' => array(
            'formHasObjectsSelected',
          ),
          '#action_submit' => array(
            'formRemoveSelectedFromList',
          ),
        ),
        'remove_all' => array(
          '#type' => 'submit',
          '#value' => t('Remove all'),
          '#action_submit' => array(
            'formRemoveAllFromList',
          ),
        ),
      );
    }

    if (user_access('export islandora bookmarks')) {
      $export_styles = array();
      $export = islandora_bookmark_get_export_options();
      $export_options['default'] = t('- Export type -');
      // As export options can be user configurable and want to maintain
      // human readability through the UI need to account for the fact that
      // some options could have spaces in them. If spaces are encountered in
      // the $_POST Drupal will not pick them up in the form state values.
      // As such two formats can be used one which specifics a callable
      // and a human-readable pair and the old form.
      foreach ($export as $module_name => $export_array) {
        foreach ($export_array as $name => $export_func) {
          if (is_callable($export_func)) {
            $export_options[t("@module_name", array("@module_name" => $module_name))][$name] = $name;
            $form_state['islandora_bookmark_export'][$name] = $export_func;
          }
          else {
            $export_options[t("@module_name", array("@module_name" => $module_name))][$name] = $export_func['human_readable'];
            $form_state['islandora_bookmark_export'][$name] = $export_func['callable'];
          }
          $export_styles[$name] = islandora_bookmark_get_export_format_options($name);
        }
      }

      $style_elements = array();
      foreach ($export_styles as $key => $value) {
        if (!empty($value)) {
          $style_elements[$key] = array(
            '#type' => 'select',
            '#title' => t('Style'),
            '#options' => $value,
            '#weight' => 0,
            '#states' => array(
              'visible' => array(
                ':input[name="bookmarks[fieldset][export_options]"]' => array('value' => $key),
              ),
            ),
          );
        }
      }

      $groups += array(
        'export_title' => array(
          '#markup' => '<h3>' . t('Export') . '</h3>',
          '#weight' => 3,
        ),
        'export_options' => array(
          '#type' => 'select',
          '#title' => t('Format'),
          '#options' => $export_options,
          '#weight' => 4,
        ),
        'format_opts' => array(
          '#weight' => 5,
          'export_styles' => $style_elements,
          'export_selected' => array(
            '#type' => 'submit',
            '#value' => t('Export selected @type', array(
              // XXX: Should be made better for translating... (Not all
              // languages add an "s" to pluralize).
              '@type' => variable_get('islandora_bookmark_type', 'bookmark') . 's',
            )),
            '#weight' => 5,
            '#action_validate' => array(
              'formHasExportTypeSelected',
              'formHasObjectsSelected',
            ),
            '#action_submit' => array(
              'formExportSelected',
            ),
          ),
          'export_all' => array(
            '#type' => 'submit',
            '#value' => t('Export all @type', array(
              // XXX: Should be made better for translating... (Not all
              // languages add an "s" to pluralize).
              '@type' => variable_get('islandora_bookmark_type', 'bookmark') . 's',
            )),
            '#weight' => 6,
            '#action_validate' => array(
              'formHasExportTypeSelected',
            ),
            '#action_submit' => array(
              'formExportAll',
            ),
          ),
        ),
      );
    }

    if (!empty($groups)) {
      $container += $groups;
    }
  }

  /**
   * Add secondary actions to the form.
   */
  protected function formAddSecondaryActions(&$form, &$form_state) {
    global $user;
    // Share.
    if ($this->managementAccess()) {
      if (user_access('share islandora bookmarks')) {
        // Triggered element (Add item or remove).
        if (isset($form_state['triggering_element'])) {
          // Add.
          if ($form_state['triggering_element']['#button_id'] == 'islandora-bookmark-add-user') {
            $add_user = $form_state['input']['bookmarks']['users']['listusers'];
            if (drupal_strlen($add_user)) {
              if ($add_user == 0) {
                $output_user = variable_get('islandora_bookmark_owner', 'Everyone');
              }
              else {
                $output_user = user_load($add_user)->name;
              }
              drupal_set_message(t('The user @adduser has been added to @listname', array(
                '@listname' => $this->bookmarkName,
                '@adduser' => $output_user,
              )));
              $this->addUsers(array($add_user));
            }
          }
          // Remove.
          if ($form_state['triggering_element']['#button_id'] == 'islandora-bookmark-remove-user') {
            $row = $form_state['triggering_element']['#row'];
            $remove_user = $form_state['values']['bookmarks']['users'][$row]['user'];
            if ($remove_user == 0) {
              $output_user = variable_get('islandora_bookmark_owner', 'Everyone');
            }
            else {
              $output_user = user_load($remove_user)->name;
            }
            drupal_set_message(t('The user @username has been removed from the list @listname.', array(
              '@listname' => $this->bookmarkName,
              '@username' => $output_user,
                    )
            ));
            $this->removeUsers(array($remove_user));
          }
        }

        $fku = array();
        if ($this->getIsPersistent()) {

          $users = $this->getUsers();
          if (count($users) === 0) {
            $fku['empty'] = array(
              '#type' => 'item',
              '#markup' => t('@type list is not shared with other users.', array(
                '@type' => ucwords(variable_get('islandora_bookmark_type', 'bookmark')),
              )),
            );
          }
          else {
            $fku['title'] = array(
              '#markup' => '<h3>' . t('Shared with') . ':</h3>',
            );
            // Remove the user from the populated shared users.
            $bookmark_users = $this->getUsers();
            $key = array_search($user->uid, $bookmark_users);
            if ($key !== FALSE) {
              unset($bookmark_users[$key]);
            }

            foreach ($bookmark_users as $itr => $uid) {
              $tempuser = user_load($uid);
              if ($tempuser->uid == 0) {
                $user_output = variable_get('islandora_bookmark_owner', 'Everyone');
              }
              else {
                $user_output = $tempuser->name;
              }
              if ($tempuser->uid != $user->uid) {
                $fku[$itr] = array(
                  '#prefix' => '<div class="islandora-bookmark-users">',
                  '#suffix' => '</div>',
                  'label' => array(
                    '#type' => 'item',
                    '#markup' => $user_output,
                  ),
                  'user' => array(
                    '#type' => 'value',
                    '#value' => $uid,
                  ),
                  'list-id' => array(
                    '#type' => 'value',
                    '#value' => $this->getId(),
                  ),
                  'remove' => array(
                    '#type' => 'button',
                    '#value' => t('Remove'),
                    '#name' => 'islandora_bookmark_remove_user_' . $itr,
                    '#row' => $itr,
                    '#button_id' => 'islandora-bookmark-remove-user',
                    '#ajax' => array(
                      'event' => 'click',
                      'callback' => 'islandora_bookmark_remove_user',
                      'wrapper' => 'islandora-bookmark-detailed-overview-users',
                      'method' => 'replace',
                    ),
                  ),
                );
              }
            }
          }

          $selectoptions = array(
            '' => t('- Select user -'),
          );
          if ($user->uid != 0) {
            $options['0'] = array(
              'uid' => 0,
              'name' => variable_get('islandora_bookmark_owner', 'Everyone'),
            );
          }
          // Get all users for use in select for forms.
          $result = db_select('users', 'u')
              ->fields('u', array('uid', 'name'))
              ->condition('STATUS', 0, '!=')
              ->condition('uid', $user->uid, '!=')
              ->orderBy('uid')
              ->execute();

          $options += $result->fetchAllAssoc('uid', PDO::FETCH_ASSOC);
          if (isset($options['0'])) {
            $options[0]['name'] = variable_get('islandora_bookmark_owner', 'Everyone');
          }

          $available = array_diff_key($options, drupal_map_assoc($this->getUsers()));
          foreach ($available as $index => $item) {
            $selectoptions[$index] = $item['uid'] . ': ' . $item['name'];
          }
          $fku['listusers'] = array(
            '#type' => 'select',
            '#default value' => t("- Select user -"),
            '#options' => $selectoptions,
            '#width' => 20,
            '#prefix' => '<div class="islandora-bookmark-adduser">',
          );
          $fku['listadd'] = array(
            '#type' => 'button',
            '#value' => t('Share'),
            '#button_id' => 'islandora-bookmark-add-user',
            '#suffix' => '</div>',
            '#ajax' => array(
              'event' => 'click',
              'callback' => 'islandora_bookmark_add_user',
              'wrapper' => 'islandora-bookmark-detailed-overview-users',
              'method' => 'replace',
            ),
          );

          $form['bookmarks']['users'] = array(
            '#type' => 'fieldset',
            '#collapsible' => TRUE,
            '#collapsed' => FALSE,
            '#title' => t('Share'),
            '#prefix' => '<div id="islandora-bookmark-detailed-overview-users">',
            '#suffix' => '</div>',
          ) + $fku;
        }
      }
    }
    elseif ($user->uid != 0 && $user->uid != $this->bookmarkOwner && !in_array(0, $this->getUsers())) {
      $form['bookmarks']['remove_self'] = array(
        '#type' => 'submit',
        '#value' => t('Remove yourself from this list'),
        '#submit' => array('islandora_bookmark_remove_self'),
      );
    }
  }

  /**
   * Form validation callback; ensure at least one object is selected.
   */
  protected function formHasObjectsSelected($form, $form_state) {
    $pids = $this->formGetSelected($form_state);
    if (empty($pids)) {
      form_set_error('bookmarks][fieldset][table', t('You must select at least one object.'));
    }
  }

  /**
   * One of our '#action_validate' callbacks.
   *
   * Checks that the export action has an export type selected.
   */
  protected function formHasExportTypeSelected($form, $form_state) {
    $el = $form['bookmarks']['fieldset']['export_options'];
    $key = drupal_array_get_nested_value($form_state['values'], $el['#parents']);
    if ($key == 'default') {
      form_set_error(implode('][', $el['#parents']), t('Please select an export type.'));
    }
  }

  /**
   * Submission callback; export the selected PIDs.
   */
  protected function formExportSelected(&$form, &$form_state) {
    $pids = $this->formGetSelected($form_state);
    $this->formExport($pids, $form_state);
  }

  /**
   * Submission callback; export all PIDs in the list.
   */
  protected function formExportAll(&$form, &$form_state) {
    $pids = $this->formGetAll($form_state);
    $this->formExport($pids, $form_state);
  }

  /**
   * Submission helper; export the given PIDs.
   */
  protected function formExport($pids, $form_state) {
    $el = $form_state['complete form']['bookmarks']['fieldset']['export_options'];
    $key = drupal_array_get_nested_value($form_state['values'], $el['#parents']);
    $params = array(
      $pids,
    );
    if (isset($form_state['values']['bookmarks']['fieldset']['format_opts']['export_styles'])) {
      if (array_key_exists($key, $form_state['values']['bookmarks']['fieldset']['format_opts']['export_styles'])) {
        $params[] = $form_state['values']['bookmarks']['fieldset']['format_opts']['export_styles'][$key];
      }
      // Preserve position.
      else {
        $params[] = NULL;
      }
    }
    // Preserve position.
    else {
      $params[] = NULL;
    }
    // Add the form state on the end for use in potential form alters. Doing it
    // here as opposed to earlier to preserve the listing of parameters that
    // existing implementations may expect.
    $params[] = $form_state;
    call_user_func_array($form_state['islandora_bookmark_export'][$key], $params);
  }

  /**
   * Submission callback; remove the selected PIDs from the list.
   */
  protected function formRemoveSelectedFromList(&$form, &$form_state) {
    $pids = $this->formGetSelected($form_state);
    $this->formRemoveFromList($pids, $form_state);
  }

  /**
   * Submission callback; remove all PIDs from the list.
   */
  protected function formRemoveAllFromList(&$form, &$form_state) {
    $pids = $this->formGetAll($form_state);
    $this->formRemoveFromList($pids, $form_state);
  }

  /**
   * Submission helper; remove the given PIDs from the list.
   */
  protected function formRemoveFromList($pids, $form_state) {
    $this->removePids($pids);
  }

  /**
   * Form validation handler.
   */
  public function formValidate(&$form, &$form_state) {
    $pids_to_remove = $this->formGetPidToRemove($form_state);
    if ($pids_to_remove) {
      // No-op.
    }
    elseif (isset($form_state['clicked_button']['#action_validate'])) {
      $params = array(&$form, &$form_state);
      foreach ($form_state['clicked_button']['#action_validate'] as $validator) {
        call_user_func_array(array($this, $validator), $params);
      }
    }
    elseif (isset($form_state['triggering_element']['#button_id'])) {
      if ($form_state['triggering_element']['#button_id'] == 'islandora-bookmark-add-user') {
        $parents = array('bookmarks', 'users', 'listusers');
        $add_user = drupal_array_get_nested_value($form_state['values'], $parents);
        if (drupal_strlen($add_user) === 0) {
          form_set_error(implode('][', $parents), t('Please select a user to add to @listname.', array(
            '@listname' => $this->bookmarkName,
          )));
        }
      }
    }
  }

  /**
   * Get the PIDs of any single remove button(s).
   *
   * @param array $form_state
   *   The Drupal form state.
   *
   * @return array
   *   An array of PIDs--may be empty if there are none present.
   */
  protected function formGetPidToRemove($form_state) {
    $to_remove = preg_grep('/remove_pid_.*/', array_keys($form_state['input']));
    return preg_replace('/remove_pid_(.*)/', '$1', $to_remove);
  }

  /**
   * Form submission handler.
   */
  public function formSubmit(&$form, &$form_state) {
    $pids_to_remove = $this->formGetPidToRemove($form_state);
    if ($pids_to_remove) {
      $this->removePids($pids_to_remove);

      foreach (array_map('islandora_object_load', $pids_to_remove) as $removed_object) {
        drupal_set_message(t('The @type @objectname has been removed from @listname.', array(
          '@type' => variable_get('islandora_bookmark_type', 'bookmark'),
          '@listname' => $this->bookmarkName,
          '@objectname' => $removed_object->label,
        )));
      }
    }
    elseif (isset($form_state['clicked_button']['#action_submit'])) {
      $params = array(&$form, &$form_state);
      foreach ($form_state['clicked_button']['#action_submit'] as $submit) {
        call_user_func_array(array($this, $submit), $params);
      }
    }

    $errors = form_get_errors();
    if (count($errors) === 0) {
      $page = pager_default_initialize(
          $this->getPidCount(), variable_get('islandora_bookmark_detailed_page_elements', 10), $form_state['islandora_bookmark_pager_element']
      );

      $form_state['redirect'] = array(
        'islandora-bookmark/listid/' . $this->getId(),
        array(),
      );

      if ($page > 0) {
        $form_state['redirect'][1]['query'] = array(
          'page' => $page,
        );
      }
    }
  }

  /**
   * Submission helper; gets all PIDs.
   */
  protected function formGetAll($form_state) {
    $selected = array_filter($this->getPids());
    $functional = array_filter($selected, 'islandora_object_load');
    $broken = array_diff($selected, $functional);
    if (!empty($broken)) {
      watchdog('islandora_bookmark', 'Broken PIDs encountered: @pids', array(
        '@pids' => implode(', ', $broken),
          ), WATCHDOG_WARNING);
    }

    return $functional;
  }

  /**
   * Submission helper; gets the PIDs of the selected items.
   */
  protected function formGetSelected($form_state) {
    $selected = array_filter($form_state['values']['bookmarks']['fieldset']['table']);
    $functional = array_filter($selected, 'islandora_object_load');
    $broken = array_diff($selected, $functional);
    if (!empty($broken)) {
      watchdog('islandora_bookmark', 'Broken PIDs encountered: @pids', array(
        '@pids' => implode(', ', $broken),
          ), WATCHDOG_WARNING);
    }

    return $functional;
  }

}
