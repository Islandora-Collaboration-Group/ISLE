<?php

/**
 * @file
 * Extract the version from the Javascript file.
 *
 * @return string
 *   The version string, or NULL if we couldn't find it.
 */

/**
 * Get the installed version.
 */
function islandora_openseadragon_get_installed_version() {
  $version = &drupal_static(__FUNCTION__, NULL);

  if ($version === NULL) {
    $path = libraries_get_path('openseadragon');
    $openseadragon_js = file_get_contents("$path/openseadragon.js");
    $matches = array();
    $count = preg_match('/@version\s+OpenSeadragon\s+((\d+\.){2}\d+)/i', $openseadragon_js, $matches);
    if ($count) {
      $version = $matches[1];
    }
  }

  return $version;
}

/**
 * Helper to construct the URL used to retrieve a "clip" from Djatoka.
 *
 * @param string $params
 *   A string containing the $_GET clip value to be parsed.
 * @param bool $download
 *   Whether the URL being generated is for a download link.
 *
 * @return bool|array
 *   FALSE when invalid parameters passed in, otherwise an array containing:
 *   -original_params (string): A rawurlencoded string which can be used as or
 *   appended to the URL query string.
 *   -image_url (string): URL to retrieve the image from the image server.
 *   -dimensions (array): An array containing the width and height of the image.
 *   Used when rendering the image on the print page.
 */
function islandora_openseadragon_construct_clip_url($params, $download = FALSE) {
  global $base_root;
  $settings = islandora_openseadragon_get_settings();
  $decoded_params = drupal_get_query_array($params);
  if (islandora_openseadragon_use_djatoka_server()) {
    $djatoka_url = url(variable_get('islandora_openseadragon_djatoka_url', 'adore-djatoka/resolver'),
      array('absolute' => TRUE)
    );
    $djatoka_params = array(
      'url_ver' => 'Z39.88-2004',
      'svc_id' => 'info:lanl-repo/svc/getRegion',
      'svc_val_fmt' => 'info:ofi/fmt:kev:mtx:jpeg2000',
      'svc.format' => 'image/jpeg',
    );
    // Only grab the two parameters that are expected to be there, ignore
    // anything else.
    $allowed_djatoka_param_keys = drupal_map_assoc(array(
      'rft_id',
      'svc.region',
    ));
    $allowed_djatoka_params = array_intersect_key($decoded_params, $allowed_djatoka_param_keys);
    if (empty($allowed_djatoka_params)) {
      return FALSE;
    }
    $rft_id = $decoded_params['rft_id'];
    $path = parse_url($rft_id, PHP_URL_PATH);
    // Replace the first left-most slash to get a more Drupal-y path to validate
    // against.
    $drupal_path = preg_replace('/\//', '', $path, 1);
    $item = menu_get_item($drupal_path);
    if (!$item || $item['path'] !== 'islandora/object/%/datastream/%/view') {
      return FALSE;
    }
    $djatoka_url_params = drupal_http_build_query($djatoka_params + $allowed_djatoka_params);
    // XXX: See if Djatoka is relative or absolute and handle accordingly.
    // Building up URL this way because may encounter a relative or absolute
    // path to Djatoka. Similarly, the built in Drupal functions such as URL
    // will construct URLs with the multi-site/language prefixs present. Lastly,
    // not using url_is_external as a relative Djatoka path may have two slashes
    // preceding it which it would flag falsely.
    if (!$download || strpos($djatoka_url, 'http') === 0) {
      $url = "$djatoka_url?$djatoka_url_params";
    }
    else {
      $url = "{$base_root}{$djatoka_url}?$djatoka_url_params";
    }
    $dimensions = array();
    if (isset($decoded_params['dimensions'])) {
      $dimensions = explode(',', $decoded_params['dimensions']);
      $dimensions = array(
        'width' => $dimensions[0],
        'height' => $dimensions[1],
      );
    }
    return array(
      // XXX: Using drupal_http_build_query to avoid changing the underlying
      // functionality in the solution packs that currently implement.
      'original_params' => drupal_http_build_query($allowed_djatoka_params),
      'image_url' => $url,
      'dimensions' => $dimensions,
    );
  }
  else {
    $identifier = $decoded_params['identifier'];
    $region = $decoded_params['region'];
    $size = $decoded_params['size'];
    $rotation = 0;
    $format = "default.jpg";
    return array(
      'original_params' => drupal_http_build_query($decoded_params),
      'image_url' => "{$identifier}/{$region}/{$size}/{$rotation}/{$format}",
      'dimensions' => array(),
    );
  }
}

