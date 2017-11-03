<?php
/**
 * @file
 * Views hook implementations.
 */

/**
 * Implements hook_views_data().
 */
function islandora_batch_views_data() {
  $data = array();

  $data['islandora_batch_queue'] = array(
    'table' => array(
      'group' => t('Islandora Batch'),
      'base' => array(
        'field' => 'id',
        'title' => t('Islandora Batch Queue'),
        'help' => t('The queue of objects to batch ingest.'),
      ),
      'join' => array(
        'islandora_batch_queue' => array(
          'left_field' => 'id',
          'field' => 'parent',
        ),
        'islandora_batch_set' => array(
          'left_field' => 'id',
          'field' => 'sid',
        ),
      ),
    ),
    'id' => array(
      'title' => t('ID'),
      'help' => t('Identifier of an item in the queue.'),
      'field' => array(
        'handler' => 'views_handler_field',
        // This is use by the table display plugin.
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'parent' => array(
      'title' => t('Parent'),
      'help' => t('Identifier of the parent of the current item.'),
      'field' => array(
        'handler' => 'views_handler_field',
        // This is use by the table display plugin.
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'sid' => array(
      'title' => t('Set ID'),
      'help' => t('ID of the set in which an item resides.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_numeric',
      ),
      'relationship' => array(
        'base' => 'islandora_batch_set',
        'base field' => 'id',
        'handler' => 'views_handler_relationship',
        'label' => t('Associated Set'),
        'title' => t('Associated Set'),
        'help' => t('The set of objects with which the given object is associated.'),
      ),
    ),
    'actions' => array(
      'title' => t('Item Actions'),
      'help' => t('A list of actions which may be performed on an item in the queue.'),
      'field' => array(
        'handler' => 'IslandoraBatchViewsFieldHandlerQueueActions',
      ),
    ),
  );

  $data['islandora_batch_set'] = array(
    'table' => array(
      'group' => t('Islandora Batch'),
      'base' => array(
        'field' => 'id',
        'title' => t('Islandora Batch Set'),
        'help' => t('A set of items, to groups items from the queue.'),
      ),
    ),
    'id' => array(
      'title' => t('Set ID'),
      'help' => t('ID of a set of related items.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
    ),
    'uid' => array(
      'title' => t('User ID'),
      'help' => t('ID of the user who created a set.'),
      'field' => array(
        'handler' => 'views_handler_field_user',
        'click sortable' => TRUE,
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_user_uid',
        'name field' => 'name',
      ),
      'filter' => array(
        'title' => t('Name'),
        'handler' => 'views_handler_filter_user_name',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'relationship' => array(
        'base' => 'users',
        'base field' => 'uid',
        'handler' => 'views_handler_relationship',
        'label' => t('Set creator'),
        'title' => t('Set Creator'),
        'help' => t('The user who created a given set.'),
      ),
    ),
    'created' => array(
      'title' => t('Created Date'),
      'help' => t('The date on which the given set was registered.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
      ),
    ),
    'actions' => array(
      'title' => t('Set Actions'),
      'help' => t('A list of actions which may be performed on a set of items in the queue.'),
      'field' => array(
        'handler' => 'IslandoraBatchViewsFieldHandlerSetActions',
      ),
    ),
  );

  $data['islandora_batch_state'] = array(
    'table' => array(
      'group' => t('Islandora Batch'),
      'join' => array(
        'islandora_batch_queue' => array(
          'left_field' => 'id',
          'field' => 'id',
        ),
      ),
    ),
    'id' => array(
      'title' => t('ID'),
      'help' => t('Identifier of an item in the queue.'),
    ),
    'state' => array(
      'title' => t('Item State'),
      'help' => t('The state of an item in the queue.'),
      'field' => array(
        'handler' => 'IslandoraBatchStateFieldHandler',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
  );

  $data['islandora_batch_queue_messages'] = array(
    'table' => array(
      'group' => t('Islandora Batch'),
      'join' => array(
        'islandora_batch_queue' => array(
          'left_field' => 'id',
          'field' => 'id',
        ),
      ),
    ),
    'id' => array(
      'title' => t('ID'),
      'help' => t('Identifier of an item in the queue.'),
    ),
    'message' => array(
      'title' => t('Item Message'),
      'help' => t('The message for an item in the queue.'),
      'field' => array(
        'handler' => 'views_handler_field',
      ),
    ),
  );
  return $data;
}
