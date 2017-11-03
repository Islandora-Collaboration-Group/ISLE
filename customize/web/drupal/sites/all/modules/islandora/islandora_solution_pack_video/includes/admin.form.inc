<?php

/**
 * @file
 * Handles the display/submission of the admin settings form for this module.
 */

/**
 * Defines the admin settings form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_video_admin(array $form, array &$form_state) {
  module_load_include('inc', 'islandora', 'includes/solution_packs');
  $form = array();
  // Get viewer table.
  $viewer_table = islandora_viewers_form('islandora_video_viewers', 'video/mp4');
  $form += $viewer_table;

  // Viewer stuff.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#weight' => 1,
    '#submit' => array('islandora_video_admin_submit'),
  );

  // Misc config.
  $form['islandora_video_retain_original'] = array(
    '#type' => 'checkbox',
    '#title' => t('Keep original file after ingest'),
    '#description' => t('Stores the originally uploaded video file for archival purposes.'),
    '#default_value' => variable_get('islandora_video_retain_original', TRUE),
  );

  // FFMPEG configuration.
  $form['islandora_video_ffmpeg_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to ffmpeg executable.'),
    '#default_value' => variable_get('islandora_video_ffmpeg_path', 'ffmpeg'),
    '#description' => t('Path to ffmpeg on your server.'),
    '#required' => TRUE,
  );
  $form['islandora_video_make_archive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create .mkv derivative'),
    '#description' => t('Appends a video datastream to the object using the Matroska container.'),
    '#default_value' => variable_get('islandora_video_make_archive', TRUE),
  );
  $form['islandora_video_make_mp4_locally'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create display MP4 locally'),
    '#description' => t('Leave this box checked unless processing of files is done on an external server.'),
    '#default_value' => variable_get('islandora_video_make_mp4_locally', TRUE),
  );
  $form['islandora_video_mp4_audio_codec'] = array(
    '#type' => 'textfield',
    '#title' => t('MP4 audio codec'),
    '#description' => t("Defaults to libfaac, a non-free encoder. Ffmpeg must have been compiled from source with that encoder enabled. See !FFMPEG for more info.",
      array(
        '!FFMPEG' => l(t("FFmpeg's AAC encoding guide"), 'https://trac.ffmpeg.org/wiki/Encode/AAC'),
      )),
    '#default_value' => variable_get('islandora_video_mp4_audio_codec', 'libfaac'),
  );

  // Theora configuration.
  $form['islandora_video_make_ogg_locally'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create OGG locally'),
    '#description' => t("Check this box if an optional OGG archival datastream is desired and should being created locally. Please note that OGG derivatives are no longer used within Islandora and this option remains to be consistent."),
    '#default_value' => variable_get('islandora_video_make_ogg_locally', FALSE),
  );
  $form['islandora_video_ffmpeg2theora_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path to Theora executable'),
    '#description' => t('Path to Theora conversion program on your server'),
    '#default_value' => variable_get('islandora_video_ffmpeg2theora_path', 'ffmpeg2theora'),
    '#states' => array(
      'visible' => array(
        ':input[name="islandora_video_make_ogg_locally"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['islandora_video_make_thumbnail_locally'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create thumbnail locally'),
    '#description' => t('Leave this box checked unless processing of files is done on an external server.'),
    '#default_value' => variable_get('islandora_video_make_thumbnail_locally', TRUE),
  );
  // Fallback to OBJ datastream.
  $form['islandora_video_play_obj'] = array(
    '#type' => 'checkbox',
    '#title' => t('Play OBJ datastream if MP4 is not present'),
    '#description' => t('Check this box to use the OBJ datastream in the viewer (configured above) if the MP4 datastream is not present and the OBJ datastream has mimetype video/mp4.'),
    '#default_value' => variable_get('islandora_video_play_obj', TRUE),
  );
  $form['islandora_video_max_obj_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum file size for playing OBJ (in MB)'),
    '#description' => t("Set a maximum size (in megabytes) for sending the OBJ datastream to the player."),
    '#default_value' => variable_get('islandora_video_max_obj_size', '500'),
  );

  return system_settings_form($form);
}

/**
 * Form submit for allowing the deletion of the viewer variable.
 */
function islandora_video_admin_submit($form, &$form_state) {
  $op = $form_state['clicked_button']['#id'];
  switch ($op) {
    case 'edit-reset':
      variable_del('islandora_video_viewers');
      variable_del('islandora_video_ffmpeg_path');
      variable_del('islandora_video_retain_original');
      variable_del('islandora_video_make_archive');
      variable_del('islandora_video_make_mp4_locally');
      variable_del('islandora_video_make_ogg_locally');
      variable_del('islandora_video_ffmpeg2theora_path');
      variable_del('islandora_video_mp4_audio_codec');
      variable_del('islandora_video_play_obj');
      variable_del('islandora_video_max_obj_size');
      break;
  }
}

/**
 * Form validation ensuring the selected ffmpeg options are available.
 */
function islandora_video_admin_validate($form, &$form_state) {
  // Ensure the mp4 audio codec is enabled.
  $raw_value = $form_state['values']['islandora_video_mp4_audio_codec'];
  if ($raw_value) {
    if (preg_match('/[^0-9a-zA-Z_-]/', $raw_value) === 1) {
      form_set_error('', "The value entered for MP4 audio codec contains forbidden characters.");
      return;
    }
    $ffmpeg = ($form_state['values']['islandora_video_ffmpeg_path'] !== variable_get('islandora_video_ffmpeg_path', 'ffmpeg') ? $form_state['values']['islandora_video_ffmpeg_path'] : variable_get('islandora_video_ffmpeg_path', 'ffmpeg'));
    $safe_value = strtolower(trim($raw_value));
    $command = $ffmpeg . ' -encoders 2>/dev/null | grep "^ ...... ' . $safe_value . ' "';
    exec($command, $output, $ret);
    if (!$output) {
      form_set_error('', 'The selected MP4 codec was not found in ffmpeg. Try using aac or enable the desired codec.');
    }
    $form_state['values']['islandora_video_mp4_audio_codec'] = $safe_value;
  }
  else {
    $form_state['values']['islandora_video_mp4_audio_codec'] = 'libfaac';
  }
}