/**
 * Get OpenSeadragon settings.
 *
 * @param bool $filter_null
 *   Strip NULL values, used when returning to Javascript.
 *
 * @return array
 *   Configuration options for OpenSeadragon.
 */
function islandora_openseadragon_get_settings($filter_null = TRUE) {
  $settings = variable_get('islandora_openseadragon_settings', array()) + islandora_openseadragon_default_options();
  // We have to filter NULL values, to prevent issues with javascript.
  $is_not_null = function ($val) {
    return !is_null($val);
  };
  if ($filter_null) {
    $settings = array_filter($settings, $is_not_null);
  }
  return $settings;
}

/**
 * Gets the default settings for the OpenSeadragon viewer.
 *
 * @see https://openseadragon.github.io/docs/OpenSeadragon.html#.Options
 *
 * @return array
 *   An associative array to pass to customize the behavior of OpenSeadragon.
 */
function islandora_openseadragon_default_options() {
  $library_path = libraries_get_path('openseadragon');
  $defaults = array(
    'id' => 'islandora-openseadragon',
    'element' => NULL,
    'tileSources' => NULL,
    'tabIndex' => 0,
    'overlays' => NULL,
    'prefixUrl' => url("{$library_path}/images/", array('absolute' => TRUE)),
    'navImages' => NULL,
    'debugMode' => FALSE,
    'debugGridColor' => '#437AB2',
    'blendTime' => 0,
    'alwaysBlend' => FALSE,
    'autoHideControls' => TRUE,
    'immediateRender' => FALSE,
    'defaultZoomLevel' => 0,
    'opacity' => 1,
    'compositeOperation' => NULL,
    'placeholderFillStyle' => NULL,
    'degrees' => 0,
    'minZoomLevel' => NULL,
    'maxZoomLevel' => NULL,
    'homeFillsViewer' => FALSE,
    'panHorizontal' => TRUE,
    'panVertical' => TRUE,
    'constrainDuringPan' => FALSE,
    'wrapHorizontal' => FALSE,
    'wrapVertical' => FALSE,
    'minZoomImageRatio' => 0.9,
    'maxZoomPixelRatio' => 1.1,
    'smoothTileEdgesMinZoom' => 1.1,
    'iOSDevice' => NULL,
    'autoResize' => TRUE,
    'preserveImageSizeOnResize' => FALSE,
    'minScrollDeltaTime' => 50,
    'pixelsPerWheelLine' => 40,
    'visibilityRatio' => 0.5,
    'viewportMargins' => NULL,
    'imageLoaderLimit' => 0,
    'clickTimeThreshold' => 300,
    'clickDistThreshold' => 5,
    'dblClickTimeThreshold' => 300,
    'dblClickDistThreshold' => 20,
    'springStiffness' => 6.5,
    'animationTime' => 1.2,
    'gestureSettingsMouse' => array(
      'scrollToZoom' => TRUE,
      'clickToZoom' => TRUE,
      'dblClickToZoom' => FALSE,
      'pinchToZoom' => FALSE,
      'flickEnabled' => FALSE,
      'flickMinSpeed' => 120,
      'flickMomentum' => 0.25,
      'pinchRotate' => FALSE,
    ),
    'gestureSettingsTouch' => array(
      'scrollToZoom' => FALSE,
      'clickToZoom' => FALSE,
      'dblClickToZoom' => TRUE,
      'pinchToZoom' => TRUE,
      'flickEnabled' => TRUE,
      'flickMinSpeed' => 120,
      'flickMomentum' => 0.25,
      'pinchRotate' => FALSE,
    ),
    'gestureSettingsPen' => array(
      'scrollToZoom' => FALSE,
      'clickToZoom' => TRUE,
      'dblClickToZoom' => FALSE,
      'pinchToZoom' => FALSE,
      'flickEnabled' => FALSE,
      'flickMinSpeed' => 120,
      'flickMomentum' => 0.25,
      'pinchRotate' => FALSE,
    ),
    'gestureSettingsUnknown' => array(
      'scrollToZoom' => TRUE,
      'clickToZoom' => FALSE,
      'dblClickToZoom' => TRUE,
      'pinchToZoom' => TRUE,
      'flickEnabled' => TRUE,
      'flickMinSpeed' => 120,
      'flickMomentum' => 0.25,
      'pinchRotate' => FALSE,
    ),
    'zoomPerClick' => 2.0,
    'zoomPerScroll' => 1.2,
    'zoomPerSecond' => 1.0,
    'navigatorId' => NULL,
    'showNavigator' => FALSE,
    'navigatorPosition' => 'TOP_RIGHT',
    'navigatorSizeRatio' => 0.2,
    'navigatorMaintainSizeRatio' => FALSE,
    'navigatorTop' => NULL,
    'navigatorLeft' => NULL,
    'navigatorHeight' => NULL,
    'navigatorWidth' => NULL,
    'navigatorAutoResize' => TRUE,
    'navigatorAutoFade' => TRUE,
    'navigatorRotate' => TRUE,
    'controlsFadeDelay' => 2000,
    'controlsFadeLength' => 1500,
    'maxImageCacheCount' => 200,
    'timeout' => 30000,
    'useCanvas' => TRUE,
    'minPixelRatio' => 0.5,
    'mouseNavEnabled' => TRUE,
    'showNavigationControl' => TRUE,
    'navigationControlAnchor' => 'TOP_LEFT',
    'showZoomControl' => TRUE,
    'showHomeControl' => TRUE,
    'showFullPageControl' => TRUE,
    'showRotationControl' => FALSE,
    'showSequenceControl' => TRUE,
    'sequenceControlAnchor' => 'TOP_LEFT',
    'navPrevNextWrap' => FALSE,
    'zoomInButton' => NULL,
    'zoomOutButton' => NULL,
    'homeButton' => NULL,
    'fullPageButton' => NULL,
    'rotateLeftButton' => NULL,
    'rotateRightButton' => NULL,
    'previousButton' => NULL,
    'nextButton' => NULL,
    'sequenceMode' => FALSE,
    'initialPage' => 0,
    'preserveViewport' => FALSE,
    'preserveOverlays' => FALSE,
    'showReferenceStrip' => FALSE,
    'referenceStripScroll' => 'horizontal',
    'referenceStripElement' => NULL,
    'referenceStripHeight' => NULL,
    'referenceStripWidth' => NULL,
    'referenceStripPosition' => 'BOTTOM_LEFT',
    'referenceStripSizeRatio' => 0.2,
    'collectionMode' => FALSE,
    'collectionRows' => 3,
    'collectionColumns' => 0,
    'collectionLayout' => 'horizontal',
    'collectionTileSize' => 800,
    'collectionTileMargin' => 80,
    'crossOriginPolicy' => FALSE,
    'ajaxWithCredentials' => FALSE,
  );
  return $defaults;
}

