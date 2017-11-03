<?php

/**
 * @file
 * This file contains all the functions for image manipulation used in the large
 * image solution pack.
 */

/**
 * Fetches the upload image file from Fedora, and saves it to a temp location.
 *
 * @param AbstractObject $object
 *   The object to fetch the uploaded image file from.
 *
 * @return string
 *   The file path to the temp file if successful, FALSE otherwise.
 */
function islandora_large_image_get_uploaded_file(AbstractObject $object) {
  $base_name = str_replace(':', '-', $object->id);
  $mime_detector = new MimeDetect();
  $ext = $mime_detector->getExtension($object['OBJ']->mimeType);
  $filename = file_create_filename("{$base_name}_OBJ.{$ext}", 'temporary://');
  $object['OBJ']->getContent($filename);
  return $filename;
}

/**
 * Creates the JP2 derived datatstream from the uploaded image file.
 *
 * @param AbstractObject $object
 *   The object to add the derived datastream to.
 * @param bool $force
 *   Whether or not we should force the derivatives to be created, in the case
 *   that they already exist on the given object.
 *
 * @return bool
 *   TRUE if successful, FALSE otherwise.
 */
function islandora_large_image_create_jp2_derivative(AbstractObject $object, $force = TRUE) {
  module_load_include('inc', 'islandora_large_image', 'includes/utilities');
  $base_name = str_replace(':', '-', $object->id);
  if ($force || !isset($object['JP2'])) {
    $to_return = array(
      'success' => FALSE,
      'messages' => array(),
    );
    $uploaded_file = islandora_large_image_get_uploaded_file($object);
    $lossless = variable_get('islandora_lossless', FALSE);
    if (islandora_large_image_is_jp2($uploaded_file) && !$lossless) {
      $derivative_file = $uploaded_file;
    }
    else {

      $depth = islandora_large_image_get_bit_depth($uploaded_file);

      // Create JP2 with kakadu.
      $kakadu = variable_get('islandora_use_kakadu', !islandora_large_image_check_imagemagick_for_jpeg2000());
      $mime = strtolower($object['OBJ']->mimetype);
      if ($mime == 'image/jpeg' || $mime == 'image/jpg' || $depth < 8) {
        $kakadu = FALSE;
      }
      // Use lossless if the image would look too bad with compression.
      if (!$lossless) {
        $identify = islandora_large_image_get_identify();
        // Define all these variables in one call for a large performance gain.
        $file_path = drupal_realpath($uploaded_file);
        list($height, $width, $y_resolution, $x_resolution) = explode(
          ',',
          exec(escapeshellcmd("$identify -format \"%h,%W,%y,%x\" $file_path"))
        );
        if (((int) $x_resolution < 300 || (int) $y_resolution < 300) ||
          ($height < 1024 || $width < 1024)) {
          $lossless = TRUE;
        }
      }

      if ($kakadu) {
        // Create JP2.
        if (!$lossless) {
          $derivative_file = islandora_large_image_kdu_compress($uploaded_file, "temporary://{$base_name}_JP2.jp2");
        }
        else {
          $derivative_file = islandora_large_image_kdu_compress(
            $uploaded_file,
            "temporary://{$base_name}_JP2.jp2",
            'Creversible=yes -rate -,1,0.5,0.25 Clevels=5'
          );
        }
        if ($derivative_file === FALSE) {
          $to_return['messages'][] = array(
            'message' => t("Kakadu failed. Trying ImageMagick ..."),
            'type' => 'dsm',
            'severity' => 'status',
          );
          // Force retry with ImageMagick if Kakadu has failed.
          $kakadu = FALSE;
        }
      }
      if (!$kakadu) {
        $args = islandora_large_image_get_args($lossless);
        if ($depth < 8) {
          $args = array_merge($args, array("-depth 8"));
        }
        $derivative_file = islandora_large_image_imagemagick_convert(
          $uploaded_file,
          "temporary://{$base_name}_JP2.jp2",
          $args);
      }
    }
    if ($derivative_file === FALSE) {
      $to_return['messages'][] = array(
        'message' => t('Failed to create JP2 derivative.'),
        'type' => 'watchdog',
        'severity' => WATCHDOG_ERROR,
      );
    }
    else {
      $added_successfully = islandora_large_image_add_datastream($object, 'JP2', $derivative_file, 'image/jp2', t('JPEG 2000'));
      if (TRUE === $added_successfully) {
        $to_return['messages'][] = array(
          'message' => t('Created JP2 derivative.'),
          'type' => 'dsm',
          'severity' => 'status',
        );
        $to_return['success'] = TRUE;
      }
      else {
        $to_return['messages'][] = array(
          'message' => t('Failed to add JP2 derivative to the object. Error message: @message', array('@message' => $added_successfully)),
          'type' => 'dsm',
          'severity' => 'warning',
        );
      }
    }
    file_unmanaged_delete($uploaded_file);
    file_unmanaged_delete($derivative_file);
    return $to_return;
  }
}

