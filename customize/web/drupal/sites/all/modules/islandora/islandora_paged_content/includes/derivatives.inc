<?php

/**
 * @file
 * Callbacks used in derivatives hook.
 */

/**
 * Generate a PDF derivative.
 */
function islandora_paged_content_derivatives_pdf(AbstractObject $object, $force = TRUE) {
  if ($force || !isset($object['PDF'])) {
    module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
    $pdf_file = islandora_paged_content_page_derive_pdf($object);
    if (!$pdf_file) {
      return FALSE;
    }
    $ret = islandora_paged_content_update_datastream($object, $pdf_file, 'PDF');
    file_unmanaged_delete($pdf_file);

    if ($ret) {
      return array(
        'success' => TRUE,
        'messages' => array(
          array(
            'message' => t('Generated %dsid on %pid.'),
            'message_sub' => array(
              '%dsid' => 'PDF',
              '%pid' => $object->id,
            ),
            'type' => 'dsm',
            'severity' => 'status',
          ),
        ),
      );
    }
    else {
      return array(
        'success' => FALSE,
        'messages' => array(
          array(
            'message' => t('Failed to generated %dsid on %pid.'),
            'message_sub' => array(
              '%dsid' => 'PDF',
              '%pid' => $object->id,
            ),
            'type' => 'watchdog',
            'severity' => WATCHDOG_ERROR,
          ),
        ),
      );
    }
  }
}

/**
 * Add thumbnail to parent, based on first member.
 */
function islandora_paged_content_derive_thumbnail_on_parent(AbstractObject $object, $force = TRUE) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $book_pid = islandora_paged_content_get_relationship($object->relationships, FEDORA_RELS_EXT_URI, 'isMemberOf', NULL);
  $book_object = islandora_object_load($book_pid);
  if (is_object($book_object)) {
    islandora_paged_content_derive_thumbnail_of_parent($book_object, $force);
  }
}

/**
 * Add thumbnail, based on first member.
 */
function islandora_paged_content_derive_thumbnail_of_parent(AbstractObject $book_object, $force = TRUE) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  if ($force || !isset($book_object['TN'])) {
    if (islandora_paged_content_update_paged_content_thumbnail($book_object)) {
      return array(
        'success' => TRUE,
        'messages' => array(
          array(
            'message' => t('Updated @dsid on @parent.'),
            'message_sub' => array(
              '@dsid' => 'TN',
              '@parent' => $book_object->id,
            ),
            'type' => 'dsm',
            'severity' => 'status',
          ),
        ),
      );
    }
    else {
      return array(
        'success' => FALSE,
        'messages' => array(
          array(
            'message' => t('Failed to update @dsid on @parent.'),
            'message_sub' => array(
              '@dsid' => 'TN',
              '@parent' => $book_object->id,
            ),
            'type' => 'watchdog',
            'severity' => WATCHDOG_ERROR,
          ),
        ),
      );
    }
  }
}

/**
 * Creates a PDF of all the pages on a paged content object.
 */
function islandora_paged_content_aggregate_pdf_derivative(AbstractObject $object, $force = TRUE) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  module_load_include('inc', 'islandora_paged_content', 'includes/batch');
  if ($object->relationships->get(ISLANDORA_RELS_EXT_URI, 'create_pdf', 'true', TRUE) || $force) {
    $batch = islandora_paged_content_create_pdf_batch(
      $object,
      array_keys(islandora_paged_content_get_pages($object)),
      array(
        '-density' => '96',
        '-compress' => 'LZW',
      )
    );

    $batch['operations'][] = array('islandora_book_batch_remove_pdf_flag', array($object));
    batch_set($batch);
  }
}

/**
 * Creates an OCR file of all the pages on a paged content object.
 */
function islandora_paged_content_aggregate_ocr_derivative(AbstractObject $object, $force = TRUE) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  module_load_include('inc', 'islandora_paged_content', 'includes/batch');
  if ($object->relationships->get(ISLANDORA_RELS_EXT_URI, 'create_ocr', 'true', TRUE) || $force) {
    $batch = islandora_paged_content_create_ocr_batch(
      $object,
      array_keys(islandora_paged_content_get_pages($object)),
      array('aggregate_ocr' => TRUE)
    );
    batch_set($batch);
  }
}

/**
 * Removes the PDF derivative creation flag.
 *
 * @param AbstractObject $object
 *   The object to remove the relationship from.
 */
function islandora_paged_content_remove_pdf_flag(AbstractObject $object) {
  $object->relationships->remove(ISLANDORA_RELS_EXT_URI, 'create_pdf', 'true', RELS_TYPE_PLAIN_LITERAL);
}

/**
 * Sets the PDF derivative creation flag.
 *
 * @param AbstractObject $object
 *   The object to set the relationship on.
 */
function islandora_paged_content_set_pdf_flag(AbstractObject $object) {
  $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'create_pdf', 'true', RELS_TYPE_PLAIN_LITERAL);
}

/**
 * Removes the OCR derivative creation flag.
 *
 * @param AbstractObject $object
 *   The object to remove the relationship from.
 */
function islandora_paged_content_remove_ocr_flag(AbstractObject $object) {
  $object->relationships->remove(ISLANDORA_RELS_EXT_URI, 'create_ocr', 'true', RELS_TYPE_PLAIN_LITERAL);
}

/**
 * Sets the OCR derivative creation flag.
 *
 * @param AbstractObject $object
 *   The object to set the relationship on.
 */
function islandora_paged_content_set_ocr_flag(AbstractObject $object) {
  $object->relationships->add(ISLANDORA_RELS_EXT_URI, 'create_ocr', 'true', RELS_TYPE_PLAIN_LITERAL);
}
