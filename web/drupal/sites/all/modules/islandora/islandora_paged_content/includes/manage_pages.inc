<?php

/**
 * @file
 * Defines forms for managing objects with child pages.
 *
 * Management Forms:
 *  Create PDF.
 *  Create OCR.
 *  Create Images.
 *  Update Thumbnail.
 *  Reorder Pages.
 *  Delete Pages.
 */

require_once dirname(__FILE__) . '/utilities.inc';
require_once dirname(__FILE__) . '/batch.inc';

/**
 * Displays all the book management forms in a set of vertical tabs.
 *
 * @param AbstractObject $object
 *   The book object to manage.
 *
 * @return array
 *   A renderable array containing all the management forms related to
 *   book objects.
 */
function islandora_paged_content_manage_pages_menu(AbstractObject $object) {
  module_load_include('inc', 'islandora', 'includes/breadcrumb');
  drupal_set_breadcrumb(islandora_get_breadcrumbs($object));
  drupal_set_title($object->label);
  $manage_tabs = array(
    'manage_pages' => array(
      '#type' => 'vertical_tabs',
      'pdf' => array(
        '#access' => islandora_object_access(ISLANDORA_ADD_DS, $object),
        '#title' => t('Create PDF'),
        '#type' => 'fieldset',
        'form' => drupal_get_form('islandora_paged_content_manage_pages_pdf_form', $object),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      ),
      'ocr' => array(
        '#access' => islandora_object_access(ISLANDORA_ADD_DS, $object),
        '#title' => t('Perform OCR'),
        '#type' => 'fieldset',
        'form' => drupal_get_form('islandora_paged_content_manage_pages_ocr_form', $object),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      ),
      'images' => array(
        '#access' => islandora_object_access(ISLANDORA_ADD_DS, $object),
        '#title' => t('Create Images'),
        '#type' => 'fieldset',
        'form_1' => drupal_get_form('islandora_paged_content_manage_pages_thumbnail_form', $object),
        'form_2' => drupal_get_form('islandora_paged_content_manage_pages_images_form', $object),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      ),
      'page_progression' => array(
        '#access' => islandora_datastream_access(ISLANDORA_METADATA_EDIT, $object['RELS-EXT']),
        '#title' => t('Page Progression'),
        '#type' => 'fieldset',
        'form' => drupal_get_form('islandora_paged_content_manage_page_progression_form', $object),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      ),
      'sequence' => array(
        '#access' => islandora_datastream_access(ISLANDORA_METADATA_EDIT, $object['RELS-EXT']),
        '#title' => t('Reorder Pages'),
        '#type' => 'fieldset',
        'form' => drupal_get_form('islandora_paged_content_manage_pages_sequences_form', $object),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      ),
      'delete' => array(
        '#access' => islandora_object_access(ISLANDORA_PURGE, $object),
        '#title' => t('Delete Pages'),
        '#type' => 'fieldset',
        'form' => drupal_get_form('islandora_paged_content_manage_pages_delete_form', $object),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      ),
    ),
  );
  $context = array('object' => $object);
  drupal_alter('islandora_paged_content_pages_management_tabs', $manage_tabs, $context);
  return $manage_tabs;
}

/**
 * Creates a PDF of all the child pages.
 *
 * Generates a PDF file for each page and then combines them into a single PDF.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 * @param AbstractObject $object
 *   The object to fetch the pages from.
 *
 * @return array
 *   The Drupal form.
 */
function islandora_paged_content_manage_pages_pdf_form(array $form, array &$form_state, AbstractObject $object) {
  $form_state['object'] = $object;
  $can_derive = islandora_paged_content_can_create_pdf() && islandora_paged_content_can_combine_pdf();
  return array(
    'description' => array(
      '#type' => 'item',
      '#description' => t('You must have both <b>ImageMagick</b> and <b>GhostScript</b> installed to create/combine PDF files.<br/> This will also update the PDF datastreams for each Page object.'),
    ),
    'dpi' => array(
      '#type' => 'select',
      '#title' => t('DPI - Dots Per Inch'),
      '#description' => t('Set the DPI for the generated PDF. The higher the resolution the longer the process will take.'),
      '#options' => drupal_map_assoc(array('72', '96', '300', '600')),
    ),
    'submit' => array(
      '#disabled' => !$can_derive,
      '#type' => 'submit',
      '#value' => t('Create PDF'),
    ),
  );
}