/**
 * Creates the JPG derived datatstream from the uploaded image file.
 *
 * @param AbstractObject $object
 *   The object to add the derived datastream to.
 * @param bool $force
 *   Whether or not we should force the derivatives to be created, in the case
 *   that they already exist on the given object.
 *
 * @return bool
 *   TRUE if successful, FALSE otherwise.
 */
function islandora_large_image_create_jpg_derivative(AbstractObject $object, $force = TRUE) {
  $base_name = str_replace(':', '-', $object->id);
  if ($force || !isset($object['JPG'])) {
    $to_return = array(
      'success' => FALSE,
      'messages' => array(),
    );

    $uploaded_file = islandora_large_image_get_uploaded_file($object);
    $args = array();
    $args[] = '-resize ' . escapeshellarg("600 x 800");
    $args[] = '-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75));
    $derivative_file = islandora_large_image_imagemagick_convert($uploaded_file, "temporary://{$base_name}_JPG.jpg", $args);
    if ($derivative_file === FALSE) {
      $to_return['messages'][] = array(
        'message' => t('Failed to create JPG derivative.'),
        'type' => 'watchdog',
        'severity' => WATCHDOG_WARNING,
      );
    }
    else {
      $added_successfully = islandora_large_image_add_datastream($object, 'JPG', $derivative_file, 'image/jpeg', t('Medium sized JPEG'));
      if (TRUE === $added_successfully) {
        $to_return['messages'][] = array(
          'message' => t('Created JPG derivative.'),
          'type' => 'dsm',
          'severity' => 'status',
        );
        $to_return['success'] = TRUE;
      }
      else {
        $to_return['messages'][] = array(
          'message' => t('Failed to add JPG derivative to the object. Error message: @message', array('@message' => $added_successfully)),
          'type' => 'dsm',
          'severity' => 'warning',
        );
      }
    }

    file_unmanaged_delete($uploaded_file);
    file_unmanaged_delete($derivative_file);
    return $to_return;
  }
}

/**
 * Creates the TN derived datatstream from the uploaded image file.
 *
 * @param AbstractObject $object
 *   The object to add the derived datastream to.
 * @param bool $force
 *   Whether or not we should force the derivatives to be created, in the case
 *   that they already exist on the given object.
 *
 * @return bool
 *   TRUE if successful, FALSE otherwise.
 */
