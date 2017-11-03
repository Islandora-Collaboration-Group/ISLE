<?php

/**
 * @file
 * Implementation of Drush hooks.
 */

/**
 * Implements hook_drush_command().
 */
function islandora_book_batch_drush_command() {
  $items = array();

  $items['islandora_book_batch_preprocess'] = array(
    'aliases' => array('ibbp'),
    'description' => 'Preprocessed books into database entries.',
    'drupal dependencies' => array('islandora_batch',
      'islandora_book',
      'islandora_book_batch'),
    'options' => array(
      'type' => array(
        'description' => 'Either "directory" or "zip".',
        'required' => TRUE,
      ),
      'namespace' => array(
        'description' => 'The namespace for objects created by this command.  Defaults to namespce set in fedora config.',
        'required' => FALSE,
      ),
      'content_models' => array(
        'description' => 'A comma-separated list of content models to assign ' .
        'to the objects. Only applies to the "book" level object.',
        'value' => 'optional',
      ),
      'parent' => array(
        'description' => 'The collection to which the generated items should ' .
        'be added.  Only applies to the "book" level object. If ' .
        '"directory" and the directory containing the book description is ' .
        'a valid PID, it will be set as the parent. If this is specified ' .
        'and itself is a PID, all books will be related to the given PID.',
        'value' => 'optional',
      ),
      'parent_relationship_uri' => array(
        'description' => 'The namespace URI of the relationship to the parent.' .
        ' Defaults to "info:fedora/fedora-system:def/relations-external#".',
        'value' => 'optional',
      ),
      'parent_relationship_pred' => array(
        'description' => 'The predicate of the relationship to the parent. ' .
        'Defaults to "isMemberOfCollection".',
        'value' => 'optional',
      ),
      'create_pdfs' => array(
        'description' => 'A flag to cause PDFs to be created in books. Page PDF creation is dependant on the configuration within Drupal proper.',
        'value' => 'optional',
      ),
      'do_not_generate_ocr' => array(
        'description' => 'A flag to allow for conditional OCR generation.',
        'value' => 'optional',
      ),
      'aggregate_ocr' => array(
        'description' => 'A flag to cause OCR to be aggregated to books, if OCR is also being generated per-page.',
        'value' => 'optional',
      ),
      'email_admin' => array(
        'description' => 'A flag to notify the site admin when the book is ' .
        'fully ingested (depends on Rules being enabled).',
        'value' => 'optional',
      ),
      'wait_for_metadata' => array(
        'description' => 'A flag to indicate that we should hold off on ' .
        'trying to ingest books until we have metadata available for them' .
        'at the book level.',
        'value' => 'optional',
      ),
      'directory_dedup' => array(
        'description' => 'A flag to indicate that we should avoid ' .
        'repreprocessing books which are located in directories.',
      ),
      'page_progression' => array(
        'description' => 'A flag to indicate the page progression for the book. If not specified will default to LR.',
        'value' => 'optional',
      ),
      'output_set_id' => array(
        'description' => 'A flag to indicate whether to print the set ID of the preprocessed book.',
        'value' => 'optional',
      ),
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_LOGIN,
  );
  // XXX: The target parameter became reserved in Drush 7 and above, for
  // backwards compatibility both will be supported. Not using
  // strict-option-handling (http://www.drush.org/en/master/strict-options) as
  // it requires manual argument parsing.
  if (DRUSH_VERSION >= 7) {
    $items['islandora_book_batch_preprocess']['options']['scan_target'] = array(
      'description' => 'The target to directory or zip file to scan.',
      'required' => TRUE,
    );
  }
  else {
    $items['islandora_book_batch_preprocess']['options']['target'] = array(
      'description' => 'The target to directory or zip file to scan.',
      'required' => TRUE,
    );
  }
  return $items;
}

/**
 * Implements hook_drush_command().
 */
function drush_islandora_book_batch_preprocess() {
  // XXX: Due to how Drush bootstrapping works, the connection may be created
  // without credentials (when your site's front page is
  // 'islandora/object/some:object', for example). Resetting to ensure a new
  // connection gets created should fix it.
  drupal_static_reset('islandora_get_tuque_connection');
  $connection = islandora_get_tuque_connection();
  $parameters = array(
    'type' => drush_get_option('type'),
    'namespace' => drush_get_option('namespace'),
    'target' => DRUSH_VERSION >= 7 ? drush_get_option('scan_target') : drush_get_option('target'),
    'parent' => drush_get_option('parent', 'islandora:bookCollection'),
    'parent_relationship_uri' => drush_get_option('parent_relationship_uri', 'info:fedora/fedora-system:def/relations-external#'),
    'parent_relationship_pred' => drush_get_option('parent_relationship_pred', 'isMemberOfCollection'),
    'create_pdfs' => drush_get_option('create_pdfs', FALSE),
    'aggregate_ocr' => drush_get_option('aggregate_ocr', FALSE),
    'email_admin' => drush_get_option('email_admin', FALSE),
    'wait_for_metadata' => drush_get_option('wait_for_metadata', FALSE),
    'directory_dedup' => drush_get_option('directory_dedup', FALSE),
    'page_progression' => drush_get_option('page_progression', FALSE),
  );

  if ($content_models = drush_get_option('content_models', FALSE)) {
    $parameters['content_models'] = explode(',', $content_models);
  }
  else {
    $parameters['content_models'] = array('islandora:bookCModel');
  }

  if ($do_not_generate = drush_get_option('do_not_generate_ocr', FALSE)) {
    $parameters['generate_ocr'] = FALSE;
  }
  else {
    $parameters['generate_ocr'] = TRUE;
  }

  $preprocessor = new IslandoraBookBatch($connection, $parameters);

  // Pass the preprocessor off to run.
  $preprocessed = islandora_batch_handle_preprocessor($preprocessor);

  if (drush_get_option('output_set_id', FALSE)) {
    drush_print($preprocessor->getSetId());
  }
}

/**
 * Book batch preprocess validation.
 */
function drush_islandora_book_batch_preprocess_validate() {
  $page_progression = drush_get_option('page_progression', FALSE);
  if ($page_progression) {
    if ($page_progression != 'lr' && $page_progression != 'rl') {
      return drush_set_error('Invalid page progression parameter.', dt('The specified page progression of @progression is not valid. Only the values "lr" or "rl" can be used.', array('@progression' => $page_progression)));
    }
  }
}