/**
 * Creates a PDF of the book and a PDF of every page.
 *
 * Triggers a batch to derive a PDF datastreams in each page object.then
 * combined them into a single PDF which is stored in the book object.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_paged_content_manage_pages_pdf_form_submit(array $form, array &$form_state) {
  $object = $form_state['object'];
  $pages = array_keys(islandora_paged_content_get_pages($object));
  $options = array(
    '-density' => $form_state['values']['dpi'],
    '-compress' => 'LZW',
  );
  $batch = islandora_paged_content_create_pdf_batch($object, $pages, $options);
  batch_set($batch);
}

/**
 * Derives the OCR datastreams in each child page.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 * @param AbstractObject $object
 *   The object to fetch the pages from.
 *
 * @return array
 *   The Drupal form.
 */
function islandora_paged_content_manage_pages_ocr_form(array $form, array &$form_state, AbstractObject $object) {
  $form_state['object'] = $object;
  $can_derive = FALSE;
  $languages = array();
  unset($languages['no_ocr']);
  if (module_exists('islandora_ocr')) {
    module_load_include('inc', 'islandora_ocr', 'includes/utilities');
    $can_derive = islandora_ocr_can_derive_ocr();
    $languages = islandora_ocr_get_enabled_tesseract_languages();
  }
  return array(
    'description' => array(
      '#type' => 'item',
      '#description' => t('You must have <b>Tesseract</b> installed to perform OCR.<br/> This will update the OCR and HOCR datastreams for each Page object.'),
    ),
    'language' => array(
      '#access' => $can_derive,
      '#title' => t('Language'),
      '#type' => 'select',
      '#description' => t('Please select the language the pages are written in.'),
      '#options' => $languages,
    ),
    'aggregate_ocr' => array(
      '#access' => $can_derive,
      '#type' => 'checkbox',
      '#title' => t('Aggregate OCR to the Parent'),
      '#description' => t('Check this to aggregate a consolidated OCR datastream generated from each page and append it to the current object.'),
      '#default_value' => FALSE,
    ),
    'submit' => array(
      '#disabled' => !$can_derive,
      '#type' => 'submit',
      '#value' => t('Create OCR'),
    ),
  );
}

/**
 * Triggers a batch to derive the OCR datastreams in each page object.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_paged_content_manage_pages_ocr_form_submit(array $form, array &$form_state) {
  $object = $form_state['object'];
  $pages = array_keys(islandora_paged_content_get_pages($object));
  $options = array(
    'language' => $form_state['values']['language'],
    'preprocess' => FALSE,
    'aggregate_ocr' => $form_state['values']['aggregate_ocr'],
  );
  $batch = islandora_paged_content_create_ocr_batch($object, $pages, $options);
  batch_set($batch);
}

/**
 * Updates the objects thumbnail from the first child page.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 * @param AbstractObject $object
 *   The object to update.
 *
 * @return array
 *   The Drupal form.
 */
function islandora_paged_content_manage_pages_thumbnail_form(array $form, array &$form_state, AbstractObject $object) {
  $form_state['object'] = $object;
  return array(
    'description' => array(
      '#type' => 'item',
      '#description' => t("Update the thumbnail image. The book must have pages, and the first page must have a TN datastream."),
    ),
    'submit' => array(
      '#disabled' => !islandora_paged_content_can_update_paged_content_thumbnail($object),
      '#type' => 'submit',
      '#value' => t('Update Thumbnail'),
    ),
  );
}