function islandora_large_image_create_tn_derivative(AbstractObject $object, $force = TRUE) {
  $base_name = str_replace(':', '-', $object->id);
  if ($force || !isset($object['TN'])) {
    $to_return = array(
      'success' => FALSE,
      'messages' => array(),
    );

    $uploaded_file = islandora_large_image_get_uploaded_file($object);

    $args = array();
    $args[] = '-quality ' . escapeshellarg(variable_get('imagemagick_quality', 75));
    $args[] = '-resize ' . escapeshellarg("200 x 200");
    $derivative_file = islandora_large_image_imagemagick_convert($uploaded_file, "temporary://{$base_name}_TN.jpg", $args);
    if ($derivative_file === FALSE) {
      $to_return['messages'][] = array(
        'message' => t('Failed to create TN derivative.'),
        'type' => 'watchdog',
        'severity' => WATCHDOG_WARNING,
      );
    }
    else {
      $added_successfully = islandora_large_image_add_datastream($object, 'TN', $derivative_file, 'image/jpeg', t('Thumbnail'));
      if (TRUE === $added_successfully) {
        $to_return['messages'][] = array(
          'message' => t('Created TN derivative.'),
          'type' => 'dsm',
          'severity' => 'status',
        );
        $to_return['success'] = TRUE;
      }
      else {
        $to_return['messages'][] = array(
          'message' => t('Failed to add TN derivative to the object. Error message: @message', array('@message' => $added_successfully)),
          'type' => 'dsm',
          'severity' => 'warning',
        );
      }
    }

    file_unmanaged_delete($uploaded_file);
    file_unmanaged_delete($derivative_file);
    return $to_return;
  }
}

/**
 * Function kdu_compress.
 *
 * Calls kdu_compress with the given arguments, or defaults if none are
 * provided.
 *
 * @param string $src
 *   The source file to pass to kdu_compress
 * @param string $dest
 *   The destination file which kdu_compress will generate.
 * @param string $args
 *   Any additional arguments for kdu_compress, if not provided reasonable
 *   defaults will be used.
 *
 * @return file
 *   Returns the newly generated file uri or FALSE if the conversion failed
 */
function islandora_large_image_kdu_compress($src, $dest, $args = NULL) {
  // Kakadu does a poor job of converting low bit depths.
  module_load_include('inc', 'islandora_large_image', 'includes/utilities');
  if (islandora_large_image_get_bit_depth($src) < 8) {
    return FALSE;
  }

  // Kinda weird logic, to avoid changing the source.
  $unlink_source = FALSE;

  // First, convert to (potentially uncompressed) TIFF file.
  if (!islandora_large_image_is_uncompressed_tiff($src)) {
    $c_args = array();

    // If the source is not a TIFF, or we have been instructed to uncompress
    // TIFFs, make it so.
    if (!islandora_large_image_is_tiff($src) ||
      variable_get('islandora_large_image_uncompress_tiff', TRUE)) {
      $c_args[] = "-compress None";
    }
    $c_dest = file_create_filename(drupal_basename($src) . ".tif", 'temporary://');
    $src = islandora_large_image_imagemagick_convert($src, $c_dest, $c_args);
    $unlink_source = TRUE;
  }
  // Default args.
  $args = isset($args) ? $args : '-rate 0.5 Clayers=1 Clevels=7 "Cprecincts={256,256},{256,256},{256,256},{128,128},{128,128},{64,64},{64,64},{32,32},{16,16}" "Corder=RPCL" "ORGgen_plt=yes" "ORGtparts=R" "Cblk={32,32}" Cuse_sop=yes';
  $src = drupal_realpath($src);
  $dest = drupal_realpath($dest);
  $ret = -1;
  $ret_palette = 0;
  $output = array();
  $kdu_compress = variable_get('islandora_kakadu_url', 'kdu_compress');
  $command = "$kdu_compress -i \"$src\" -o \"$dest\" $args 2>&1";
  exec($command, $output, $ret);

  $output_string = implode(' ', $output);

  if (strpos($output_string, 'no_palette') !== FALSE) {
    // Warning was thrown re-create with the -no_palette flag so we don't end
    // up with a corrupted JP2.
    $args .= ' -no_palette';
    $command = "$kdu_compress -i \"$src\" -o \"$dest\" $args 2>&1";
    exec($command, $output, $ret_palette);
  }

  if ($unlink_source) {
    file_unmanaged_delete($src);
  }
  if ($ret != 0 || $ret_palette != 0) {
    return FALSE;
  }
  return $dest;
}

/**
 * Calls imagemagick's convert command with the given arguments.
 *
 * @param string $src
 *   The source file uri to convert from.
 * @param string $dest
 *   The destination file uri to convert into.
 *
 * @args int $width
 *   The source file to convert from.
 * @args int $height
 *   The destination file to convert into.
 *
 * @return string
 *   The destination file path if successful otherwise FALSE.
 */
