<?php

/**
 * @file
 * Provides nodequeue (subqueue) rules integration.
 */

/**
 * Implementation of hook_rules_event_info().
 *
 * Events are triggered by invokations of rules_invoke_event('EVENT_NAME', $nodequeue, $node);
 */
function nodequeue_rules_event_info() {
  $items = array();

  $vars = array(
    'nodequeue' => array(
      'label' => t('Changed nodequeue'),
      'type' => 'nodequeue',
    ),
    'node' => array(
      'type' => 'node',
      'label' => t('Node'),
    ),
  );

  $items['nodequeue_added'] = array(
    'label' => t('Node has been added to a nodequeue'),
    'group' => t('Nodequeue'),
    'variables' => $vars,
  );

  $items['nodequeue_removed'] = array(
    'label' => t('Node has been removed from a nodequeue'),
    'group' => t('Nodequeue'),
    'variables' => $vars,
  );

  $items['nodequeue_saved'] = array(
    'label' => t('A nodequeue has been updated'),
    'group' => t('Nodequeue'),
    'variables' => array(
      'nodequeue' => array(
        'label' => t('Changed nodequeue'),
        'type' => 'nodequeue',
      ),
      'nodes' => array(
        'label' => t('Nodes'),
        'type' => 'nodes'
      ),
    )
  );

  $items['nodequeue_node_changed'] = array(
    'label' => t('A nodequeue\'s node has been updated'),
    'group' => t('Nodequeue'),
    'variables' => $vars,
  );

  return $items;
}

/**
 * Implementation of hook_rules_data_type_info().
 *
 * We define a nodequeue data type to be used as a ruleset argument.
 */
function nodequeue_rules_data_info() {
  return array(
    'nodequeue' => array(
      'label' => 'nodequeue',
      'wrap' => FALSE,
      'wrapper class' => 'NodequeueRulesDataWrapper'
    ),
    'nodes' => array(
      'label' => 'nodes',
      'wrap' => FALSE,
      'wrapper class' => 'NodeIdArrayRulesDataWrapper',
    ),
  );
}

/**
 * Defines rules data wrapper for nodequeue type.
 */
class NodequeueRulesDataWrapper extends RulesIdentifiableDataWrapper {

  protected function extractIdentifier($nodequeue) {
    $nodequeue = current($nodequeue);
    return $nodequeue->qid;
  }

  protected function load($sqid){
    return nodequeue_load_subqueue($sqid);
  }

}

/**
 * Defines rules data wrapper for array of node nids.
 */
class NodeIdArrayRulesDataWrapper extends RulesIdentifiableDataWrapper {

  protected function extractIdentifier($rules_node_data) {
    // Check whether we have an array of nids or just a node object
    if (is_array($rules_node_data)) {
      $nids = array();
      foreach ($rules_node_data as $node_data) {
        $nids[] = $node_data['nid'];
      }
      return $nids;
    }
    else {
      return $rules_node_data;
    }

  }

  protected function load($nids) {
    if (is_array($nids)) {
      $nodes = array();
      foreach ($nids as $nid) {
        $nodes[] = node_load($nid);
      }
      return $nodes;
    }
    // We have a loaded node object
    else {
      $node = $nids;
      return $nids;
    }
  }
}

/**
 * Implementation of hook_rules_condition_info().
 *
 * As we have generic events, e. g. node added to nodequeue,
 * we might also want to identify and filter by certain nodequeues.
 */
function nodequeue_rules_condition_info() {
  $conditions = array();

  $conditions['nodequeue_rules_condition_belongs_to_nodequeue'] = array(
    'label' => t('Belongs to Nodequeue(s)'),
    'group' => t('Nodequeue'),
    'parameter' => array(
      'nodequeue' => array(
        'type' => 'nodequeue',
        'label' => t('Nodequeue'),
      ),
      'qid' => array(
        'type' => 'list<text>',
        'label' => t('Queue ID of nodequeue to check'),
        'options list' => 'nodequeue_rules_qid_options_list',
        'multiple' => TRUE,
      ),
    ),
  );
  $conditions['nodequeue_rules_condition_node_belongs_to_nodequeue'] = array(
    'label' => t('Node Belongs to Nodequeue(s)'),
    'group' => t('Nodequeue'),
    'parameter' => array(
      'node' => array(
        'type' => 'node',
        'label' => t('Node'),
      ),
      'qids' => array(
        'type' => 'list<text>',
        'label' => t('Queue ID(s) of nodequeue to check'),
        'options list' => 'nodequeue_rules_qid_options_list',
        'multiple' => TRUE,
      ),
    ),
  );

  return $conditions;
}

/**
 * Helper function for rules condition options list
 */
function nodequeue_rules_qid_options_list() {
  $queues = nodequeue_load_subqueues_by_queue(array_keys(nodequeue_get_all_qids()));
  foreach ($queues as $sqid => $queue) {
    $queuenames[$sqid] = $queue->title;
  }
  return $queuenames;
}

/**
 * The condition verifies if the provided nodequeue argument is contained within the configured nodequeues.
 *
 * @param $nodequeue
 *   nodequeue (subqueue) object
 * @param $qid
 *   list of nodequeue ID's
 * @return boolean
 *   boolean indicating whether the conditions applies
 */