/**
 * The submit handler for the update thumbnail form.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_paged_content_manage_pages_thumbnail_form_submit(array $form, array &$form_state) {
  if (islandora_paged_content_update_paged_content_thumbnail($form_state['object'])) {
    drupal_set_message(t('Thumbnail successfully updated.'), 'status');
  }
  else {
    drupal_set_message(t('Failed to update thumbnail'), 'error');
  }
}

/**
 * Derives the image datastreams for each page object.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 * @param AbstractObject $object
 *   The object to fetch the pages from.
 *
 * @return array
 *   The Drupal form.
 */
function islandora_paged_content_manage_pages_images_form(array $form, array &$form_state, AbstractObject $object) {
  $form_state['object'] = $object;
  $can_derive = islandora_paged_content_can_create_images();
  return array(
    'description' => array(
      '#type' => 'item',
      '#description' => t('You must have the <b>Large Image Solution Pack</b> installed to create image derivatives.<br/> This will update the TN, JPG and JP2 datastreams for each page object.'),
    ),
    'submit' => array(
      '#disabled' => !$can_derive,
      '#type' => 'submit',
      '#value' => t('Create Images'),
    ),
  );
}

/**
 * Triggers a batch to derive image datastreams in each page object.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_paged_content_manage_pages_images_form_submit(array $form, array &$form_state) {
  $object = $form_state['object'];
  $pages = array_keys(islandora_paged_content_get_pages($object));
  $batch = islandora_paged_content_create_images_batch($object, $pages);
  batch_set($batch);
}

/**
 * Form for reordering the child pages.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 * @param AbstractObject $object
 *   The book object to fetch the pages from.
 *
 * @return array
 *   The Drupal form.
 */
function islandora_paged_content_manage_pages_sequences_form(array $form, array &$form_state, AbstractObject $object) {
  $form_state['pid'] = $object->id;
  // Cache the generated rows, as this build function gets called for every
  // ajax callback.
  if (!isset($form_state['rows'])) {
    $pages = islandora_paged_content_get_pages($object);
    $rows = array();
    foreach ($pages as $page) {
      $id = $page['pid'];
      $url = "islandora/object/{$id}";
      $rows[$id] = array(
        'tn' => array(
          '#markup' => theme(
              'image', array(
                'path' => url("$url/datastream/TN/view"),
                'attributes' => array(
                  'width' => '64px',
                  'height' => '64px',
                )))),
        'label' => array(
          '#markup' => l($page['label'], $url, array(
            'html' => TRUE,
            'attributes' => array('title' => $page['label'])))),
      );
    }
    $form_state['rows'] = $rows;
  }
  return array(
    'table' => array(
      '#type' => 'swaptable',
      '#header' => array(
        t('Thumbnail'),
        t('Label'),
      ),
      '#rows' => $form_state['rows'],
    ),
    'quick_reorder' => array(
      '#type' => 'fieldset',
      '#title' => t('Quick Reorder'),
      'pages_to_move' => array(
        '#type' => 'textfield',
        '#title' => t('Move Pages:'),
        '#description' => t("Must follow the form '1, 2-4,5'."),
      ),
      'pages_insert_point' => array(
        '#type' => 'textfield',
        '#title' => t('Insert After:'),
      ),
      'submit' => array(
        '#validate' => array('islandora_paged_content_quick_reorder_validate'),
        '#type' => 'submit',
        '#submit' => array('islandora_paged_content_quick_reorder_submit'),
        '#value' => t('Go'),
      ),
    ),
    'actions' => array(
      '#type' => 'actions',
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Save Changes'),
      ),
    ),
  );
}