function islandora_large_image_imagemagick_convert($src, $dest, $args) {
  $src = drupal_realpath($src);
  if (islandora_large_image_is_tiff($src)) {
    $src = $src . '[0]';
  }
  $dest = drupal_realpath($dest);
  $context = array(
    'source' => $src,
    'destination' => $dest,
  );
  drupal_alter('imagemagick_arguments', $args, $context);

  // To make use of ImageMagick 6's parenthetical command grouping we need to
  // make the $source image the first parameter and $dest the last.
  // See http://www.imagemagick.org/Usage/basics/#cmdline
  $command = escapeshellarg($src) . ' ' . implode(' ', $args) . ' ' . escapeshellarg($dest);
  $output = array();
  $ret = -1;
  if (_imagemagick_convert_exec($command, $output, $ret) !== TRUE) {
    $variables = array(
      '@ret' => $ret,
      '@command' => $command,
      '!output' => implode('<br/>', $output),
    );
    watchdog('islandora_large_image', 'ImageMagick failed to convert.<br/>Error: @ret<br/>Command: @command <br/>Output !output', $variables, WATCHDOG_ERROR);
    return FALSE;
  }
  return $dest;
}

/**
 * Adds a datastream to an object.
 *
 * @param AbstractObject $object
 *   The object to add a datastream to.
 * @param string $dsid
 *   The datastream ID to be added.
 * @param string $file
 *   The URI of the file to be added as the datastream content.
 * @param string $mimetype
 *   The mimetype of the file.
 * @param string $label
 *   The label for the datastream.
 *
 * @return bool|string
 *   TRUE if the datastream was added/modified successfully, the error message
 *   otherwise.
 */
function islandora_large_image_add_datastream(AbstractObject $object, $dsid, $file, $mimetype, $label) {
  try {
    $ingest = !isset($object[$dsid]);
    if ($ingest) {
      $ds = $object->constructDatastream($dsid, 'M');
      $ds->label = $label;
      $ds->mimeType = $mimetype;
    }
    else {
      $ds = $object[$dsid];
    }
    $ds->setContentFromFile($file);
    if ($ingest) {
      $object->ingestDatastream($ds);
    }
    return TRUE;
  }
  catch (exception $e) {
    $variables = array(
      '@ret' => $e->getTraceAsString(),
    );
    // Adding watchdog here, islandora_large_image_add_datastream() calls will
    // add a dsm statement, but not a watchdog entry.
    watchdog('islandora_large_image',
      'Large Image failed to ingest data stream.<br/>Stack: @ret',
      $variables,
      WATCHDOG_ERROR);
    $message = $e->getMessage();
    return $message;
  }
}

/**
 * Return preconfigured paramaters for ImageMagick JP2 creation.
 *
 * @param bool $lossless
 *   Weather or not the conversion should be lossless.
 *
 * @return array
 *   Returns the args
 */
function islandora_large_image_get_args($lossless) {
  if ($lossless) {
    $args[] = " -define numrlvls=6";
    $args[] = " -define jp2:tilewidth=1024";
    $args[] = " -define jp2:tileheight=1024";
    $args[] = " -define jp2:rate=1.0";
    $args[] = " -define jp2:lazy";
    $args[] = " -define jp2:prg=rlcp";
    $args[] = " -define jp2:ilyrrates='0.015625,0.01858,0.0221,0.025,0.03125,0.03716,0.04419,0.05,0.0625,0.075,0.088,0.1,0.125,0.15,0.18,0.21,0.25,0.3,0.35,0.4,0.5,0.6,0.7,0.84'";
    $args[] = " -define jp2:mode=int";
  }
  else {
    $args[] = "-define numrlvls=7";
    $args[] = "-define jp2:tilewidth=1024";
    $args[] = "-define jp2:tileheight=1024";
    $args[] = "-define jp2:rate=0.02348";
    $args[] = "-define jp2:prg=rpcl";
    $args[] = "-define jp2:mode=int";
    $args[] = "-define jp2:prcwidth=16383";
    $args[] = "-define jp2:prcheight=16383";
    $args[] = "-define jp2:cblkwidth=64";
    $args[] = "-define jp2:cblkheight=64";
    $args[] = "-define jp2:sop";
  }
  return $args;
}

