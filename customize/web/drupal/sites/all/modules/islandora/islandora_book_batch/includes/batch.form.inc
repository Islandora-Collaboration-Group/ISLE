<?php

/**
 * @file
 * Form to kickoff a book batch ingest from the front-end.
 */

/**
 * Form building function.
 */
function islandora_book_batch_form($form, &$form_state, $object) {
  form_load_include($form_state, 'inc', 'islandora_book_batch', 'includes/batch.form');
  module_load_include('inc', 'islandora', 'includes/utilities');
  $default_namespace = islandora_get_namespace($object->id);

  $form['target'] = array(
    '#type' => 'managed_file',
    '#title' => t('Zip file'),
    '#description' => t('A Zip file containing a number of books. Each book is structured as a directory, containing a number of directories representing pages. Basenames will be used as datastream identifiers except or "--METADATA--". "--METADATA--" can be either a .xml containing XML (MODS, DC or MARCXML), or a .mrc (Binary MARC), which will be transformed to produce both a MODS and DC stream.'),
    '#required' => TRUE,
    '#upload_location' => 'public://',
    '#upload_validators' => array(
      'file_validate_extensions' => array('zip'),
    ),
  );

  $form['parent'] = array(
    '#type' => 'value',
    '#value' => $object->id,
  );

  $form['page_progression'] = array(
    '#type' => 'radios',
    '#title' => t('Page Progression'),
    '#options' => array(
      'lr' => t('Left-to-Right'),
      'rl' => t('Right-to-Left'),
    ),
    '#default_value' => 'lr',
  );

  $form['create_pdfs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create PDFs?'),
    '#description' => t('Whether or not we should generate PDFs for the books after ingesting all of the pages.'),
  );
  $form['namespace'] = array(
    '#type' => 'textfield',
    '#title' => t('Namespace for created objects?'),
    '#description' => t('Book and page objects will be contructed within this namespace.'),
    "#default_value" => $default_namespace,
    '#size' => 15,
  );
  if (module_exists('islandora_ocr')) {
    module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
    $derive = islandora_paged_content_get_enabled_ingest_derivatives('islandora_book');
    if ($derive['ocr']) {
      $form['generate_ocr'] = array(
        '#type' => 'checkbox',
        '#title' => t('Generate OCR?'),
        '#description' => t('Whether or not we should generate OCR for the pages of the book.'),
        '#default_value' => TRUE,
      );
      $form['aggregate_ocr'] = array(
        '#type' => 'checkbox',
        '#title' => t('Aggregate OCR?'),
        '#description' => t('Whether or not we should generate OCR for the books after ingesting all of the pages.'),
        '#default_value' => FALSE,
        '#states' => array(
          'invisible' => array(
            ':input[name="generate_ocr"]' => array('checked' => FALSE),
          ),
        ),
      );
    }
  }
  $form['email_admin'] = array(
    '#type' => 'checkbox',
    '#title' => t('Notify admin after ingest?'),
    '#description' => t('Whether or not we should notify the site admin that a book has been ingested after the ingest of the book completes (requires relevant "Rules" rule).'),
  );

  $form['ingest_set_immediately'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ingest immediately?'),
    '#description' => t('If not selected, objects will just be preprocessed into the queue of objects to be ingested, to be fully-processed (and ingested) later--whenever an admin deems it an appropriate time.'),
    '#default_value' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Ingest'),
  );

  return $form;
}

/**
 * Form submit handler.
 *
 * May kick off a batch process.
 */
function islandora_book_batch_form_submit($form, $form_state) {
  $target = file_load($form_state['values']['target']);

  $form_state['values']['target'] = $target->uri;

  $connection = islandora_get_tuque_connection();
  $parameters = $form_state['values'] + array(
    'type' => 'zip',
    'parent_relationship_uri' => 'info:fedora/fedora-system:def/relations-external#',
    'parent_relationship_pred' => 'isMemberOfCollection',
    'wait_for_metadata' => FALSE,
    'directory_dedup' => FALSE,
    'content_models' => array('islandora:bookCModel'),
  );

  $preprocessor = new IslandoraBookBatch($connection, $parameters);
  // XXX: We currently do not have a place where we remove usages, as we never
  // really delete "sets" (or "items", for that matter)...  To come in the
  // management side of things, in INT-192?
  file_usage_add($target, 'islandora_batch', 'islandora_batch_set', $preprocessor->getSetId());

  if ($form_state['values']['ingest_set_immediately']) {
    islandora_batch_ingest(array(
      'preprocessor' => $preprocessor,
    ));
  }
  else {
    $preprocessed = islandora_batch_handle_preprocessor($preprocessor);
  }
}