/**
 * Validates quick_reorder fieldset.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_paged_content_quick_reorder_validate($form, &$form_state) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');

  $pages = islandora_paged_content_get_pages(islandora_object_load($form_state['pid']));
  $first_page = array_shift($pages);
  $last_page = array_pop($pages);
  $page_numbers = range($first_page['page'], $last_page['page']);

  // Insert point validation.
  if (!preg_match('/^[0-9\s]/', $form_state['values']['pages_insert_point']) ||
      !in_array($form_state['values']['pages_insert_point'], $page_numbers)) {
    form_set_error('pages_insert_point', t('Insert point must be a valid page number.'));
  }

  // Pages to move validation.
  if (!preg_match('/^[0-9\s,\-]/', $form_state['values']['pages_to_move'])) {
    form_set_error(
        'pages_to_move', t("Pages to move must follow the form '1, 2-4,5'.")
    );
  }
  $pages_to_move_list = islandora_paged_content_parse_pages_to_move($form_state['values']['pages_to_move']);
  if ($pages_to_move_list != array_intersect($pages_to_move_list, $page_numbers)) {
    form_set_error('pages_to_move', t('All pages must be in range.'));
  }
}

/**
 * Validates quick_reorder fieldset.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_paged_content_quick_reorder_submit($form, &$form_state) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $page_numbers_to_move = islandora_paged_content_parse_pages_to_move($form_state['values']['pages_to_move']);

  // Build page array without changed pids.
  $book = islandora_object_load($form_state['pid']);
  $raw_pages = islandora_paged_content_get_pages($book);
  $pages = array();
  $page_pids_to_move = array();
  foreach ($raw_pages as $page_pid => $page_info) {
    if (in_array($page_info['page'], $page_numbers_to_move)) {
      $page_pids_to_move[] = $page_pid;
    }
    else {
      $pages[$page_info['page']] = $page_pid;
    }
  }

  // Add in changed PIDs. Fixes sequence numbers.
  array_splice(
      $pages, array_search(
          $form_state['values']['pages_insert_point'], array_keys($pages)
      ) + 1, 0, $page_pids_to_move
  );

  $batch = islandora_paged_content_sequence_pages_batch($book, $pages);
  batch_set($batch);
}

/**
 * Submit handler for the sequence form.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_paged_content_manage_pages_sequences_form_submit(array $form, array &$form_state) {
  $object = islandora_object_load($form_state['pid']);
  $pages = $form_state['values']['table']['order'];
  if ($pages) {
    $batch = islandora_paged_content_sequence_pages_batch($object, $pages);
    batch_set($batch);
  }
}

/**
 * Gets the delete pages form.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 * @param AbstractObject $object
 *   The object to fetch the pages from.
 *
 * @return array
 *   The Drupal form.
 */
function islandora_paged_content_manage_pages_delete_form(array $form, array &$form_state, AbstractObject $object) {
  $form_state['object'] = $object;
  $pages = islandora_paged_content_get_pages($object);
  return array(
    'table' => array(
      '#type' => 'tableselect',
      '#header' => array(
        'pid' => t('PID'),
        'page' => t('Sequence Number'),
        'label' => t('Label'),
      ),
      '#options' => $pages,
      '#multiple' => TRUE,
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Delete Selected Pages'),
    ),
    'item' => array(
      '#type' => 'item',
      '#markup' => t('Note: Make sure to re-order pages after deleting pages to keep the numerical index in sequence for search term highlighting.'),
    ),
  );
}

/**
 * Submit handler for the delete pages form.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_paged_content_manage_pages_delete_form_submit(array $form, array &$form_state) {
  $object = $form_state['object'];
  $pages = array_values(array_filter($form_state['values']['table']));
  $batch = islandora_paged_content_delete_pages_batch($object, $pages);
  batch_set($batch);
}

/**
 * Local menu action to present a zipped file ingest form.
 *
 * @param AbstractObject $object
 *   The object to ingest into.
 * @param array $allowed_extensions
 *   The array of allowed extensions.
 * @param string $model
 *   The content model of the page object
 * @param array $derivatives
 *   The array of derivatives.
 *
 * @return string
 *   The HTML representation of the ingest page form.
 */
function islandora_paged_content_ingest_zipped_pages(AbstractObject $object, $allowed_extensions, $model, $derivatives) {
  module_load_include('inc', 'islandora', 'includes/breadcrumb');
  module_load_include('inc', 'islandora', 'includes/utilities');
  drupal_set_breadcrumb(islandora_get_breadcrumbs($object));
  return drupal_get_form('islandora_paged_content_zipped_upload_form', $object->id, $allowed_extensions, $model, $derivatives);
}

