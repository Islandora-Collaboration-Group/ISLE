<?php

/**
 * @file
 * This file contains all the batch operations for collection objects.
 */

require_once dirname(__FILE__) . '/utilities.inc';

/**
 * Starts the batch operation.
 *
 * @param array $operands
 *   All the objects to operate on.
 * @param array $context
 *   The context of this batch operation.
 */
function islandora_basic_collection_start_operation(array $operands, array &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['total'] = count($operands);
  }
  return @$operands[$context['sandbox']['progress']];
}

/**
 * Ends the batch operation.
 *
 * @param array $context
 *   The context of this batch operation.
 *
 * @return bool
 *   TRUE or FALSE depending on the context of the batch.
 */
function islandora_basic_collection_end_operation(array &$context) {
  $context['sandbox']['progress']++;
  if ($context['sandbox']['progress'] < $context['sandbox']['total']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['total'];
    return FALSE;
  }
  return TRUE;
}

/**
 * The batch 'finished' callback.
 */
function islandora_basic_collection_batch_finished($success, $results, $operations) {
  if (!$success) {
    // Generic Error Message.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array(
                 '%error_operation' => $error_operation[0],
                 '@arguments' => print_r($error_operation[1], TRUE)));
    drupal_set_message($message, 'error');
  }
}

/**
 * Delete the given child objects within the collection.
 *
 * If no child objects are given its assumed all child objects are to be
 * deleted.
 *
 * @param AbstractObject $collection
 *   The collection object to purge the children from.
 * @param mixed $children
 *   An array of PID's of the children to delete.  Defualts to NULL.  If NULL
 *   all children will be deleted.
 *
 * @return array
 *   The batch definition.
 */
function islandora_basic_collection_delete_children_batch(AbstractObject $collection, $children = NULL) {
  $batch = array(
    'finished' => 'islandora_basic_collection_batch_finished',
    'progress_message' => t('Time elapsed: @elapsed <br/>Estimated time remaining @estimate.'),
    'error_message' => t('An error has occurred.'),
    'file' => drupal_get_path('module', 'islandora_basic_collection') . '/includes/batch.inc',
  );

  $message_parameters = array('@collection' => $collection->label);
  if (is_array($children)) {
    $child_count = count($children);
    $batch += array(
      'operations' => array(
        array(
          'islandora_basic_collection_delete_children_batch_operation',
          array($collection, $children),
        ),
      ),
      'title' => format_plural($child_count,
        'Deleting 1 child from @collection ...',
        'Deleting @count children from @collection ...', $message_parameters),
      'init_message' => format_plural($child_count,
        'Preparing to delete 1 child from @collection ...',
        'Preparing to delete @count children from @collection ...', $message_parameters),
    );
  }
  else {
    $batch += array(
      'operations' => array(
        array(
          'islandora_basic_collection_delete_all_children_batch_operation',
          array($collection),
        ),
      ),
      'title' => t('Deleting all children of @collection.', $message_parameters),
      'init_message' => t('Preparing to delete all children of @collection.', $message_parameters),
    );
  }

  return $batch;
}

/**
 * Deletes the given children from the given collection.
 *
 * Children are only deleted if the given collection is there only parent,
 * otherwise they are removed from the given collection.
 *
 * @param AbstractObject $collection
 *   The collection object to purge the children from.
 * @param array $children
 *   The PID's of the children to delete.
 * @param array $context
 *   The batch context.
 */
function islandora_basic_collection_delete_children_batch_operation(AbstractObject $collection, array $children, array &$context) {
  $child = islandora_basic_collection_start_operation($children, $context);
  if ($child && $child = islandora_object_load($child)) {
    $other_parents = islandora_basic_collection_get_other_parents($child, $collection);
    // If only one parent, delete the object.
    if (empty($other_parents)) {
      islandora_delete_object($child);
    }
    else {
      // If more than one parent, only remove parent relationship.
      islandora_basic_collection_remove_from_collection($child, $collection);
    }
  }
  if (islandora_basic_collection_end_operation($context)) {
    drupal_set_message(format_plural(count($children),
      'Deleted 1 child from <a href="@collection-url">@collection</a>.',
      'Deleted @count children from <a href="@collection-url">@collection</a>.',
      array(
        '@collection' => $collection->label,
        '@collection-url' => url("islandora/object/{$collection->id}"),
      )
    ));
  }
}

/**
 * Batch operation to delete ALL children from the given collection.
 *
 * @param AbstractObject $collection
 *   The collection object to purge the children from.
 * @param array $context
 *   The batch context.
 */
