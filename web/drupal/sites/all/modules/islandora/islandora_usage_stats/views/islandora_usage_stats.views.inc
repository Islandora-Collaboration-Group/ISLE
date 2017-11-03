<?php
/**
 * @file
 * Views integration.
 */

/**
 * Implements hook_views_data().
 */
function islandora_usage_stats_views_data() {
  $data = array();
  $data['islandora_usage_stats_objects'] = array(
    'table' => array(
      'group' => t('Islandora Usage Stats'),
      'base' => array(
        'field' => 'id',
        'title' => t('Islandora Usage Stats Objects'),
        'help' => t('Object information.'),
      ),
    ),
    'id' => array(
      'title' => t('Object ID'),
      'help' => t('ID of the object.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'label' => array(
      'title' => t('Label'),
      'help' => t('Label of the object.'),
      'field' => array(
        'handler' => 'views_handler_field',
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
    'pid' => array(
      'title' => t('PID'),
      'help' => t('PID of the object.'),
      'field' => array(
        'handler' => 'views_handler_field',
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
  );
  $data['islandora_usage_stats_datastreams'] = array(
    'table' => array(
      'group' => t('Islandora Usage Stats'),
      'base' => array(
        'field' => 'id',
        'title' => t('Islandora Usage Stats Datastreams'),
        'help' => t('Stores information about DSID and PID. Useful in relation to the datastream downloads data.'),
      ),
    ),
    'id' => array(
      'title' => t('Datastream ID'),
      'help' => t('ID of the Datastream'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'dsid' => array(
      'title' => t('DS Type'),
      'help' => t('ID of the DS'),
      'field' => array(
        'handler' => 'views_handler_field',
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
    'pid_id' => array(
      'title' => t('PID ID'),
      'help' => t('ID of the parent object.'),
      'relationship' => array(
        'base' => 'islandora_usage_stats_objects',
        'base field' => 'id',
        'handler' => 'views_handler_relationship',
        'label' => t('Object'),
        'title' => t('Object'),
      ),
    ),
    'ds_label' => array(
      'title' => t('DS Label'),
      'help' => t('Label of the datastream.'),
      'field' => array(
        'handler' => 'views_handler_field',
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
  );
  $data['islandora_usage_stats_object_access_log'] = array(
    'table' => array(
      'group' => t('Islandora Usage Stats'),
      'base' => array(
        'field' => 'id',
        'title' => t('Islandora Usage Stats Object Access'),
        'help' => t('The log of objects being visited.'),
      ),
    ),
    'id' => array(
      'title' => t('Log ID'),
      'help' => t('ID of the log entry.'),
      'relationship' => array(
        'base' => 'islandora_usage_stats_collection_access_log',
        'base field' => 'object_access_id',
        'handler' => 'views_handler_relationship',
        'label' => t('Collection'),
        'title' => t('Collection'),
      ),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'pid_id' => array(
      'title' => t('PID ID'),
      'help' => t('ID of the object.'),
      'relationship' => array(
        'base' => 'islandora_usage_stats_objects',
        'base field' => 'id',
        'handler' => 'views_handler_relationship',
        'label' => t('Object'),
        'title' => t('Object'),
      ),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'ip' => array(
      'title' => t('IP'),
      'help' => t('IP of user that accessed the object.'),
      'field' => array(
        'handler' => 'views_handler_field',
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
    'uid' => array(
      'title' => t('User ID'),
      'help' => t('ID of the Drupal user that accessed the object.'),
      'field' => array(
        'handler' => 'views_handler_field',
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
    'time' => array(
      'title' => t('Time'),
      'help' => t('Time the view is being logged.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
        'handler' => 'datepicker_views_filter_handler',
        'is date' => TRUE,
      ),
    ),
  );
  $data['islandora_usage_stats_object_ds_access_log'] = array(
    'table' => array(
      'group' => t('Islandora Usage Stats'),
      'base' => array(
        'field' => 'id',
        'title' => t('Islandora Usage Stats Object Datastream Downloads'),
        'help' => t('Tracks the downloads of datastreams from Islandora objects.'),
      ),
    ),
    'id' => array(
      'title' => t('Log ID'),
      'help' => t('ID of the log entry.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'ds_id' => array(
      'title' => t('DS ID'),
      'help' => t('ID of the datastream.'),
      'relationship' => array(
        'base' => 'islandora_usage_stats_datastreams',
        'base field' => 'id',
        'handler' => 'views_handler_relationship',
        'label' => t('Datastream'),
        'title' => t('Datastream'),
      ),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'ip' => array(
      'title' => t('IP'),
      'help' => t('IP of user that accessed the object.'),
      'field' => array(
        'handler' => 'views_handler_field',
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
    'uid' => array(
      'title' => t('User ID'),
      'help' => t('ID of the Drupal user that accessed the object.'),
      'field' => array(
        'handler' => 'views_handler_field',
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
    'time' => array(
      'title' => t('Time'),
      'help' => t('Time the view is being logged.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
        'handler' => 'datepicker_views_filter_handler',
        'is date' => TRUE,
      ),
    ),
  );
  $data['islandora_usage_stats_collection_access_log'] = array(
    'table' => array(
      'group' => t('Islandora Usage Stats'),
      'base' => array(
        'field' => 'id',
        'title' => t('Islandora Usage Stats Collection Access'),
        'help' => t('The log of the parents of objects that are visited.'),
      ),
    ),
    'collection' => array(
      'title' => t('Collection'),
      'help' => t('The PID ID of the parent of the object being logged.'),
      'relationship' => array(
        'base' => 'islandora_usage_stats_objects',
        'base field' => 'id',
        'handler' => 'views_handler_relationship',
        'label' => t('Collection Object'),
        'title' => t('Collection Object'),
      ),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'id' => array(
      'title' => t('ID'),
      'help' => t('ID for a logged parent.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'object_access_id' => array(
      'title' => t('Log ID'),
      'help' => t('ID of the log entry.'),
      'relationship' => array(
        'base' => 'islandora_usage_stats_object_access_log',
        'base field' => 'id',
        'handler' => 'views_handler_relationship',
        'label' => t('Object Collection Access'),
        'title' => t('Object Collection Access'),
      ),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
  );
  $data['islandora_usage_stats_content_model_access_log'] = array(
    'table' => array(
      'group' => t('Islandora Usage Stats'),
      'base' => array(
        'field' => 'id',
        'title' => t('Islandora Usage Stats Content Model Access'),
        'help' => t('The log of content models of objects that are visited.'),
      ),
    ),
    'content_model' => array(
      'title' => t('Content Model'),
      'help' => t('The PID ID of the content model of the object being logged.'),
      'relationship' => array(
        'base' => 'islandora_usage_stats_objects',
        'base field' => 'id',
        'handler' => 'views_handler_relationship',
        'label' => t('Content Model Object'),
        'title' => t('Content Model Object'),
      ),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'id' => array(
      'title' => t('ID'),
      'help' => t('ID for a logged content model.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'object_access_id' => array(
      'title' => t('Log ID'),
      'help' => t('ID of the log entry.'),
      'relationship' => array(
        'base' => 'islandora_usage_stats_object_access_log',
        'base field' => 'id',
        'handler' => 'views_handler_relationship',
        'label' => t('Object Content Model Access'),
        'title' => t('Object Content Model Access'),
      ),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
  );
  return $data;
}