/**
 * Defines the zipped page form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 * @param string $pid
 *   PID of object into which pages are being ingested.
 * @param array $allowed_extensions
 *   The array of allowed extensions.
 * @param string $model
 *   The content model of the page object
 * @param array $derivatives
 *   The array of derivatives.
 *
 * @return array
 *   Drupal form definition.
 */
function islandora_paged_content_zipped_upload_form(array $form, array &$form_state, $pid, $allowed_extensions, $model, $derivatives) {
  module_load_include('inc', 'islandora_ocr', 'includes/utilities');
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $object = islandora_object_load($pid);
  $current_pages = islandora_paged_content_get_pages($object);
  $last_page_number = count($current_pages);
  $languages = module_exists('islandora_ocr') ? islandora_ocr_get_enabled_tesseract_languages() : array();
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  $extensions = array('zip');
  $form = array();
  $do_ocr = in_array('ocr', $derivatives) && module_exists('islandora_ocr');
  $message = t("This sequence currently has @count pages. Additional pages will be appended to the end of the sequence by default. !break", array("@count" => $last_page_number, '!break' => '<br />'));
  $message .= t("Choose a number lower than @count to insert page(s) at a specific location in the sequence.", array("@count" => $last_page_number, '!break' => '<br />'));

  $form['language'] = array(
    '#access' => module_exists('islandora_ocr') && $do_ocr,
    '#title' => t('Language'),
    '#type' => 'select',
    '#description' => t('Please select the language the page is written in.'),
    '#options' => $languages,
    '#default_value' => 'English',
  );

  if ($current_pages) {
    $form['insertion_point'] = array(
      '#type' => 'textfield',
      '#title' => t('Last sequence number'),
      '#default_value' => $last_page_number,
      '#description' => filter_xss_admin($message),
      '#size' => 5,
    );
  }

  // Value behaves more consistently when passed as a string.
  $form['current_pages'] = array(
    '#type' => 'hidden',
    '#value' => serialize($current_pages),
  );

  $form['file'] = array(
    '#title' => t('Compressed images file.'),
    '#type' => 'managed_file',
    '#required' => TRUE,
    '#description' => t('Select file to upload.<br/>Files must be less than <b>@size MB.</b><br/>Allowed file types: <b>@ext.</b>', array('@size' => $upload_size, '@ext' => $extensions[0])),
    '#default_value' => isset($form_state['values']['files']) ? $form_state['values']['files'] : NULL,
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => $extensions,
      'file_validate_size' => array($upload_size * 1024 * 1024),
    ),
  );

  $form['pid'] = array(
    '#type' => 'hidden',
    '#value' => $pid,
  );
  $form['allowed_extensions'] = array(
    '#type' => 'hidden',
    '#value' => $allowed_extensions,
  );
  $form['model'] = array(
    '#type' => 'hidden',
    '#value' => $model,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add files'),
  );
  return $form;
}

