<?php

/**
 * @file
 * Default views hook.
 */

/**
 * Implements hook_views_default_views().
 */
function islandora_batch_views_default_views() {
  $views = array();

  $view = new view();
  $view->name = 'islandora_batch_queue';
  $view->description = '';
  $view->tag = 'islandora_batch';
  $view->base_table = 'islandora_batch_queue';
  $view->human_name = 'Islandora Batch Queue';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Islandora Batch Ingest Queue';
  $handler->display->display_options['css_class'] = 'islandora-batch-view-table';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'id' => 'id',
    'state' => 'state',
    'parent' => 'parent',
    'sid' => 'sid',
    'id_1' => 'id_1',
    'id_2' => 'id_2',
    'contextual_links' => 'contextual_links',
  );
  $handler->display->display_options['style_options']['default'] = '-1';
  $handler->display->display_options['style_options']['info'] = array(
    'id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'state' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'parent' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'sid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'id_1' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'id_2' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'contextual_links' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
  );
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No items available.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Islandora Batch: Associated Set */
  $handler->display->display_options['relationships']['sid']['id'] = 'sid';
  $handler->display->display_options['relationships']['sid']['table'] = 'islandora_batch_queue';
  $handler->display->display_options['relationships']['sid']['field'] = 'sid';
  /* Relationship: Islandora Batch: Set Creator */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'islandora_batch_set';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['relationship'] = 'sid';
  /* Field: Islandora Batch: ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'islandora_batch_queue';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['alter']['make_link'] = TRUE;
  $handler->display->display_options['fields']['id']['alter']['path'] = 'islandora/object/[id]';
  $handler->display->display_options['fields']['id']['alter']['target'] = '_blank';
  /* Field: Islandora Batch: Item State */
  $handler->display->display_options['fields']['state']['id'] = 'state';
  $handler->display->display_options['fields']['state']['table'] = 'islandora_batch_state';
  $handler->display->display_options['fields']['state']['field'] = 'state';
  $handler->display->display_options['fields']['state']['label'] = 'State';
  $handler->display->display_options['fields']['state']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['state']['alter']['text'] = '[state-text]';
  /* Field: Islandora Batch: Item Message */
  $handler->display->display_options['fields']['message']['id'] = 'message';
  $handler->display->display_options['fields']['message']['table'] = 'islandora_batch_queue_messages';
  $handler->display->display_options['fields']['message']['field'] = 'message';
  $handler->display->display_options['fields']['message']['label'] = 'Message';
  $handler->display->display_options['fields']['message']['hide_empty'] = TRUE;
  /* Field: Islandora Batch: Parent */
  $handler->display->display_options['fields']['parent']['id'] = 'parent';
  $handler->display->display_options['fields']['parent']['table'] = 'islandora_batch_queue';
  $handler->display->display_options['fields']['parent']['field'] = 'parent';
  /* Field: Islandora Batch: Set ID */
  $handler->display->display_options['fields']['sid']['id'] = 'sid';
  $handler->display->display_options['fields']['sid']['table'] = 'islandora_batch_queue';
  $handler->display->display_options['fields']['sid']['field'] = 'sid';
  /* Field: Islandora Batch: Item Actions */
  $handler->display->display_options['fields']['actions']['id'] = 'actions';
  $handler->display->display_options['fields']['actions']['table'] = 'islandora_batch_queue';
  $handler->display->display_options['fields']['actions']['field'] = 'actions';
  $handler->display->display_options['fields']['actions']['label'] = '';
  $handler->display->display_options['fields']['actions']['element_label_colon'] = FALSE;
  /* Contextual filter: Islandora Batch: Set ID */
  $handler->display->display_options['arguments']['sid']['id'] = 'sid';
  $handler->display->display_options['arguments']['sid']['table'] = 'islandora_batch_queue';
  $handler->display->display_options['arguments']['sid']['field'] = 'sid';
  $handler->display->display_options['arguments']['sid']['title_enable'] = TRUE;
  $handler->display->display_options['arguments']['sid']['title'] = 'Set %1 Batch Queue';
  $handler->display->display_options['arguments']['sid']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['sid']['default_argument_options']['argument'] = 'all';
  $handler->display->display_options['arguments']['sid']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['sid']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['sid']['summary_options']['items_per_page'] = '25';
  /* Filter criterion: Islandora Batch: Item State */
  $handler->display->display_options['filters']['state']['id'] = 'state';
  $handler->display->display_options['filters']['state']['table'] = 'islandora_batch_state';
  $handler->display->display_options['filters']['state']['field'] = 'state';
  $handler->display->display_options['filters']['state']['group'] = 1;
  $handler->display->display_options['filters']['state']['exposed'] = TRUE;
  $handler->display->display_options['filters']['state']['expose']['operator_id'] = 'state_op';
  $handler->display->display_options['filters']['state']['expose']['label'] = 'Item State';
  $handler->display->display_options['filters']['state']['expose']['operator'] = 'state_op';
  $handler->display->display_options['filters']['state']['expose']['identifier'] = 'state';
  $handler->display->display_options['filters']['state']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['state']['group_info']['label'] = 'Item State';
  $handler->display->display_options['filters']['state']['group_info']['identifier'] = 'state';
  $handler->display->display_options['filters']['state']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Error',
      'operator' => '=',
      'value' => array(
        'value' => '-1',
        'min' => '',
        'max' => '',
      ),
    ),
    2 => array(
      'title' => 'Not Ready',
      'operator' => '=',
      'value' => array(
        'value' => '0',
        'min' => '',
        'max' => '',
      ),
    ),
    3 => array(
      'title' => 'Ready',
      'operator' => '=',
      'value' => array(
        'value' => '1',
        'min' => '',
        'max' => '',
      ),
    ),
    4 => array(
      'title' => 'Children Pending',
      'operator' => '=',
      'value' => array(
        'value' => '2',
        'min' => '',
        'max' => '',
      ),
    ),
    5 => array(
      'title' => 'Ingested',
      'operator' => '=',
      'value' => array(
        'value' => '3',
        'min' => '',
        'max' => '',
      ),
    ),
  );

  /* Display: User Queue Page */
  $handler = $view->new_display('page', 'User Queue Page', 'islandora_batch_queue_page');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Your Batch Ingest Queue';
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'ingest fedora objects';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Islandora Batch: Item State */
  $handler->display->display_options['filters']['state']['id'] = 'state';
  $handler->display->display_options['filters']['state']['table'] = 'islandora_batch_state';
  $handler->display->display_options['filters']['state']['field'] = 'state';
  $handler->display->display_options['filters']['state']['group'] = 1;
  $handler->display->display_options['filters']['state']['exposed'] = TRUE;
  $handler->display->display_options['filters']['state']['expose']['operator_id'] = 'state_op';
  $handler->display->display_options['filters']['state']['expose']['label'] = 'Item State';
  $handler->display->display_options['filters']['state']['expose']['operator'] = 'state_op';
  $handler->display->display_options['filters']['state']['expose']['identifier'] = 'state';
  $handler->display->display_options['filters']['state']['is_grouped'] = TRUE;
  $handler->display->display_options['filters']['state']['group_info']['label'] = 'Item State';
  $handler->display->display_options['filters']['state']['group_info']['identifier'] = 'state';
  $handler->display->display_options['filters']['state']['group_info']['group_items'] = array(
    1 => array(
      'title' => 'Error',
      'operator' => '=',
      'value' => array(
        'value' => '-1',
        'min' => '',
        'max' => '',
      ),
    ),
    2 => array(
      'title' => 'Not Ready',
      'operator' => '=',
      'value' => array(
        'value' => '0',
        'min' => '',
        'max' => '',
      ),
    ),
    3 => array(
      'title' => 'Ready',
      'operator' => '=',
      'value' => array(
        'value' => '1',
        'min' => '',
        'max' => '',
      ),
    ),
    4 => array(
      'title' => 'Children Pending',
      'operator' => '=',
      'value' => array(
        'value' => '2',
        'min' => '',
        'max' => '',
      ),
    ),
    5 => array(
      'title' => 'Ingested',
      'operator' => '=',
      'value' => array(
        'value' => '3',
        'min' => '',
        'max' => '',
      ),
    ),
  );
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid_current']['value'] = '1';
  $handler->display->display_options['path'] = 'islandora_batch/reports/queue';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Islandora Batch Queue';
  $handler->display->display_options['menu']['description'] = 'Your items in the ingest queue.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Admin Queue Page */
  $handler = $view->new_display('page', 'Admin Queue Page', 'islandora_batch_queue_admin');
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'islandora_batch_manage_queue';
  $handler->display->display_options['path'] = 'admin/reports/islandora_batch_queue';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Islandora Batch Queue';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['islandora_batch_queue'] = array(
    t('Master'),
    t('Islandora Batch Ingest Queue'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Displaying @start - @end of @total'),
    t('No items available.'),
    t('Associated Set'),
    t('Set creator'),
    t('ID'),
    t('State'),
    t('[state-text]'),
    t('Message'),
    t('Parent'),
    t('Set ID'),
    t('.'),
    t(','),
    t('All'),
    t('Set %1 Batch Queue'),
    t('Item State'),
    t('User Queue Page'),
    t('Your Batch Ingest Queue'),
    t('Admin Queue Page'),
  );

  $views['islandora_batch_queue'] = $view;

  $view = new view();
  $view->name = 'islandora_batch_sets';
  $view->description = '';
  $view->tag = 'islandora_batch';
  $view->base_table = 'islandora_batch_set';
  $view->human_name = 'Islandora Batch Sets';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Islandora Batch Ingest Sets';
  $handler->display->display_options['css_class'] = 'islandora-batch-view-table';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '100';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'id' => 'id',
    'name' => 'name',
    'created' => 'created',
    'id_1' => 'id_1',
    'nothing' => 'nothing',
    'nothing_3' => 'nothing_3',
    'nothing_1' => 'nothing_1',
    'nothing_2' => 'nothing_2',
    'contextual_links' => 'contextual_links',
  );
  $handler->display->display_options['style_options']['default'] = 'created';
  $handler->display->display_options['style_options']['info'] = array(
    'id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'id_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nothing' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'nothing_3' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'nothing_1' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'nothing_2' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'contextual_links' => array(
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
  );
  /* Header: Global: Result summary */
  $handler->display->display_options['header']['result']['id'] = 'result';
  $handler->display->display_options['header']['result']['table'] = 'views';
  $handler->display->display_options['header']['result']['field'] = 'result';
  /* No results behavior: Global: Text area */
  $handler->display->display_options['empty']['area']['id'] = 'area';
  $handler->display->display_options['empty']['area']['table'] = 'views';
  $handler->display->display_options['empty']['area']['field'] = 'area';
  $handler->display->display_options['empty']['area']['empty'] = TRUE;
  $handler->display->display_options['empty']['area']['content'] = 'No sets available.';
  $handler->display->display_options['empty']['area']['format'] = 'filtered_html';
  /* Relationship: Islandora Batch: Set Creator */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'islandora_batch_set';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['required'] = TRUE;
  /* Field: Islandora Batch: Set ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'islandora_batch_set';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Set Creator';
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: Islandora Batch: Created Date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'islandora_batch_set';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'long';
  /* Field: COUNT(Islandora Batch: ID) */
  $handler->display->display_options['fields']['id_1']['id'] = 'id_1';
  $handler->display->display_options['fields']['id_1']['table'] = 'islandora_batch_queue';
  $handler->display->display_options['fields']['id_1']['field'] = 'id';
  $handler->display->display_options['fields']['id_1']['group_type'] = 'count';
  $handler->display->display_options['fields']['id_1']['label'] = 'Set Size';
  /* Field: Islandora Batch: Set Actions */
  $handler->display->display_options['fields']['actions']['id'] = 'actions';
  $handler->display->display_options['fields']['actions']['table'] = 'islandora_batch_set';
  $handler->display->display_options['fields']['actions']['field'] = 'actions';
  $handler->display->display_options['fields']['actions']['label'] = '';
  $handler->display->display_options['fields']['actions']['element_label_colon'] = FALSE;

  /* Display: User Set Page */
  $handler = $view->new_display('page', 'User Set Page', 'islandora_batch_set_page');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Your Batch Ingest Sets';
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'ingest fedora objects';
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: User: Current */
  $handler->display->display_options['filters']['uid_current']['id'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['table'] = 'users';
  $handler->display->display_options['filters']['uid_current']['field'] = 'uid_current';
  $handler->display->display_options['filters']['uid_current']['relationship'] = 'uid';
  $handler->display->display_options['filters']['uid_current']['value'] = '1';
  $handler->display->display_options['path'] = 'islandora_batch/reports/set';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Islandora Batch Sets';
  $handler->display->display_options['menu']['description'] = 'List of the current user\'s batch sets';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Admin Set Page */
  $handler = $view->new_display('page', 'Admin Set Page', 'islandora_batch_sets_admin');
  $handler->display->display_options['defaults']['access'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'islandora_batch_manage_queue';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Islandora Batch: Set ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'islandora_batch_set';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name']['label'] = 'Set Creator';
  $handler->display->display_options['fields']['name']['link_to_user'] = FALSE;
  /* Field: Islandora Batch: Created Date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'islandora_batch_set';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['date_format'] = 'long';
  /* Field: COUNT(Islandora Batch: ID) */
  $handler->display->display_options['fields']['id_1']['id'] = 'id_1';
  $handler->display->display_options['fields']['id_1']['table'] = 'islandora_batch_queue';
  $handler->display->display_options['fields']['id_1']['field'] = 'id';
  $handler->display->display_options['fields']['id_1']['group_type'] = 'count';
  $handler->display->display_options['fields']['id_1']['label'] = 'Set Size';
  /* Field: Islandora Batch: Set Actions */
  $handler->display->display_options['fields']['actions']['id'] = 'actions';
  $handler->display->display_options['fields']['actions']['table'] = 'islandora_batch_set';
  $handler->display->display_options['fields']['actions']['field'] = 'actions';
  $handler->display->display_options['fields']['actions']['label'] = '';
  $handler->display->display_options['fields']['actions']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['actions']['queue_path'] = 'admin/reports/islandora_batch_queue';
  $handler->display->display_options['path'] = 'admin/reports/islandora_batch_sets';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Islandora Batch Sets';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
  $translatables['islandora_batch_sets'] = array(
    t('Master'),
    t('Islandora Batch Ingest Sets'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Displaying @start - @end of @total'),
    t('No sets available.'),
    t('Set creator'),
    t('Set ID'),
    t('.'),
    t(','),
    t('Set Creator'),
    t('Created Date'),
    t('Set Size'),
    t('User Set Page'),
    t('Your Batch Ingest Sets'),
    t('Admin Set Page'),
  );

  $views['islandora_batch_sets'] = $view;

  return $views;
}