function nodequeue_rules_condition_belongs_to_nodequeue($nodequeue, $qid) {
  if (in_array($nodequeue->qid, $qid)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * The condition verifies if the provided nodequeue argument is contained within the configured nodequeues.
 *
 * @param $node
 * @param $qids
 *
 * @return bool
 *   boolean indicating whether the conditions applies
 *
 * @internal param $nodequeue
 *   nodequeue (subqueue) object
 * @internal param $qid
 *   list of nodequeue ID's
 */
function nodequeue_rules_condition_node_belongs_to_nodequeue($node, $qids) {
  $queues = nodequeue_load_queues(array_keys($qids));
  $nodesqueues = nodequeue_get_subqueues_by_node($queues, $node);
  if (!empty($nodesqueues)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

/**
 * Implements hook_rules_action_info().
 */
function nodequeue_rules_action_info() {
  return array(
    // Action: Add a given node to the given nodequeue.
    'nodequeue_action_add_node' => array(
      'label' => t('Add node to nodequeue'),
      'group' => t('Nodequeue'),
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('Node'),
          'description' => t('The node you want to add.'),
        ),
        'queue' => array(
          'type' => 'text',
          'label' => t('Queue'),
          'description' => t('The queue where the node should be added to.'),
          'options list' => 'nodequeue_get_queue_names',
        ),
        'unique' => array(
            'type' => 'boolean',
            'label' => t('Prevent duplication'),
            'description' => t('Selecting this option will prevent the node from being added more than once to any subqueue.'),
            'default value' => TRUE,
          )
      ),
    ),
    // Action: Remove the given node from the given nodequeue.
    'nodequeue_action_remove_node' => array(
      'label' => t('Remove node from nodequeue'),
      'group' => t('Nodequeue'),
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('Node'),
          'description' => t('The node you want to remove.'),
        ),
        'queue' => array(
          'type' => 'text',
          'label' => t('Queue'),
          'description' => t('The queue where the node should be removed from.'),
          'options list' => 'nodequeue_get_queue_names',
        ),
      ),
    ),
  );
}

/**
 * Action: Add the given node to the given nodequeue.
 */
function nodequeue_action_add_node($node, $queue_name, $unique) {
  $queue = nodequeue_load_queue_by_name($queue_name);
  $smartqueue_enabled = function_exists('smartqueue_taxonomy_nodequeue_subqueues') ? TRUE : FALSE;
  if ($smartqueue_enabled) {
    $references = smartqueue_taxonomy_nodequeue_subqueues($queue, $node);
  }

  $smartqueue_domain_enabled = function_exists('smartqueue_domain_nodequeue_subqueues') ? TRUE : FALSE;
  if ($smartqueue_domain_enabled) {
    $subqueue_domain = smartqueue_domain_nodequeue_subqueues($queue, $node);
    $node_domains = nodequeue_get_node_domains($node);
  }
  $subqueues = nodequeue_load_subqueues_by_queue($queue->qid);
  foreach ($subqueues as $subqueue) {
    $add = TRUE;
    // unique means we don't add it if it's in the queue already
    if ($unique) {
      $add = nodequeue_subqueue_position($subqueue->sqid, $node->nid) === FALSE;
    }
    if ($add) {
      if ($smartqueue_domain_enabled) {
        // Do not allow nodes to be added to queues on domains they are not publishted to.
        // The domain id is the last part of the queue reference string.
        // All smartqueue_domain subqueue references also contain ":domain:".
        if (!empty($subqueue->reference) && !empty($node_domains)){
          $reference_array = explode(':', $subqueue->reference);
          $queue_domain_id = end($reference_array);
          if (strpos($subqueue->reference, ':domain:') && in_array($queue_domain_id, $node_domains)) {
            nodequeue_subqueue_add($queue, $subqueue, $node->nid);
          }
        }
      }
      if ($smartqueue_enabled) {
        if (in_array($subqueue->reference, $references)) {
          nodequeue_subqueue_add($queue, $subqueue, $node->nid);
        }
      }
      if (!$smartqueue_domain_enabled && !$smartqueue_domain_enabled){
        nodequeue_subqueue_add($queue, $subqueue, $node->nid);
      }
    }
  }
}

/**
 * Action: Remove the given node from the given nodequeue.
 */
function nodequeue_action_remove_node($node, $queue_name) {
  $queue = nodequeue_load_queue_by_name($queue_name);
  $subqueues = nodequeue_load_subqueues_by_queue($queue->qid);
  foreach ($subqueues as $subqueue) {
    if (!$queue->i18n || (isset($node->tnid) && empty($node->tnid))) {
      nodequeue_subqueue_remove_node($subqueue->sqid, $node->nid);
    }
  }
}

/**
 * Return a list of nodequeue names.
 */
function nodequeue_get_queue_names() {
  $items = array();
  $queues = nodequeue_load_queues(nodequeue_get_all_qids());
  foreach ($queues as $queue) {
    $items[$queue->name] = $queue->title;
  }
  return $items;
}

/**
 * Helper function for fetching possible domain ids for a node.
 */
function nodequeue_get_node_domains($node){
  $node_domains = array();
  if (empty($node->nid) || !function_exists('domain_get_node_domains')){
    return $node_domains;
  }
  $domain_data = domain_get_node_domains($node->nid);
  if (!empty($domain_data['domain_id'])){
    return $domain_data['domain_id'];
  }
  return $node_domains;
}