/**
 * Submit handler for uploaded zip files.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_paged_content_zipped_upload_form_submit(array $form, array &$form_state) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $tuque = new IslandoraTuque();
  $repository = $tuque->repository;
  $pid = $form_state['values']['pid'];
  $form_state['redirect'] = "islandora/object/$pid";
  $namespace = substr($pid, 0, strpos($pid, ":"));
  $tmp_dir = uniqid();
  $object = islandora_object_load($pid);
  $book_label = $object->label;
  if (!$object) {
    drupal_set_message(t("This object does not exist in this repository"), 'warning');
    return;
  }
  $language = $form_state['values']['language'];
  $current_pages = unserialize($form_state['values']['current_pages']);

  $insertion_point = isset($form_state['values']['insertion_point']) ? (int) $form_state['values']['insertion_point'] : 0;
  $pages_to_renumber = array();
  foreach ($current_pages as $current_page) {
    if ((int) $current_page['page'] > $insertion_point) {
      $pages_to_renumber[] = $current_page;
    }
  }
  // Extract file.
  $zip_file = file_load($form_state['values']['file']);
  $zip_uri = drupal_realpath($zip_file->uri);
  $zip = new ZipArchive();
  $zip->open($zip_uri);
  $destination_dir = drupal_realpath("temporary://$tmp_dir");

  // Extract zipped file to named directory.
  if (!$zip->extractTo($destination_dir)) {
    drupal_set_message(t('Ingest failed.'), 'warning');
    return;
  }
  $zip->close();
  file_delete($zip_file);

  $allowed_extensions = $form_state['values']['allowed_extensions'];
  $callback = function ($element) use ($allowed_extensions) {
    $ext = pathinfo($element, PATHINFO_EXTENSION);
    $ext = drupal_strtolower($ext);

    // An allowed extension and does /not/ contain __MACOSX.
    return in_array($ext, $allowed_extensions) && preg_match('/__MACOSX/', $element) === 0;
  };

  $objects = new RecursiveIteratorIterator(new RecursiveDirectoryIterator($destination_dir), RecursiveIteratorIterator::SELF_FIRST);
  foreach ($objects as $file => $object) {
    $unfiltered[] = $file;
  }
  $files_to_add = array_values(array_filter($unfiltered, $callback));
  // Sort files based on name.
  $comparator = function ($a, $b) {
    $file_a = pathinfo($a, PATHINFO_FILENAME);
    $file_b = pathinfo($b, PATHINFO_FILENAME);
    return ($file_a < $file_b) ? -1 : 1;
  };
  usort($files_to_add, $comparator);

  $renumber_count = count($pages_to_renumber);
  $add_count = count($files_to_add);
  $status_message = format_plural(
      $add_count, 'adding 1 page', 'adding @count pages'
  );

  if ($renumber_count) {
    $status_message = format_plural(
        $renumber_count, 'Renumbering 1 page and @adding_message.', 'Renumbering @count pages and @adding_message.', array(
          '@adding_message' => $status_message,
        )
    );
  }

  if ($add_count > 0) {
    $batch = array(
      'title' => ucfirst($status_message),
      'progress_message' => t('Completed @current operations out of @total.'),
      'operations' => array(),
      'file' => drupal_get_path('module', 'islandora_paged_content') . '/includes/manage_pages.inc',
      'finished' => 'islandora_paged_content_zipped_upload_ingest_finished',
    );
    $file_count = count($files_to_add);
    $config = array(
      'language' => $language,
      'pid' => $pid,
      'destination_dir' => $destination_dir,
      'namespace' => $namespace,
      'language' => $form_state['values']['language'],
      'model' => $form_state['values']['model'],
      'file_count' => $file_count,
      'book_label' => $book_label,
    );
    if (isset($pages_to_renumber[0])) {
      foreach ($pages_to_renumber as $page) {
        $batch['operations'][] = array(
          'islandora_paged_content_content_insert_sequence_gap',
          array($page, $file_count),
        );
      }
    }
    foreach ($files_to_add as $image) {
      $config['page_number'] = ++$insertion_point;
      $config['image'] = $image;
      $batch['operations'][] = array(
        'islandora_paged_content_add_pages',
        array($repository, $config, $destination_dir),
      );
    }

    batch_set($batch);
  }
  else {
    drupal_set_message(t('No files were found to add in the provided ZIP file.'), 'warning');
  }
}

/**
 * Creates page objects and associates them with object.
 *
 * @param Repository $repository
 *   Active repository object to build NewFedoraObject
 * @param array $config
 *   Associative array of required values.
 * @param string $pages_directory
 *   This is a hack.  The finished callback needs this value so it can delete
 *   the temporary directory used to store  pages.
 */
