<?php

/**
 * Triggered when a node is added to a sub-queue, after adding the sub-queue.
 *
 * This hook is invoked after the update has been sent to apachesolr module, if
 * it is enabled.
 *
 * @param int $qid
 *   The sub-queue id.
 * @param int $nid
 *   The node id
 *
 * @see nodequeue_subqueue_add()
 */
function hook_nodequeue_add($qid, $nid) {
}

/**
 * Triggered when nodes are removed from a sub-queue.
 *
 * This is invoked in a loop for all removed nodes, after the queue has been
 * updated.
 *
 * @param int $qid
 *   The subqueue id.
 * @param int $nid
 *   The node id
 *
 * @see nodequeue_subqueue_remove()
 */
function hook_nodequeue_remove($qid, $nid) {
}

/**
 * Triggered when a node changes position within a sub-queue, after the swap.
 *
 * @param int $qid
 *   The subqueue id.
 * @param int $nid
 *   The node id
 *
 * @see nodequeue_queue_swap()
 */
function hook_nodequeue_swap($qid, $nid) {
}

/**
 * Expose nodequeue info.
 *
 * @see nodequeue_api_info()
 */
function hook_nodequeue_info() {
  // This example show how the built-in queues are declared.
  return array(
    'nodequeue' => array(
      'title' => t('Simple queue'),
      'description' => t('Simple queues have just one subqueue. Nodes put into a queue are added to the back of the queue; when a node is added to a full queue, the node in the front of the queue will be popped out to make room.'),
    ),
    'smartqueue_taxonomy' => array(
      'title' => t('Taxonomy queue'),
      'description' => t('Each particular grouping of taxonomy terms from the selected vocabularies have their own unique subqueue. You can place nodes into any of these subqueues based on which terms that node has been tagged with. Using this with large or too many taxonomies may degrade performance.'),
    ),
  );
}

/**
 * Allow altering a node reordering action.
 *
 * This is called after the queue is loaded, but before any action is taken.
 *
 * @param array $nodes
 * @param int $sqid
 *
 * @see nodequeue_save_subqueue_order()
 */
function hook_nodequeue_sort_alter($nodes, $sqid) {
}

/**
 * Allow altering a node reordering action.
 *
 * This is called after validation has been performed and nodes removed, but
 * before the new order of the queue is saved. This is the next-before-last
 * nodequeue hook in a reorder operation, the last being hook_nodequeue_update.
 *
 * @param array $nodes
 * @param int $qid
 */
function hook_nodequeue_save_subqueue_order_alter($qid, $nodes) {
  // Invoked from:
  nodequeue_save_subqueue_order();
}

/**
 * Allow modules to react to a node reordering action.
 *
 * This is the last hook in a reordering, after hook_nodequeue_sort_alter() and
 * hook_nodequeue_save_subqueue_order_alter().
 *
 * @param mixed $qid
 *   The queue being reordered.
 * @param mixed $s_qid
 *   The sub-queue being reordered.
 *
 * @see nodequeue_save_subqueue_order()
 *
 * @since After 7.x-2.0.
 */
function hook_nodequeue_update($qid, $s_qid) {

}

/**
 * Allow modules to react to a queue deletion.
 *
 * @param int $qid
 *   The id of the queue for which deletion was attempted.
 * @param array|object
 *   An object containing the deleted queue, if found, an empty array otherwise.
 *
 * @see nodequeue_delete()
 *
 * @since After 7.x-2.0-beta, with just a qid
 * @since After commit dd1f758 with the $queue parameter.
 */
function hook_nodequeue_delete($qid) {
}

/**
 * Allow altering the queue edit form.
 *
 * This hook is actually a callback for the module defining the queue instead
 * of a generic hook.
 *
 * @param object $queue
 *   A queue being edited.
 * @param array $form
 *   The form array to edit the queue.
 *
 * @see nodequeue_api_queue_form()
 */
function hook_nodequeue_form($queue, &$form) {
}

/**
 * Allow validating the queue edit form.
 *
 * This hook is actually a callback for the module defining the queue instead
 * of a generic hook.
 *
 * @param object $queue
 * @param array $form_state
 * @param array $form
 *
 * @see nodequeue_api_queue_form_validate()
 */
function hook_nodequeue_form_validate($queue, $form_state, $form) {
}

/**
 * Allow submitting the queue edit form. First pass.
 *
 * This hook is actually a callback for the module defining the queue instead
 * of a generic hook.
 *
 * @param object $queue
 * @param array $form_state
 *
 * @see nodequeue_api_queue_form_submit()
 */
function hook_nodequeue_form_submit($queue, $form_state) {
}

/**
 * Allow submitting the queue edit form. Second pass.
 *
 * This hook is actually a callback for the module defining the queue instead
 * of a generic hook.
 *
 * @param stdClass $queue
 * @param array $form_state
 *
 * @see nodequeue_api_queue_form_submit()
 */
function hook_nodequeue_form_submit_finish($queue, $form_state) {
}

/**
 * Allow providing a list of valid sub-queues for a node within a queue.
 *
 * This hook is actually a callback for the module defining the queue instead
 * of a generic hook.
 *
 * @param stdClass $queue
 * @param stdClass $node
 *   A fully loaded node object
 *
 * @see nodequeue_api_subqueues()
 */
function hook_nodequeue_subqueues($queue, $node) {
}

/**
 * Provide an alternative way to provide auto-complete results.
 *
 * If this hook is implemented, the builtin query in nodequeue is not used, and
 * the results from this hook are returned instead.
 *
 * This hook is actually a callback for the module defining the queue instead
 * of a generic hook.
 *
 * @param object $queue
 * @param object $subqueue
 * @param string $string
 * @param object $query
 *
 * @see nodequeue_api_autocomplete()
 */
function hook_nodequeue_autocomplete($queue, $subqueue, $string, $query) {
}

/**
 * Provide queue access control.
 *
 * This hook is actually a callback for the module defining the queue instead
 * of a generic hook.
 *
 * @param stdClass $queue
 * @param stdClass $account
 *
 * @return NULL|TRUE|FALSE
 *   NULL and TRUE grant access. FALSE rejects it.
 *
 * @see nodequeue_api_queue_access()
 */
function hook_queue_access($queue, $account) {
}

/**
 * Provide sub-queue access control.
 *
 * This hook is actually a callback for the module defining the queue instead
 * of a generic hook.
 *
 * @param stdClass $sub_queue
 * @param stdClass $account
 * @param stdClass $queue
 *
 * @return NULL|TRUE|FALSE
 *   NULL and TRUE grant access. FALSE refuses it.
 *
 * @see nodequeue_api_subqueue_access()
 */
function hook_subqueue_access($sub_queue, $account, $queue) {
}

/**
 * Allow modifying queues being loaded.
 *
 * @param array $loaded
 *   A qid-indexed hash of queues
 * @param string $context
 *   This hook receives the constant string 'load_queues' in this parameter, and
 *   its value is unused by the caller afterwards.
 *
 * @see nodequeue_load_queues()
 */
function hook_nodequeue_alter($loaded, $context) {
}

/**
 * Allow modifying the table setup before output.
 *
 * For example, it's possible to add some columns in the table
 * with the help of this hook.
 *
 * @param array $form
 *   The form to access submitted values.
 * @param array $header
 *   The table headers before sending it to theming layer.
 * @param array $rows
 *   The table rows before sending it to theming layer.
 *
 * @see theme_nodequeue_arrange_subqueue_form_table().
 */
function hook_nodequeue_arrange_subqueue_form_alter($form, $header, $rows) {
}