function islandora_basic_collection_delete_all_children_batch_operation(AbstractObject $collection, &$context) {
  $query_info = islandora_basic_collection_get_query_info(array(
    'object' => $collection,
  ), 'manage');

  if (!isset($context['sandbox']['total_count'])) {
    $context['sandbox']['total_count'] = $collection->repository->ri->countQuery($query_info['query'], $query_info['type']);
    if ($context['sandbox']['total_count'] === 0) {
      drupal_set_message(t('No objects to delete in @collection.', array(
        '@collection' => $collection->label,
      )));
      $context['finished'] = 1;
      return;
    }
    $context['sandbox']['current'] = 0.0;
  }

  $results = islandora_basic_collection_get_member_objects($collection, 0, 10, 'manage');
  if ($results) {
    list($count, $members) = $results;
    foreach ($members as $result) {
      $child = $result['object']['value'];
      if ($child && $child = islandora_object_load($child)) {
        $other_parents = islandora_basic_collection_get_other_parents($child, $collection);
        // If only one parent, delete the object.
        if (empty($other_parents)) {
          islandora_delete_object($child);
        }
        else {
          // If more than one parent, only remove parent relationship.
          islandora_basic_collection_remove_from_collection($child, $collection);
        }
      }
    }

    $member_count = count($members);
    $context['sandbox']['current'] += $member_count;

    if ($member_count === 0) {
      // We did not find any objects this pass... We are done.
      $context['finished'] = 1;
    }
    else {
      // Update our progress.
      $context['finished'] = $context['sandbox']['current'] / $context['sandbox']['total_count'];
    }
  }

  if (!$results || $context['finished'] == 1) {
    drupal_set_message(format_plural($context['sandbox']['current'],
      'Deleted 1 child from <a href="@collection-url">@collection</a>.',
      'Deleted @count children from <a href="@collection-url">@collection</a>.',
      array(
        '@collection' => $collection->label,
        '@collection-url' => url("islandora/object/{$collection->id}"),
      )
    ));
  }
}

/**
 * Migrate the given child objects from the source to destination collection.
 *
 * @param AbstractObject $source
 *   The source collection object.
 * @param AbstractObject $destination
 *   The destination collection object.
 * @param array $children
 *   The PID's of the children to migrate.
 *
 * @return array
 *   The batch definition.
 */
function islandora_basic_collection_migrate_children_batch(AbstractObject $source, AbstractObject $destination, $children = NULL) {
  $batch = array(
    'finished' => 'islandora_basic_collection_batch_finished',
    'progress_message' => t('Time elapsed: @elapsed <br/>Estimated time remaining @estimate.'),
    'error_message' => t('An error has occurred.'),
    'file' => drupal_get_path('module', 'islandora_basic_collection') . '/includes/batch.inc',
  );

  $message_parameters = array(
    '@source' => $source->label,
    '@destination' => $destination->label,
  );
  if (is_array($children)) {
    $child_count = count($children);
    $batch += array(
      'operations' => array(
        array(
          'islandora_basic_collection_migrate_children_batch_operation',
          array($source, $destination, $children),
        ),
      ),
      'title' => format_plural($child_count,
        'Migrating 1 child from @source to @destination ...',
        'Migrating @count children from @source to @destination...', $message_parameters),
      'init_message' => format_plural($child_count,
        'Preparing to migrate 1 child from @source to @destination ...',
        'Preparing to migrate @count children from @source to @destination ...', $message_parameters),
    );
  }
  else {
    $batch += array(
      'operations' => array(
        array(
          'islandora_basic_collection_migrate_all_children_batch_operation',
          array($source, $destination),
        ),
      ),
      'title' => t('Migrating all children of @source to @destination.', $message_parameters),
      'init_message' => t('Preparing to migrate all children of @source to @destination.', $message_parameters),
    );
  }

  return $batch;
}

/**
 * Moves the given children from the current collection to the new collection.
 *
 * @param AbstractObject $source
 *   The collection object the children are from.
 * @param AbstractObject $destination
 *   The destination collection.
 * @param array $children
 *   The PID's of the children to migrate.
 * @param array $context
 *   The batch context.
 */
function islandora_basic_collection_migrate_children_batch_operation(AbstractObject $source, AbstractObject $destination, array $children, array &$context) {
  $child = islandora_basic_collection_start_operation($children, $context);
  if ($child && $child = islandora_object_load($child)) {
    islandora_basic_collection_add_to_collection($child, $destination);
    islandora_basic_collection_remove_from_collection($child, $source);
  }
  if (islandora_basic_collection_end_operation($context)) {
    drupal_set_message(format_plural(count($children),
      'Migrated 1 child from <a href="@source-url">@source</a> to <a href="@destination-url">@destination</a>.',
      'Migrated @count children from <a href="@source-url">@source</a> to <a href="@destination-url">@destination</a>.',
      array(
        '@source' => $source->label,
        '@source-url' => url("islandora/object/{$source->id}"),
        '@destination' => $destination->label,
        '@destination-url' => url("islandora/object/{$destination->id}"),
      )
    ));
  }
}

/**
 * Share the given child objects from the source with destination collection.
 *
 * @param AbstractObject $source
 *   The source collection object.
 * @param AbstractObject $destination
 *   The destination collection object.
 * @param array $children
 *   The PID's of the children to share.
 *
 * @return array
 *   The batch definition.
 */