/**
 * Image metadata pertinent to the users chosen TileSources implementation.
 *
 * @param string $identifier
 *   The URL to the image to generate the tile source for.
 *
 * @return mixed
 *   The metadata required by the users chosen implementation.
 */
function islandora_openseadragon_identifier_tile_source($identifier) {
  switch (variable_get('islandora_openseadragon_tilesource', 'djatoka')) {
    case 'djatoka':
      return islandora_openseadragon_djatoka_tile_source($identifier);

    case 'iiif':
      return islandora_openseadragon_iiif_tile_source($identifier);

    default;
      return array();
  }
}

/**
 * Image metadata pertinent to the users chosen TileSources implementation.
 *
 * @param AbstractDatastream $datastream
 *   The datastream for which to fetch the tile source implementation.
 *
 * @return mixed
 *   The metadata required by the users chosen implementation.
 */
function islandora_openseadragon_datastream_tile_source(AbstractDatastream $datastream) {
  $identifier = islandora_openseadragon_identifier_url($datastream);
  return islandora_openseadragon_identifier_tile_source($identifier);
}

/**
 * Image metadata pertinent to the IIIF URL specification.
 *
 * @param string $identifier
 *   The URL to the image to generate the tile source for.
 *
 * @return string|bool
 *   The IIIF URL to the image source if the URL can be successfully created,
 *   FALSE otherwise.
 */
function islandora_openseadragon_iiif_tile_source($identifier) {
  $base_url = rtrim(variable_get('islandora_openseadragon_iiif_url', 'iiif'), '/');
  $identifier = urlencode($identifier);
  return file_create_url("$base_url/$identifier");
}