/**
 * Uses Imagemagick's identify to examine the file in question.
 *
 * @param string $file
 *   A file-system path to the file in question.
 *
 * @return bool
 *   A boolean indicating if the file contains a TIFF which is not compressed,
 *   (if it could be reencoded by the "free" version of Kakadu.
 */
function islandora_large_image_is_uncompressed_tiff($file) {
  $identify = islandora_large_image_get_identify();

  $escaped_file = escapeshellarg(drupal_realpath($file));

  $compression = exec(escapeshellcmd("$identify -format \"%C\" ") . $escaped_file);

  $compressed = (strtolower($compression) != 'none');

  return !$compressed && islandora_large_image_is_tiff($file);
}

/**
 * Uses Imagemagick's identify to determine if the file is a TIFF.
 *
 * @param string $file
 *   A file-system path to the file in question.
 *
 * @return bool
 *   A boolean indicating if the file contains a TIFF.
 */
function islandora_large_image_is_tiff($file) {
  $identify = islandora_large_image_get_identify();

  $escaped_file = escapeshellarg(drupal_realpath($file));

  $codec = exec(escapeshellcmd("$identify -format \"%m\" ") . $escaped_file);

  $is_tiff = strrpos(strtolower($codec), 'tiff');

  /* Avoid false negatives after strrpos */
  if ($is_tiff !== FALSE) {
    $is_tiff = TRUE;
  }

  return $is_tiff;
}

/**
 * Uses Imagemagick's identify to determine if the file is a JP2.
 *
 * @param string $file
 *   A file-system path to the file in question.
 *
 * @return bool
 *   A boolean indicating if the file contains a JP2.
 */
function islandora_large_image_is_jp2($file) {
  $identify = islandora_large_image_get_identify();

  $escaped_file = escapeshellarg(drupal_realpath($file));

  $codec = exec(escapeshellcmd("$identify -format \"%m\" ") . $escaped_file);

  $is_jp2 = (strtolower($codec) == 'jp2');

  return $is_jp2;
}


/**
 * Attempt to get the "identify" executable...
 *
 * Assume convert is sanely named, and that identify can be obtained from the
 * same place.
 *
 * @return string
 *   The command to run for "identify".
 */
function islandora_large_image_get_identify() {
  // Get path for convert.
  $convert = variable_get('imagemagick_convert', 'convert');

  // Replace "convert" with "identify"
  $identify = str_replace('convert', 'identify', $convert);

  return $identify;
}

/**
 * Legacy function until derivative regeneration is fully flushed out for UI.
 *
 * @TODO: This is legacy functionality need to remove and update it.
 *
 * @param AbstractObject $object
 *   The object to add derivatives to.
 * @param bool $force
 *   Whether or not we should force the derivatives to be created, in the case
 *   that they already exist on the given object.
 *
 * @return bool
 *   TRUE if all derivatives were created successfully, FALSE otherwise.
 */
function islandora_large_image_create_all_derivatives(AbstractObject $object, $force = TRUE) {
  if (!isset($object['OBJ'])) {
    drupal_set_message(t('Could not create image derivatives for %s. No image file was uploaded.', array('%s' => $object->id)), 'error');
    return FALSE;
  }
  $base_name = str_replace(':', '-', $object->id);
  $uploaded_file = islandora_large_image_get_uploaded_file($object, $base_name);
  if ($uploaded_file) {
    $jp2 = islandora_large_image_create_JP2_derivative($object, $uploaded_file, $base_name, $force);
    $jpg = islandora_large_image_create_JPG_derivative($object, $uploaded_file, $base_name, $force);
    $tn = islandora_large_image_create_TN_derivative($object, $uploaded_file, $base_name, $force);
    return $jp2 && $jpg && $tn && file_unmanaged_delete($uploaded_file);
  }
  return FALSE;
}
