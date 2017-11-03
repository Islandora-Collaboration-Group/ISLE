<?php

/**
 * @file
 * Batch for collection thumbnail generation based on content.
 */

/**
 * Construct a bath for pulling thumbnails from children to collections.
 */
function islandora_basic_collection_thumbs_batch($collection) {
  return array(
    'title' => t('Pull Thumbnails from Children'),
    'init_message' => t('Preparing to pull thumbnails from children.'),
    'progress_message' => t('Time elapsed: @elapsed <br/>Estimated time remaining @estimate.'),
    'error_message' => t('An error has occurred.'),
    'file' => drupal_get_path('module', 'islandora_basic_collection') . '/includes/thumbs.batch.inc',
    'operations' => array(
      array('islandora_basic_collection_thumbs_batch_operation', array($collection)),
    ),
  );
}

/**
 * Batch operation for pulling child thumbs.
 *
 * @param string $collection
 *   PID of the collection to start the recursal at.
 * @param array $context
 *   Context array for this batch. Used to tell batch when we're finished.
 */
function islandora_basic_collection_thumbs_batch_operation($collection, &$context) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  $tuque = islandora_get_tuque_connection();
  $sandbox = &$context['sandbox'];
  // Initial setup.
  if (!isset($sandbox['stack'])) {
    $sandbox['stack'] = array($collection);
    $sandbox['retry_stack'] = array($collection);
    $sandbox['completed'] = 0;
  }
  if ($sandbox['stack']) {
    $current_pid = array_pop($sandbox['stack']);
    $retry = FALSE;
  }
  elseif ($sandbox['retry_stack']) {
    $current_pid = array_pop($sandbox['retry_stack']);
    $retry = TRUE;
  }
  $replaced = FALSE;

  // If not a retry check TN and push children collections to stack.
  if (!$retry) {
    $map_results = function($o) {
      return $o['child']['value'];
    };
    $child_collections_query = <<<EOQ
SELECT ?child
FROM <#ri>
WHERE {
  ?child <fedora-rels-ext:isMemberOfCollection> <info:fedora/$current_pid> .
  ?child <fedora-model:hasModel> <info:fedora/islandora:collectionCModel>
}
EOQ;
    $collections_results = $tuque->repository->ri->sparqlQuery($child_collections_query);
    // Add children to stack.
    $sandbox['stack'] = array_merge($sandbox['stack'], array_map($map_results, $collections_results));
  }

  // Test first child for TN.
  $child_tn_query = <<<EOQ
SELECT ?child
FROM <#ri>
WHERE {
  ?child <fedora-rels-ext:isMemberOfCollection> <info:fedora/$current_pid> .
  ?child <fedora-view:disseminates> ?ds .
  ?ds <fedora-view:disseminationType> <info:fedora/*/TN>
}
EOQ;
  $tn_results = $tuque->repository->ri->sparqlQuery($child_tn_query);
  $replace = FALSE;
  if ($tn_results) {
    $tn_result = reset($tn_results);
    $first_child_pid = $tn_result['child']['value'];
    $first_child = islandora_object_load($first_child_pid);
    if (isset($first_child['TN'])) {
      $copied_file = file_create_filename('TN.png', file_default_scheme() . '://');
      $first_child['TN']->getContent($copied_file);
      $copied_file_object = islandora_temp_file_entry($copied_file);
      // TN size check to avoid checksum expense, not trusting Fedora's size
      // (observed off by one errors).
      if ($copied_file_object->filesize != 5137) {
        $replace = TRUE;
      }
      // If size the same do checksum, also don't trust Fedora, some
      // checksums are lies (only observed with Fedora re-formating XML).
      elseif (md5_file($copied_file_object->uri) == 'cb971815171163beb105f8074eae1bde') {
        $replace = TRUE;
      }
      file_delete($copied_file_object);
    }
  }
  if ($replace) {
    $current_object = islandora_object_load($current_pid);
    if (!isset($current_object['TN'])) {
      $current_object->ingestDatastream($first_child['TN']);
    }
    else {
      $current_object['TN']->mimetype = $first_child['TN']->mimetype;
      $current_object['TN']->label = $first_child['TN']->label;
      $current_object['TN']->content = $first_child['TN']->content;
    }
    $replaced = TRUE;
  }

  // Throw warning to watchdog if final try had no TN.
  if ($retry && !$replaced) {
    watchdog(
      'warning',
      "Couldn't replace TN for !current_pid, this can still be done manually.",
      array('!current_pid', $current_pid)
    );
  }
  // Abort and return to stack if not retry and child has default TN.
  elseif (!$retry && !$replaced) {
    array_push($sandbox['retry_stack'], $current_pid);
  }

  $sandbox['completed'] += 1;
  $context['finished'] = $sandbox['completed'] / (count($sandbox['retry_stack']) + count($sandbox['stack']) + $sandbox['completed']);
}