function islandora_paged_content_add_pages($repository, $config, $pages_directory, &$context) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $context['results']['pages_directory'] = $pages_directory;
  $context['results']['file_count'] = $config['file_count'];
  $context['results']['book_label'] = $config['book_label'];

  // Create object.
  $object = $repository->constructObject($config['namespace']);
  $label = pathinfo($config['image'], PATHINFO_FILENAME);
  if (variable_get('islandora_paged_content_page_label', FALSE)) {
    $label = $config['page_number'];
  }
  $object->label = $label;
  $ds_label = pathinfo($config['image'], PATHINFO_BASENAME);
  islandora_paged_content_update_datastream($object, $config['image'], 'OBJ', $ds_label, NULL, 'M', FALSE);
  $rels_ext = $object->relationships;
  $parent = $config['pid'];
  $object->relationships->add(FEDORA_MODEL_URI, 'hasModel', $config['model']);

  islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'hasLanguage', $config['language'], TRUE);
  islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isPageOf', $parent);
  islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isSequenceNumber', (string) $config['page_number'], TRUE);
  islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isPageNumber', (string) $config['page_number'], TRUE);
  islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isSection', '1', TRUE);
  islandora_paged_content_set_relationship($rels_ext, FEDORA_RELS_EXT_URI, 'isMemberOf', $parent);

  if (module_exists('islandora_xacml_editor')) {
    // XACML inheritance.
    // TODO: Implement in an object altered hook? May need a better way to
    // identify paged content objects.
    $parent_object = islandora_object_load($parent);
    islandora_xacml_editor_apply_parent_policy($object, $parent_object);
  }

  islandora_add_object($object);
}

/**
 * Alters page and sequence number of page object to allow for inserted pages.
 *
 * @param array $page_to_reindex
 *   Array Containing page pid and current page number
 * @param int $offset
 *   Offset to determine new page number.
 */
function islandora_paged_content_insert_sequence_gap($page_to_reindex, $offset) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $new_page_num = (int) $page_to_reindex['page'] + $offset;
  $page_object = islandora_object_load($page_to_reindex['pid']);
  $rels_ext = $page_object->relationships;
  islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isSequenceNumber', (string) $new_page_num, TRUE);
  islandora_paged_content_set_relationship($rels_ext, ISLANDORA_RELS_EXT_URI, 'isPageNumber', (string) $new_page_num, TRUE);
}

/**
 * Batch 'finished' callback.
 *
 * Deletes the temporary files associated with the ingested pages.
 */
function islandora_paged_content_zipped_upload_ingest_finished($success, $results, $operations) {
  file_unmanaged_delete_recursive($results['pages_directory']);
  if ($success) {
    drupal_set_message(t('Successfully added @pages pages to @label.', array(
      '@pages' => $results['file_count'],
      '@label' => $results['book_label'],
    )));
  }
  else {
    drupal_set_message(t('Problems were encountered while trying to add pages to @label. Please contact an administrator or check the logs for details.', array(
      '@label' => $results['book_label'],
    )));
  }
}

/**
 * Sets the page progression for a paged content.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 * @param AbstractObject $object
 *   The object to fetch the pages from.
 *
 * @return array
 *   The Drupal form.
 */
function islandora_paged_content_manage_page_progression_form(array $form, array &$form_state, AbstractObject $object) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $page_progression = islandora_paged_content_get_page_progression($object);
  $form_state['pid'] = $object->id;
  return array(
    'page_progression' => array(
      '#type' => 'radios',
      '#title' => t('Page Progression'),
      '#options' => array(
        'lr' => t('Left-to-Right'),
        'rl' => t('Right-to-Left'),
      ),
      '#default_value' => $page_progression,
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Set page progression'),
    ),
  );
}

/**
 * Submit handler to set page progression for paged content.
 *
 * @param array $form
 *   An array representing a form within Drupal.
 * @param array $form_state
 *   An array containing the Drupal form state.
 */
function islandora_paged_content_manage_page_progression_form_submit($form, &$form_state) {
  module_load_include('inc', 'islandora_paged_content', 'includes/utilities');
  $object = islandora_object_load($form_state['pid']);
  islandora_paged_content_set_page_progression($object, $form_state['values']['page_progression']);
  drupal_set_message(t('Page progression updated.'));
}