function islandora_basic_collection_share_children_batch(AbstractObject $source, AbstractObject $destination, $children = NULL) {
  $batch = array(
    'finished' => 'islandora_basic_collection_batch_finished',
    'progress_message' => t('Time elapsed: @elapsed <br/>Estimated time remaining @estimate.'),
    'error_message' => t('An error has occurred.'),
    'file' => drupal_get_path('module', 'islandora_basic_collection') . '/includes/batch.inc',
  );

  $message_parameters = array(
    '@source' => $source->label,
    '@destination' => $destination->label,
  );
  if (is_array($children)) {
    $child_count = count($children);
    $batch += array(
      'operations' => array(
        array(
          'islandora_basic_collection_share_children_batch_operation',
          array($source, $destination, $children),
        ),
      ),
      'title' => format_plural($child_count,
        'Sharing 1 child from @source to @destination...',
        'Sharing @count children from @source to @destination...', $message_parameters),
      'init_message' => format_plural($child_count,
        'Preparing to share 1 child from @source to @destination ...',
        'Preparing to share @count children from @source to @destination ...', $message_parameters),
    );
  }
  else {
    $batch += array(
      'operations' => array(
        array(
          'islandora_basic_collection_share_all_children_batch_operation',
          array($source, $destination,
          ),
        ),
      ),
      'title' => t('Sharing all children of @source with @destination.', $message_parameters),
      'init_message' => t('Preparing to share all children of @source with @destination.', $message_parameters),
    );
  }

  return $batch;
}
/**
 * Shares given children from the current collection to the new collection.
 *
 * @param AbstractObject $source
 *   The collection object the children are from.
 * @param AbstractObject $destination
 *   The destination collection.
 * @param array $children
 *   The PID's of the children to share.
 * @param array $context
 *   The batch context.
 */
function islandora_basic_collection_share_children_batch_operation(AbstractObject $source, AbstractObject $destination, array $children, &$context) {
  $child = islandora_basic_collection_start_operation($children, $context);
  if ($child && $child = islandora_object_load($child)) {
    islandora_basic_collection_add_to_collection($child, $destination);
  }
  if (islandora_basic_collection_end_operation($context)) {
    drupal_set_message(format_plural(count($children),
      'Shared 1 child from <a href="@source-url">@source</a> to <a href="@destination-url">@destination</a>.',
      'Shared @count children from <a href="@source-url">@source</a> to <a href="@destination-url">@destination</a>.',
      array(
        '@source' => $source->label,
        '@source-url' => url("islandora/object/{$source->id}"),
        '@destination' => $destination->label,
        '@destination-url' => url("islandora/object/{$destination->id}"),
      )
    ));
  }
}

/**
 * Migrate all children from the current collection to the new collection.
 *
 * @param AbstractObject $source
 *   The collection object the children are from.
 * @param AbstractObject $destination
 *   The destination collection.
 * @param array $context
 *   The batch context.
 */
function islandora_basic_collection_migrate_all_children_batch_operation(AbstractObject $source, AbstractObject $destination, &$context) {
  $results = islandora_basic_collection_get_member_objects($source, 0, 10, 'manage');
  if ($results) {
    list($count, $members) = $results;
    if (!isset($context['sandbox']['total_count'])) {
      $context['sandbox']['total_count'] = $count;
      $context['sandbox']['current'] = 0;
    }
    foreach ($members as $result) {
      $child = $result['object']['value'];
      if ($child && $child = islandora_object_load($child)) {
        islandora_basic_collection_add_to_collection($child, $destination);
        islandora_basic_collection_remove_from_collection($child, $source);
        $context['sandbox']['current']++;
      }
    }
  }
  $context['finished'] = $context['sandbox']['current'] / $context['sandbox']['total_count'];
  $context['message'] = 'Migrated ' . $context['sandbox']['current'] . ' children.';
}
/**
 * Shares all children from the current collection to the new collection.
 *
 * @param AbstractObject $source
 *   The collection object the children are from.
 * @param AbstractObject $destination
 *   The destination collection.
 * @param array $context
 *   The batch context.
 */
function islandora_basic_collection_share_all_children_batch_operation(AbstractObject $source, AbstractObject $destination, &$context) {
  $results = islandora_basic_collection_get_member_objects($source, $context['sandbox']['page'], 10, 'manage');
  $context['sandbox']['page']++;
  if ($results) {
    list ($count, $members) = $results;
    if (!isset($context['sandbox']['current'])) {
      $context['sandbox']['total_count'] = $count;
      $context['sandbox']['current'] = 0;
    }
    foreach ($members as $result) {
      $child = $result['object']['value'];
      if ($child && $child = islandora_object_load($child)) {
        islandora_basic_collection_add_to_collection($child, $destination);
        $context['sandbox']['current']++;
      }
    }
  }

  $context['finished'] = $context['sandbox']['current'] / $context['sandbox']['total_count'];
  $context['message'] = t('Shared @current_num children from @source to @destination', array(
                        '@current_num' => $context['sandbox']['current'],
                        '@source' => $source->label,
                        '@destination' => $destination->label,
                        ));
}