/**
 * Image metadata pertinent to the Djatoka TileSources implementation.
 *
 * @param string $identifier
 *   The URL to the image to generate the tile source for.
 *
 * @return array
 *   An associative array with the required metadata for AdoraDjatoka tile
 *   source to be rendered.
 *   - identifier: A URL where the datastream image can be accessed.
 *   - width: The width of the image in pixels if known.
 *   - height: The height of the image in pixels if known.
 *   - tileWidth: The width of the tiles to assumed to make up each pyramid
 *     layer in pixels.
 *   - tileHeight: The height of the tiles to assumed to make up each pyramid
 *     layer in pixels.
 *   - tileOverlap: The number of pixels each tile is expected to overlap
 *     touching tiles.
 *   - minLevel: The minimum level to attempt to load.
 *   - maxLevel: The maximum level to attempt to load.
 */
function islandora_openseadragon_djatoka_tile_source($identifier) {
  module_load_include('inc', 'islandora', 'includes/authtokens');
  return array(
    'identifier' => $identifier,
    'width' => NULL,
    'height' => NULL,
    'tileSize' => variable_get('islandora_openseadragon_tile_size', ISLANDORA_OPENSEADRAGON_DEFAULT_TILE_SIZE),
    'tileOverlap' => variable_get('islandora_openseadragon_tile_overlap', ISLANDORA_OPENSEADRAGON_DEFAULT_TILE_OVERLAP),
    'minLevel' => NULL,
    'maxLevel' => NULL,
  );
}

/**
 * Generates a URL to the given image datastream the image server can access.
 *
 * @param AbstractDatastream $datastream
 *   Used to generates the identifier URL.
 *
 * @return string
 *   The URL at which an image server can access the given image datastream.
 */
function islandora_openseadragon_identifier_url(AbstractDatastream $datastream) {
  module_load_include('inc', 'islandora', 'includes/authtokens');
  $pid = $datastream->parent->id;
  $dsid = $datastream->id;
  $token = islandora_get_object_token($pid, $dsid, 2);
  return url("islandora/object/{$pid}/datastream/{$dsid}/view",
    array(
      'absolute' => TRUE,
      'query' => array(
        'token' => $token,
      ),
      'language' => language_default(),
      'https' => FALSE,
    )
  );
}

/**
 * Performs a solr query and generates overlays for matching terms from HOCR.
 *
 * @param string $pid
 *   The object in which to query against.
 *
 * @return array
 *   The overlays to render in the OpenSeadragon viewer.
 */
function islandora_openseadragon_viewer_query_solr_for_overlays($pid) {
  // Only get solr results if present and paged content solution pack is
  // enabled.
  if (empty($_GET['solr']) || !module_exists('islandora_paged_content')) {
    return NULL;
  }

  $params = $_GET['solr']['params'];
  $params['fq'][] = format_string('PID:"!pid"', array(
    '!pid' => $pid,
  ));

  module_load_include('inc', 'islandora_paged_content', 'includes/highlighting');
  $highlighting_info = islandora_paged_content_perform_solr_highlighting_query($_GET['solr']['query'], $params);
  $highlights = array();

  foreach ($highlighting_info as $pid => $info) {
    if (array_key_exists('snippets', $info)) {
      foreach ($info['snippets'] as $snippet => $bounding_boxes) {
        foreach ($bounding_boxes as $bounding_box) {
          // The size on the x axis is used to calculate the proportional
          // values under the openseadragon coordinate system.
          $highlights[] = array(
            'x' => $bounding_box['left'] / $info['page']['width'],
            'y' => $bounding_box['top'] / $info['page']['width'],
            'width' => ($bounding_box['right'] - $bounding_box['left']) / $info['page']['width'],
            'height' => ($bounding_box['bottom'] - $bounding_box['top']) / $info['page']['width'],
            'className' => "islandora-openseadragon-highlight",
          );
        }
      }
    }
  }
  return $highlights;
}

/**
 * Check if we djatoka server is to be used.
 *
 * @return bool
 *   TRUE if Djatoka server is to be used.
 */
function islandora_openseadragon_use_djatoka_server() {
  return variable_get('islandora_openseadragon_tilesource', 'djatoka') == 'djatoka';
}
