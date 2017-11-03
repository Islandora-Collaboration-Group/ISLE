<?php

/**
 * @file
 * Admin callbacks for Islandora OpenSeadragon.
 */

/**
 * Admin form. Descriptions taken from the deep zoom module.
 *
 * @param array $form
 *   Drupal form.
 * @param array $form_state
 *   Drupal form state.
 *
 * @return array
 *   The drupal form definition.
 */
function islandora_openseadragon_admin(array $form, array &$form_state) {
  module_load_include('inc', 'islandora', 'includes/utilities');
  module_load_include('inc', 'islandora_openseadragon', 'includes/utilities');
  $settings = islandora_openseadragon_get_settings(FALSE);
  $version = islandora_openseadragon_get_installed_version();
  $djatoka_url = islandora_system_settings_form_default_value('islandora_openseadragon_djatoka_url', 'adore-djatoka/resolver', $form_state);
  $iiif_url = islandora_system_settings_form_default_value('islandora_openseadragon_iiif_url', 'iiif', $form_state);
  $form = array(
    'islandora_openseadragon_tilesource' => array(
      '#type' => 'select',
      '#title' => t('Image Server'),
      '#description' => t('Select the image server to use with OpenSeadragon'),
      '#default_value' => variable_get('islandora_openseadragon_tilesource', 'djatoka'),
      '#options' => array(
        'djatoka' => t('Adore-Djatoka Image Server'),
        'iiif' => t('IIIF Image Server'),
      ),
    ),
    'islandora_openseadragon_fit_to_aspect_ratio' => array(
      '#type' => 'checkbox',
      '#title' => t('Constrain image to viewport'),
      '#default_value' => islandora_system_settings_form_default_value('islandora_openseadragon_fit_to_aspect_ratio', FALSE, $form_state),
      '#description' => t('On the initial page load, the entire image will be visible in the viewport.'),
    ),
    'djatoka' => array(
      '#type' => 'fieldset',
      '#title' => t('Adore Djatoka Image Server Settings'),
      '#description' => t('<p>Settings for Adore-Djatoka Image Server</p>'),
      '#states' => array(
        'visible' => array(
          ':input[name="islandora_openseadragon_tilesource"]' => array('value' => 'djatoka'),
        ),
      ),
      'islandora_openseadragon_djatoka_url' => array(
        '#prefix' => '<div id="islandora-openseadragon-djatoka-path-wrapper">',
        '#suffix' => '</div>',
        '#type' => 'textfield',
        '#title' => t('Adore-Djatoka Image Server Base URL'),
        '#title_display' => 'invisible',
        '#default_value' => $djatoka_url,
        '#description' => t('The location of the Adore-Djatoka server OpenURL resolver. <br/> !confirmation_message', array(
          '!confirmation_message' => islandora_openseadragon_admin_form_djatoka_access_message($djatoka_url),
        )),
        '#ajax' => array(
          'callback' => 'islandora_openseadragon_admin_ajax_djatoka_url',
          'wrapper' => 'islandora-openseadragon-djatoka-path-wrapper',
        ),
      ),
    ),
    'iiif' => array(
      '#type' => 'fieldset',
      '#title' => t('IIIF Image Server Settings'),
      '#description' => t('Settings for IIIF Image Server'),
      '#states' => array(
        'visible' => array(
          ':input[name="islandora_openseadragon_tilesource"]' => array('value' => 'iiif'),
        ),
      ),
      'islandora_openseadragon_iiif_url' => array(
        '#prefix' => '<div id="islandora-openseadragon-iiif-path-wrapper">',
        '#suffix' => '</div>',
        '#type' => 'textfield',
        '#title' => t('IIIF Image Server Base URL'),
        '#default_value' => $iiif_url,
        '#description' => t('The location of the IIIF Image Server.'),
      ),
    ),
    'tilesource' => array(
      '#type' => 'fieldset',
      '#title' => t('Open Seadragon Tile Source settings'),
      '#description' => t('<p>These settings will apply to all tile sources globally. See the <a href="https://openseadragon.github.io/docs/OpenSeadragon.TileSource.html#TileSource">documentation</a> for more details.</p>'),
      'islandora_openseadragon_tile_size' => array(
        '#type' => 'textfield',
        '#title' => t('Tile Size'),
        '#default_value' => variable_get('islandora_openseadragon_tile_size', ISLANDORA_OPENSEADRAGON_DEFAULT_TILE_SIZE),
        '#element_validate' => array('element_validate_number'),
        '#size' => 10,
        '#description' => t('The size of the tiles to assumed to make up each pyramid layer in pixels. Tile size determines the point at which the image pyramid must be divided into a matrix of smaller images.'),
      ),
      'islandora_openseadragon_tile_overlap' => array(
        '#type' => 'textfield',
        '#title' => t('Tile Overlap'),
        '#default_value' => variable_get('islandora_openseadragon_tile_overlap', ISLANDORA_OPENSEADRAGON_DEFAULT_TILE_OVERLAP),
        '#element_validate' => array('element_validate_number'),
        '#size' => 10,
        '#description' => t('The number of pixels each tile is expected to overlap touching tiles.'),
      ),
    ),
    'openseadragon' => array(
      '#type' => 'fieldset',
      '#title' => t('Open Seadragon Viewer settings'),
      '#description' => t('<p>Settings for OpenSeadragon %version, see the <a href="https://openseadragon.github.io/docs/OpenSeadragon.html#.Options">documentation</a> for more details.</p>', array(
        '%version' => $version,
      )),
      'islandora_openseadragon_settings' => array(
        '#type' => 'container',
        '#tree' => TRUE,
        // We don't provide "id" as configurable to users.
        // We don't provide "element" as configurable to users.
        // We don't provide "tileSources" as configurable to users.
        'tabIndex' => array(
          '#type' => 'textfield',
          '#title' => t('Tab Index'),
          '#default_value' => $settings['tabIndex'],
          '#element_validate' => array('element_validate_number'),
          '#size' => 10,
          '#description' => t('Tabbing order index to assign to the viewer element. Positive values are selected in increasing order. When tabIndex is 0 source order is used. A negative value omits the viewer from the tabbing order.'),
        ),
        'debugMode' => array(
          '#type' => 'checkbox',
          '#title' => t('Debug mode'),
          '#default_value' => $settings['debugMode'],
          '#description' => t('Toggles whether messages should be logged and fail-fast behavior should be provided.'),
        ),
        'debugGridColor' => array(
          '#type' => 'textfield',
          '#title' => t('Debug Grid Color'),
          '#default_value' => $settings['debugGridColor'],
          '#description' => t('Color of the grid in debug mode.'),
        ),
        'blendTime' => array(
          '#type' => 'textfield',
          '#title' => t('Blend time'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['blendTime'],
          '#description' => t('Specifies the duration of animation as higher or lower level tiles are replacing the existing tile.'),
        ),
        'alwaysBlend' => array(
          '#type' => 'checkbox',
          '#title' => t('Always blend'),
          '#default_value' => $settings['alwaysBlend'],
          '#description' => t("Forces the tile to always blend. By default the tiles skip blending when the blendTime is surpassed and the current animation frame would not complete the blend."),
        ),
        'autoHideControls' => array(
          '#type' => 'checkbox',
          '#title' => t('Auto-hide controls'),
          '#default_value' => $settings['autoHideControls'],
          '#description' => t("If the user stops interacting with the viewport, fade the navigation controls. Useful for presentation since the controls are by default floated on top of the image the user is viewing."),
        ),
        'immediateRender' => array(
          '#type' => 'checkbox',
          '#title' => t('Immediate render'),
          '#default_value' => $settings['immediateRender'],
          '#description' => t('Render the best closest level first, ignoring the lowering levels which provide the effect of very blurry to sharp. It is recommended to change setting to true for mobile devices.'),
        ),
        'defaultZoomLevel' => array(
          '#type' => 'textfield',
          '#title' => t('Default zoom level'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['defaultZoomLevel'],
          '#description' => t('Zoom level to use when image is first opened or the home button is clicked. If 0, adjusts to fit viewer.'),
        ),
        'opacity' => array(
          '#type' => 'textfield',
          '#title' => t('Opacity'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['opacity'],
          '#description' => t('Default opacity of the tiled images (1=opaque, 0=transparent)'),
        ),
        'compositeOperation' => array(
          '#type' => 'select',
          '#title' => t('Composite Operation'),
          '#description' => t('Select the image server to use with OpenSeadragon'),
          '#default_value' => $settings['compositeOperation'],
          '#options' => drupal_map_assoc(array(
            NULL,
            'source-over',
            'source-atop',
            'source-in',
            'source-out',
            'destination-over',
            'destination-atop',
            'destination-in',
            'destination-out',
            'lighter',
            'copy',
            'xor',
          )),
        ),
        'placeholderFillStyle' => array(
          '#type' => 'textfield',
          '#title' => t('Placeholder Fill Style'),
          '#default_value' => $settings['placeholderFillStyle'],
          '#description' => t('Draws a colored rectangle behind the tile if it is not loaded yet. You can pass a CSS color value like "#FF8800".'),
        ),
        'degrees' => array(
          '#type' => 'textfield',
          '#title' => t('Initial Rotation'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['degrees'],
          '#description' => t('Initial rotation in degrees.'),
        ),
        'minZoomLevel' => array(
          '#type' => 'textfield',
          '#title' => t('Minimum Zoom Level'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['minZoomLevel'],
          '#description' => t('Minimum Zoom Level (integer).'),
        ),
        'maxZoomLevel' => array(
          '#type' => 'textfield',
          '#title' => t('Maximum Zoom Level'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['maxZoomLevel'],
          '#description' => t('Maximum Zoom Level (integer).'),
        ),
        'homeFillsViewer' => array(
          '#type' => 'checkbox',
          '#title' => t('Home Button Fills Viewer'),
          '#default_value' => $settings['homeFillsViewer'],
          '#description' => t('Make the "home" button fill the viewer and clip the image, instead of fitting the image to the viewer and letterboxing.'),
        ),
        'panHorizontal' => array(
          '#type' => 'checkbox',
          '#title' => t('Pan horizontal'),
          '#default_value' => $settings['panHorizontal'],
          '#description' => t('Allow horizontal pan.'),
        ),
        'panVertical' => array(
          '#type' => 'checkbox',
          '#title' => t('Pan vertical'),
          '#default_value' => $settings['panVertical'],
          '#description' => t('Allow vertical pan.'),
        ),
        'constrainDuringPan' => array(
          '#type' => 'checkbox',
          '#title' => t('Constrain During Pan'),
          '#default_value' => $settings['constrainDuringPan'],
        ),
        'wrapHorizontal' => array(
          '#type' => 'checkbox',
          '#title' => t('Wrap horizontal'),
          '#default_value' => $settings['wrapHorizontal'],
          '#description' => t('Set to true to force the image to wrap horizontally within the viewport. Useful for maps or images representing the surface of a sphere or cylinder.'),
        ),
        'wrapVertical' => array(
          '#type' => 'checkbox',
          '#title' => t('Wrap vertical'),
          '#default_value' => $settings['wrapVertical'],
          '#description' => t('Set to true to force the image to wrap vertically within the viewport. Useful for maps or images representing the surface of a sphere or cylinder.'),
        ),
        'minZoomImageRatio' => array(
          '#type' => 'textfield',
          '#title' => t('Minimum zoom image ratio'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['minZoomImageRatio'],
          '#description' => t('The minimum percentage ( expressed as a number between 0 and 1 ) of the viewport height or width at which the zoom out will be constrained. Setting it to 0, for example will allow you to zoom out infinity.'),
        ),
        'maxZoomPixelRatio' => array(
          '#type' => 'textfield',
          '#title' => t('Maximum zoom pixel ratio'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['maxZoomPixelRatio'],
          '#description' => t('The maximum ratio to allow a zoom-in to affect the highest level pixel ratio. This can be set to Infinity to allow "infinite" zooming into the image though it is less effective visually if the HTML5 Canvas is not available on the viewing device.'),
        ),
        'smoothTileEdgesMinZoom' => array(
          '#type' => 'textfield',
          '#title' => t('Smooth Tile Edges Minimum Zoom'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['smoothTileEdgesMinZoom'],
          '#description' => t('A zoom percentage ( where 1 is 100% ) of the highest resolution level. When zoomed in beyond this value alternative compositing will be used to smooth out the edges between tiles. This will have a performance impact. Can be set to Infinity to turn it off. Note: This setting is ignored on iOS devices due to a known bug (See <a href="https://github.com/openseadragon/openseadragon/issues/952">https://github.com/openseadragon/openseadragon/issues/952</a>).'),
        ),
        // We don't provide "iOSDevice" as configurable to users.
        'autoResize' => array(
          '#type' => 'checkbox',
          '#title' => t('Auto Resize'),
          '#default_value' => $settings['autoResize'],
          '#description' => t('Set to false to prevent polling for viewer size changes. Useful for providing custom resize behavior.'),
        ),
        'preserveImageSizeOnResize' => array(
          '#type' => 'checkbox',
          '#title' => t('Preserve Image Size On Resize'),
          '#default_value' => $settings['preserveImageSizeOnResize'],
          '#description' => t('Set to true to have the image size preserved when the viewer is re-sized. This requires Auto Resize to be enabled (default).'),
        ),
        'minScrollDeltaTime' => array(
          '#type' => 'textfield',
          '#title' => t('Minimum Scroll Delta Time'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['minScrollDeltaTime'],
          '#description' => t('Number of milliseconds between canvas-scroll events. This value helps normalize the rate of canvas-scroll events between different devices, causing the faster devices to slow down enough to make the zoom control more manageable.'),
        ),
        'pixelsPerWheelLine' => array(
          '#type' => 'textfield',
          '#title' => t('Pixels Per Wheel Line'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['pixelsPerWheelLine'],
          '#description' => t('For pixel-resolution scrolling devices, the number of pixels equal to one scroll line.'),
        ),
        'visibilityRatio' => array(
          '#type' => 'textfield',
          '#title' => t('Visibility ratio'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['visibilityRatio'],
          '#description' => t("The percentage ( as a number from 0 to 1 ) of the source image which must be kept within the viewport. If the image is dragged beyond that limit, it will 'bounce' back until the minimum visibility ratio is achieved. Setting this to 0 and wrapHorizontal ( or wrapVertical ) to true will provide the effect of an infinitely scrolling viewport."),
        ),
        'imageLoaderLimit' => array(
          '#type' => 'textfield',
          '#title' => t('Image loader limit'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['imageLoaderLimit'],
          '#description' => t('The maximum number of image requests to make concurrently. By default it is set to 0 allowing the browser to make the maximum number of image requests in parallel as allowed by the browsers policy.'),
        ),
        'clickTimeThreshold' => array(
          '#type' => 'textfield',
          '#title' => t('Click time threshold'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['clickTimeThreshold'],
          '#description' => t('The number of milliseconds within which a pointer down-up event combination will be treated as a click gesture.'),
        ),
        'clickDistThreshold' => array(
          '#type' => 'textfield',
          '#title' => t('Click distance threshold'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['clickDistThreshold'],
          '#description' => t('The maximum distance allowed between a pointer down event and a pointer up event to be treated as a click gesture.'),
        ),
        'dblClickTimeThreshold' => array(
          '#type' => 'textfield',
          '#title' => t('Double click distance threshold'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['dblClickTimeThreshold'],
          '#description' => t('The number of milliseconds within which two pointer down-up event combinations will be treated as a double-click gesture.'),
        ),
        'dblClickDistThreshold' => array(
          '#type' => 'textfield',
          '#title' => t('Double click distance threshold'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['dblClickDistThreshold'],
          '#description' => t('The maximum distance allowed between two pointer click events to be treated as a double-click gesture.'),
        ),
        'springStiffness' => array(
          '#type' => 'textfield',
          '#title' => t('Spring stiffness'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['springStiffness'],
          '#description' => t('Determines how sharply the springs used for animations move.'),
        ),
        'animationTime' => array(
          '#type' => 'textfield',
          '#title' => t('Animation time'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['animationTime'],
          '#description' => t('Specifies the animation duration per each OpenSeadragon.Spring which occur when the image is dragged or zoomed.'),
        ),
        'gestureSettingsMouse' => array(
          '#type' => 'fieldset',
          '#title' => t('Mouse Pointer Gesture Settings'),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
          '#description' => t('<p>Settings for gestures generated by a mouse pointer device. (See <a href="https://openseadragon.github.io/docs/OpenSeadragon.html#.GestureSettings">OpenSeadragon.GestureSettings</a>)</p>'),
          'scrollToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Scroll To Zoom'),
            '#default_value' => $settings['gestureSettingsMouse']['scrollToZoom'],
            '#description' => t('Zoom on scroll gesture.'),
          ),
          'clickToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Click To Zoom'),
            '#default_value' => $settings['gestureSettingsMouse']['clickToZoom'],
            '#description' => t('Zoom on click gesture.'),
          ),
          'dblClickToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Double Click To Zoom'),
            '#default_value' => $settings['gestureSettingsMouse']['dblClickToZoom'],
            '#description' => t('Zoom on double-click gesture. Note: If set to true then clickToZoom should be set to false to prevent multiple zooms.'),
          ),
          'pinchToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Pinch To Zoom'),
            '#default_value' => $settings['gestureSettingsMouse']['pinchToZoom'],
            '#description' => t('Zoom on pinch gesture.'),
          ),
          'flickEnabled' => array(
            '#type' => 'checkbox',
            '#title' => t('Flick Gesture'),
            '#default_value' => $settings['gestureSettingsMouse']['flickEnabled'],
            '#description' => t('Enable flick gesture.'),
          ),
          'flickMinSpeed' => array(
            '#type' => 'textfield',
            '#title' => t('Flick Minimum Speed'),
            '#size' => 10,
            '#element_validate' => array('element_validate_number'),
            '#default_value' => $settings['gestureSettingsMouse']['flickMinSpeed'],
            '#description' => t('If flickEnabled is true, the minimum speed to initiate a flick gesture (pixels-per-second).'),
          ),
          'flickMomentum' => array(
            '#type' => 'textfield',
            '#title' => t('Flick Momentum'),
            '#size' => 10,
            '#element_validate' => array('element_validate_number'),
            '#default_value' => $settings['gestureSettingsMouse']['flickMomentum'],
            '#description' => t('If flickEnabled is true, the momentum factor for the flick gesture.'),
          ),
          'pinchRotate' => array(
            '#type' => 'checkbox',
            '#title' => t('Pinch Rotate'),
            '#default_value' => $settings['gestureSettingsMouse']['pinchRotate'],
            '#description' => t('If pinchRotate is true, the user will have the ability to rotate the image using their fingers.'),
          ),
        ),
        'gestureSettingsTouch' => array(
          '#type' => 'fieldset',
          '#title' => t('Touch Pointer Gesture Settings'),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
          '#description' => t('<p>Settings for gestures generated by a touch pointer device. (See <a href="https://openseadragon.github.io/docs/OpenSeadragon.html#.GestureSettings">OpenSeadragon.GestureSettings</a>)</p>'),
          'scrollToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Scroll To Zoom'),
            '#default_value' => $settings['gestureSettingsTouch']['scrollToZoom'],
            '#description' => t('Zoom on scroll gesture.'),
          ),
          'clickToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Click To Zoom'),
            '#default_value' => $settings['gestureSettingsTouch']['clickToZoom'],
            '#description' => t('Zoom on click gesture.'),
          ),
          'dblClickToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Double Click To Zoom'),
            '#default_value' => $settings['gestureSettingsTouch']['dblClickToZoom'],
            '#description' => t('Zoom on double-click gesture. Note: If set to true then clickToZoom should be set to false to prevent multiple zooms.'),
          ),
          'pinchToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Pinch To Zoom'),
            '#default_value' => $settings['gestureSettingsTouch']['pinchToZoom'],
            '#description' => t('Zoom on pinch gesture.'),
          ),
          'flickEnabled' => array(
            '#type' => 'checkbox',
            '#title' => t('Flick Gesture'),
            '#default_value' => $settings['gestureSettingsTouch']['flickEnabled'],
            '#description' => t('Enable flick gesture.'),
          ),
          'flickMinSpeed' => array(
            '#type' => 'textfield',
            '#title' => t('Flick Minimum Speed'),
            '#size' => 10,
            '#element_validate' => array('element_validate_number'),
            '#default_value' => $settings['gestureSettingsTouch']['flickMinSpeed'],
            '#description' => t('If flickEnabled is true, the minimum speed to initiate a flick gesture (pixels-per-second).'),
          ),
          'flickMomentum' => array(
            '#type' => 'textfield',
            '#title' => t('Flick Momentum'),
            '#size' => 10,
            '#element_validate' => array('element_validate_number'),
            '#default_value' => $settings['gestureSettingsTouch']['flickMomentum'],
            '#description' => t('If flickEnabled is true, the momentum factor for the flick gesture.'),
          ),
          'pinchRotate' => array(
            '#type' => 'checkbox',
            '#title' => t('Pinch Rotate'),
            '#default_value' => $settings['gestureSettingsMouse']['pinchRotate'],
            '#description' => t('If pinchRotate is true, the user will have the ability to rotate the image using their fingers.'),
          ),
        ),
        'gestureSettingsPen' => array(
          '#type' => 'fieldset',
          '#title' => t('Pen Pointer Gesture Settings'),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
          '#description' => t('<p>Settings for gestures generated by a pen pointer device. (See <a href="https://openseadragon.github.io/docs/OpenSeadragon.html#.GestureSettings">OpenSeadragon.GestureSettings</a>)</p>'),
          'scrollToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Scroll To Zoom'),
            '#default_value' => $settings['gestureSettingsPen']['scrollToZoom'],
            '#description' => t('Zoom on scroll gesture.'),
          ),
          'clickToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Click To Zoom'),
            '#default_value' => $settings['gestureSettingsPen']['clickToZoom'],
            '#description' => t('Zoom on click gesture.'),
          ),
          'dblClickToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Double Click To Zoom'),
            '#default_value' => $settings['gestureSettingsPen']['dblClickToZoom'],
            '#description' => t('Zoom on double-click gesture. Note: If set to true then clickToZoom should be set to false to prevent multiple zooms.'),
          ),
          'pinchToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Pinch To Zoom'),
            '#default_value' => $settings['gestureSettingsPen']['pinchToZoom'],
            '#description' => t('Zoom on pinch gesture.'),
          ),
          'flickEnabled' => array(
            '#type' => 'checkbox',
            '#title' => t('Flick Gesture'),
            '#default_value' => $settings['gestureSettingsPen']['flickEnabled'],
            '#description' => t('Enable flick gesture.'),
          ),
          'flickMinSpeed' => array(
            '#type' => 'textfield',
            '#title' => t('Flick Minimum Speed'),
            '#size' => 10,
            '#element_validate' => array('element_validate_number'),
            '#default_value' => $settings['gestureSettingsPen']['flickMinSpeed'],
            '#description' => t('If flickEnabled is true, the minimum speed to initiate a flick gesture (pixels-per-second).'),
          ),
          'flickMomentum' => array(
            '#type' => 'textfield',
            '#title' => t('Flick Momentum'),
            '#size' => 10,
            '#element_validate' => array('element_validate_number'),
            '#default_value' => $settings['gestureSettingsPen']['flickMomentum'],
            '#description' => t('If flickEnabled is true, the momentum factor for the flick gesture.'),
          ),
          'pinchRotate' => array(
            '#type' => 'checkbox',
            '#title' => t('Pinch Rotate'),
            '#default_value' => $settings['gestureSettingsMouse']['pinchRotate'],
            '#description' => t('If pinchRotate is true, the user will have the ability to rotate the image using their fingers.'),
          ),
        ),
        'gestureSettingsUnknown' => array(
          '#type' => 'fieldset',
          '#title' => t('Unknown Pointer Gesture Settings'),
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
          '#description' => t('<p>Settings for gestures generated by a unknown pointer device. (See <a href="https://openseadragon.github.io/docs/OpenSeadragon.html#.GestureSettings">OpenSeadragon.GestureSettings</a>)</p>'),
          'scrollToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Scroll To Zoom'),
            '#default_value' => $settings['gestureSettingsUnknown']['scrollToZoom'],
            '#description' => t('Zoom on scroll gesture.'),
          ),
          'clickToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Click To Zoom'),
            '#default_value' => $settings['gestureSettingsUnknown']['clickToZoom'],
            '#description' => t('Zoom on click gesture.'),
          ),
          'dblClickToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Double Click To Zoom'),
            '#default_value' => $settings['gestureSettingsUnknown']['dblClickToZoom'],
            '#description' => t('Zoom on double-click gesture. Note: If set to true then clickToZoom should be set to false to prevent multiple zooms.'),
          ),
          'pinchToZoom' => array(
            '#type' => 'checkbox',
            '#title' => t('Pinch To Zoom'),
            '#default_value' => $settings['gestureSettingsUnknown']['pinchToZoom'],
            '#description' => t('Zoom on pinch gesture.'),
          ),
          'flickEnabled' => array(
            '#type' => 'checkbox',
            '#title' => t('Flick Gesture'),
            '#default_value' => $settings['gestureSettingsUnknown']['flickEnabled'],
            '#description' => t('Enable flick gesture.'),
          ),
          'flickMinSpeed' => array(
            '#type' => 'textfield',
            '#title' => t('Flick Minimum Speed'),
            '#size' => 10,
            '#element_validate' => array('element_validate_number'),
            '#default_value' => $settings['gestureSettingsUnknown']['flickMinSpeed'],
            '#description' => t('If flickEnabled is true, the minimum speed to initiate a flick gesture (pixels-per-second).'),
          ),
          'flickMomentum' => array(
            '#type' => 'textfield',
            '#title' => t('Flick Momentum'),
            '#size' => 10,
            '#element_validate' => array('element_validate_number'),
            '#default_value' => $settings['gestureSettingsPen']['flickMomentum'],
            '#description' => t('If flickEnabled is true, the momentum factor for the flick gesture.'),
          ),
          'pinchRotate' => array(
            '#type' => 'checkbox',
            '#title' => t('Pinch Rotate'),
            '#default_value' => $settings['gestureSettingsMouse']['pinchRotate'],
            '#description' => t('If pinchRotate is true, the user will have the ability to rotate the image using their fingers.'),
          ),
        ),
        'zoomPerClick' => array(
          '#type' => 'textfield',
          '#title' => t('Zoom per click'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['zoomPerClick'],
          '#description' => t('The "zoom distance" per mouse click or touch tap. Note: Setting this to 1.0 effectively disables the click-to-zoom feature (also see gestureSettings[Mouse|Touch|Pen].clickToZoom/dblClickToZoom).'),
        ),
        'zoomPerScroll' => array(
          '#type' => 'textfield',
          '#title' => t('Zoom per scroll'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['zoomPerScroll'],
          '#description' => t('The "zoom distance" per mouse scroll or touch pinch. Note: Setting this to 1.0 effectively disables the mouse-wheel zoom feature (also see gestureSettings[Mouse|Touch|Pen].scrollToZoom}).'),
        ),
        'zoomPerSecond' => array(
          '#type' => 'textfield',
          '#title' => t('Zoom per second'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['zoomPerSecond'],
          '#description' => t('The number of seconds to animate a single zoom event over.'),
        ),
        'navigatorOptions' => array(
          '#type' => 'fieldset',
          '#title' => t('Navigator options'),
          'showNavigator' => array(
            '#type' => 'checkbox',
            '#title' => t('Show Navigator'),
            '#default_value' => $settings['showNavigator'],
            '#description' => t('Set to true to make the navigator minimap appear.'),
          ),
          'navigatorContainer' => array(
            '#type' => 'container',
            '#states' => array(
              'visible' => array(
                ':input[name="islandora_openseadragon_settings[navigatorOptions][showNavigator]"]' => array('checked' => TRUE),
              ),
            ),
            // We don't provide "navigatorId" as configurable to users.
            'navigatorPosition' => array(
              '#type' => 'select',
              '#title' => t('Navigator Position'),
              '#description' => t('If "ABSOLUTE" is specified, then navigator[Top|Left|Height|Width] determines the size and position of the navigator minimap in the viewer, and navigatorSizeRatio and navigatorMaintainSizeRatio are ignored. For "TOP_LEFT", "TOP_RIGHT", "BOTTOM_LEFT", and "BOTTOM_RIGHT", the navigatorSizeRatio or navigator[Height|Width] values determine the size of the navigator minimap.'),
              '#default_value' => $settings['navigatorPosition'],
              '#options' => drupal_map_assoc(array(
                'TOP_RIGHT',
                'TOP_LEFT',
                'BOTTOM_LEFT',
                'BOTTOM_RIGHT',
                'ABSOLUTE',
              )),
            ),
            'navigatorSizeRatio' => array(
              '#type' => 'textfield',
              '#title' => t('Navigator Size Ratio'),
              '#size' => 10,
              '#element_validate' => array('element_validate_number'),
              '#default_value' => $settings['navigatorSizeRatio'],
              '#description' => t('Ratio of navigator size to viewer size. Ignored if navigator[Height|Width] are specified.'),
            ),
            'navigatorMaintainSizeRatio' => array(
              '#type' => 'checkbox',
              '#title' => t('Navigator Maintain Size Ration'),
              '#default_value' => $settings['navigatorMaintainSizeRatio'],
              '#description' => t('If true, the navigator minimap is resized (using navigatorSizeRatio) when the viewer size changes.'),
            ),
            'navigatorTop' => array(
              '#type' => 'textfield',
              '#title' => t('Navigator Top Position'),
              '#size' => 10,
              '#element_validate' => array('element_validate_number'),
              '#default_value' => $settings['navigatorTop'],
              '#description' => t('Specifies the location of the navigator minimap (see Navigator Position).'),
            ),
            'navigatorLeft' => array(
              '#type' => 'textfield',
              '#title' => t('Navigator Left Position'),
              '#size' => 10,
              '#element_validate' => array('element_validate_number'),
              '#default_value' => $settings['navigatorLeft'],
              '#description' => t('Specifies the location of the navigator minimap (see Navigator Position).'),
            ),
            'navigatorHeight' => array(
              '#type' => 'textfield',
              '#title' => t('Navigator Height'),
              '#size' => 10,
              '#element_validate' => array('element_validate_number'),
              '#default_value' => $settings['navigatorHeight'],
              '#description' => t('Specifies the size of the navigator minimap (see Navigator Position). If specified, Navigator Size Ratio and Navigator Maintain Size Ratio are ignored.'),
            ),
            'navigatorWidth' => array(
              '#type' => 'textfield',
              '#title' => t('Navigator Width'),
              '#size' => 10,
              '#element_validate' => array('element_validate_number'),
              '#default_value' => $settings['navigatorWidth'],
              '#description' => t('Specifies the size of the navigator minimap (see Navigator Position). If specified, Navigator Size Ratio and Navigator Maintain Size Ratio are ignored.'),
            ),
            'navigatorAutoResize' => array(
              '#type' => 'checkbox',
              '#title' => t('Navigator Auto Resize'),
              '#default_value' => $settings['navigatorAutoResize'],
              '#description' => t('Set to false to prevent polling for navigator size changes. Useful for providing custom resize behavior. Setting to false can also improve performance when the navigator is configured to a fixed size.'),
            ),
            'navigatorAutoFade' => array(
              '#type' => 'checkbox',
              '#title' => t('Navigator Auto Fade'),
              '#default_value' => $settings['navigatorAutoFade'],
              '#description' => t('If the user stops interacting with the viewport, fade the navigator minimap. Setting to false will make the navigator minimap always visible.'),
            ),
            'navigatorAutoFade' => array(
              '#type' => 'checkbox',
              '#title' => t('Navigator Auto Fade'),
              '#default_value' => $settings['navigatorAutoFade'],
              '#description' => t('If the user stops interacting with the viewport, fade the navigator minimap. Setting to false will make the navigator minimap always visible.'),
            ),
            'navigatorRotate' => array(
              '#type' => 'checkbox',
              '#title' => t('Navigator Rotate'),
              '#default_value' => $settings['navigatorRotate'],
              '#description' => t('If true, the navigator will be rotated together with the viewer.'),
            ),
          ),
        ),
        'controlsFadeDelay' => array(
          '#type' => 'textfield',
          '#title' => t('Controls Fade Delay'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['controlsFadeDelay'],
          '#description' => t('The number of milliseconds to wait once the user has stopped interacting with the interface before begining to fade the controls. Assumes showNavigationControl and autoHideControls are both true.'),
        ),
        'controlsFadeLength' => array(
          '#type' => 'textfield',
          '#title' => t('Controls Fade Length'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['controlsFadeLength'],
          '#description' => t('The number of milliseconds to animate the controls fading out.'),
        ),
        'controlsFadeDelay' => array(
          '#type' => 'textfield',
          '#title' => t('Controls Fade Delay'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['controlsFadeDelay'],
          '#description' => t('The number of milliseconds to wait once the user has stopped interacting with the interface before begining to fade the controls. Assumes showNavigationControl and autoHideControls are both true.'),
        ),
        'maxImageCacheCount' => array(
          '#type' => 'textfield',
          '#title' => t('Controls Fade Delay'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['maxImageCacheCount'],
          '#description' => t('The max number of images we should keep in memory (per drawer).'),
        ),
        'timeout' => array(
          '#type' => 'textfield',
          '#title' => t('timeout'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['timeout'],
        ),
        'useCanvas' => array(
          '#type' => 'checkbox',
          '#title' => t('Use Canvas'),
          '#default_value' => $settings['useCanvas'],
          '#description' => t('Set to false to not use an HTML canvas element for image rendering even if canvas is supported.'),
        ),
        'minPixelRatio' => array(
          '#type' => 'textfield',
          '#title' => t('Minimum Pixel Ratio'),
          '#size' => 10,
          '#element_validate' => array('element_validate_number'),
          '#default_value' => $settings['minPixelRatio'],
          '#description' => t('The higher the minPixelRatio, the lower the quality of the image that is considered sufficient to stop rendering a given zoom level. For example, if you are targeting mobile devices with less bandwith you may try setting this to 1.5 or higher.'),
        ),
        'mouseNavEnabled' => array(
          '#type' => 'checkbox',
          '#title' => t('Enable Mouse Navigation'),
          '#default_value' => $settings['mouseNavEnabled'],
          '#description' => t('Is the user able to interact with the image via mouse or touch. Default interactions include dragging the image in a plane, and zooming in toward and away from the image.'),
        ),
        'navigationOptions' => array(
          '#type' => 'fieldset',
          '#title' => t('Navigation Controls'),
          'showNavigationControl' => array(
            '#type' => 'checkbox',
            '#title' => t('Show Navigation Control'),
            '#default_value' => $settings['showNavigationControl'],
            '#description' => t('Set to false to prevent the appearance of the default navigation controls. Note that if set to false, the customs buttons set by the options zoomInButton, zoomOutButton etc, are rendered inactive.'),
          ),
          'navigationContainer' => array(
            '#type' => 'container',
            '#states' => array(
              'visible' => array(
                ':input[name="islandora_openseadragon_settings[navigationOptions][showNavigationControl]"]' => array('checked' => TRUE),
              ),
            ),
            'navigationControlAnchor' => array(
              '#type' => 'select',
              '#title' => t('Navigation Control Anchor'),
              '#default_value' => $settings['navigationControlAnchor'],
              '#description' => t('Placement of the default navigation controls. To set the placement of the sequence controls, see the sequenceControlAnchor option.'),
              '#options' => drupal_map_assoc(array(
                'TOP_RIGHT',
                'TOP_LEFT',
                'BOTTOM_LEFT',
                'BOTTOM_RIGHT',
                'ABSOLUTE',
              )),
            ),
            'showZoomControl' => array(
              '#type' => 'checkbox',
              '#title' => t('Show Zoom Control'),
              '#default_value' => $settings['showZoomControl'],
              '#description' => t('If true then + and - buttons to zoom in and out are displayed. Note: OpenSeadragon.Options.showNavigationControl is overriding this setting when set to false.'),
            ),
            'showHomeControl' => array(
              '#type' => 'checkbox',
              '#title' => t('Show Home Control'),
              '#default_value' => $settings['showHomeControl'],
              '#description' => t('documentation'),
            ),
            'showFullPageControl' => array(
              '#type' => 'checkbox',
              '#title' => t('Show Full Page Control'),
              '#default_value' => $settings['showFullPageControl'],
              '#description' => t('If true then the rotate left/right controls will be displayed as part of the standard controls. This is also subject to the browser support for rotate (e.g., viewer.drawer.canRotate()). Note: OpenSeadragon.Options.showNavigationControl is overriding this setting when set to false.'),
            ),
            'showRotationControl' => array(
              '#type' => 'checkbox',
              '#title' => t('Show Rotation Control'),
              '#default_value' => $settings['showRotationControl'],
              '#description' => t('If sequenceMode is true, then provide buttons for navigating forward and backward through the images.'),
            ),
          ),
        ),
        'sequenceControlAnchor' => array(
          '#type' => 'select',
          '#title' => t('Sequence Control Anchor'),
          '#default_value' => $settings['sequenceControlAnchor'],
          '#description' => t('Placement of the default sequence controls.'),
          '#options' => drupal_map_assoc(array(
            'TOP_RIGHT',
            'TOP_LEFT',
            'BOTTOM_LEFT',
            'BOTTOM_RIGHT',
            'ABSOLUTE',
          )),
        ),
        'navPrevNextWrap' => array(
          '#type' => 'checkbox',
          '#title' => t('Navigation Previous/Next Wrap'),
          '#default_value' => $settings['navPrevNextWrap'],
          '#description' => t('If true then the "previous" button will wrap to the last image when viewing the first image and the "next" button will wrap to the first image when viewing the last image.'),
        ),
        // We don't provide "zoomInButton" as configurable to users.
        // We don't provide "zoomOutButton" as configurable to users.
        // We don't provide "homeButton" as configurable to users.
        // We don't provide "fullPageButton" as configurable to users.
        // We don't provide "rotateLeftButton" as configurable to users.
        // We don't provide "rotateRightButton" as configurable to users.
        // We don't provide "previousButton" as configurable to users.
        // We don't provide "nextButton" as configurable to users.
        'sequenceOptions' => array(
          '#type' => 'fieldset',
          '#title' => 'Sequence Mode',
          'sequenceMode' => array(
            '#type' => 'checkbox',
            '#title' => t('Sequence Mode'),
            '#default_value' => $settings['sequenceMode'],
            '#description' => t('Set to true to have the viewer treat your tilesources as a sequence of images to be opened one at a time rather than all at once.'),
          ),
          'sequenceContainer' => array(
            '#type' => 'container',
            '#states' => array(
              'visible' => array(
                ':input[name="islandora_openseadragon_settings[sequenceOptions][sequenceMode]"]' => array('checked' => TRUE),
              ),
            ),
            // We don't provide "initialPage" as configurable to users.
            'preserveViewport' => array(
              '#type' => 'checkbox',
              '#title' => t('Preserve View-port'),
              '#default_value' => $settings['preserveViewport'],
              '#description' => t('If sequenceMode is true, then normally navigating through each image resets the viewport to "home" position. If preserveViewport is set to true, then the viewport position is preserved when navigating between images in the sequence.'),
            ),
            'preserveOverlays' => array(
              '#type' => 'checkbox',
              '#title' => t('Preserve Overlays'),
              '#default_value' => $settings['preserveOverlays'],
              '#description' => t('If sequenceMode is true, then normally navigating through each image resets the overlays. If preserveOverlays is set to true, then the overlays added with OpenSeadragon.Viewer#addOverlay are preserved when navigating between images in the sequence. Note: setting preserveOverlays overrides any overlays specified in the global "overlays" option for the Viewer. It\'s also not compatible with specifying per-tileSource overlays via the options, as those overlays will persist even after the tileSource is closed.'),
            ),
            'showReferenceStrip' => array(
              '#type' => 'checkbox',
              '#title' => t('Show Reference Strip'),
              '#default_value' => $settings['showReferenceStrip'],
              '#description' => t('If sequenceMode is true, then display a scrolling strip of image thumbnails for navigating through the images.'),
            ),
            'referenceStripContainer' => array(
              '#type' => 'container',
              '#states' => array(
                'visible' => array(
                  ':input[name="islandora_openseadragon_settings[sequenceOptions][showReferenceStrip]"]' => array('checked' => TRUE),
                ),
              ),
              'referenceStripScroll' => array(
                '#type' => 'select',
                '#title' => t('Reference Strip Scroll'),
                '#default_value' => $settings['referenceStripScroll'],
                '#description' => t('Display the reference strip horizontally or vertically.'),
                '#options' => drupal_map_assoc(array(
                  'horizontal',
                  'vertical',
                )),
              ),
              // We don't provide "referenceStripElement" as configurable
              // to users.
              'referenceStripHeight' => array(
                '#type' => 'textfield',
                '#title' => t('Reference Strip Height'),
                '#size' => 10,
                '#element_validate' => array('element_validate_number'),
                '#default_value' => $settings['referenceStripHeight'],
                '#description' => t('Height of the reference strip in pixels.'),
              ),
              'referenceStripWidth' => array(
                '#type' => 'textfield',
                '#title' => t('Reference Strip Width'),
                '#size' => 10,
                '#element_validate' => array('element_validate_number'),
                '#default_value' => $settings['referenceStripWidth'],
                '#description' => t('Width of the reference strip in pixels.'),
              ),
              'referenceStripPosition' => array(
                '#type' => 'textfield',
                '#title' => t('Reference Strip Position'),
                '#default_value' => $settings['referenceStripPosition'],
                '#description' => t('The position of the reference strip.'),
                '#options' => drupal_map_assoc(array(
                  'TOP_RIGHT',
                  'TOP_LEFT',
                  'BOTTOM_LEFT',
                  'BOTTOM_RIGHT',
                )),
              ),
              'referenceStripSizeRatio' => array(
                '#type' => 'textfield',
                '#title' => t('Reference Strip Size Ratio'),
                '#size' => 10,
                '#element_validate' => array('element_validate_number'),
                '#default_value' => $settings['referenceStripSizeRatio'],
                '#description' => t('Ratio of reference strip size to viewer size.'),
              ),
            ),
          ),
        ),
        'collectionModeFields' => array(
          '#type' => 'fieldset',
          '#title' => t('Collection Mode'),
          'collectionMode' => array(
            '#type' => 'checkbox',
            '#title' => t('Enable Collection Mode'),
            '#default_value' => $settings['collectionMode'],
            '#description' => t('Set to true to have the viewer arrange your TiledImages in a grid or line.'),
          ),
          'collectionModeContainer' => array(
            '#type' => 'container',
            '#states' => array(
              'visible' => array(
                ':input[name="islandora_openseadragon_settings[collectionModeFields][collectionMode]"]' => array('checked' => TRUE),
              ),
            ),
            'collectionRows' => array(
              '#type' => 'textfield',
              '#title' => t('Collection Rows'),
              '#size' => 10,
              '#element_validate' => array('element_validate_number'),
              '#default_value' => $settings['collectionRows'],
              '#description' => t('If collectionMode is true, specifies how many rows the grid should have. Use 1 to make a line. If collectionLayout is "vertical", specifies how many columns instead.'),
            ),
            'collectionColumns' => array(
              '#type' => 'textfield',
              '#title' => t('Collection Columns'),
              '#size' => 10,
              '#element_validate' => array('element_validate_number'),
              '#default_value' => $settings['collectionColumns'],
              '#description' => t('If collectionMode is true, specifies how many columns the grid should have. Use 1 to make a line. If collectionLayout is "vertical", specifies how many rows instead. Ignored if collectionRows is not set to a falsy value.'),
            ),
            'collectionLayout' => array(
              '#type' => 'select',
              '#title' => t('Collection Layout'),
              '#default_value' => $settings['collectionLayout'],
              '#description' => t('If collectionMode is true, specifies whether to arrange vertically or horizontally.'),
              '#options' => drupal_map_assoc(array(
                'horizontal',
                'vertical',
              )),
            ),
            'collectionTileSize' => array(
              '#type' => 'textfield',
              '#title' => t('Collection Tile Size'),
              '#size' => 10,
              '#default_value' => $settings['collectionTileSize'],
              '#description' => t('If collectionMode is true, specifies the size, in viewport coordinates, for each TiledImage to fit into. The TiledImage will be centered within a square of the specified size.'),
            ),
            'collectionTileMargin' => array(
              '#type' => 'textfield',
              '#title' => t('Collection Tile Margin'),
              '#size' => 10,
              '#element_validate' => array('element_validate_number'),
              '#default_value' => $settings['collectionTileMargin'],
              '#description' => t('If collectionMode is true, specifies the margin, in viewport coordinates, between each TiledImage.'),
            ),
          ),
        ),
        // We don't provide "crossOriginPolicy" as configurable to users.
        // We don't provide "ajaxWithCredentials" as configurable to users.
      ),
    ),
    'actions' => array(
      '#type' => 'actions',
      'reset' => array(
        '#type' => 'submit',
        '#value' => t('Reset to defaults'),
        '#weight' => 1,
        '#submit' => array('islandora_openseadragon_admin_submit_reset'),
      ),
    ),
    '#submit' => array('islandora_openseadragon_admin_submit_normalize'),
  );
  return system_settings_form($form);
}

/**
 * Get the element to render for the AJAX event that triggered this callback.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The form element to render.
 */
function islandora_openseadragon_admin_ajax_djatoka_url(array $form, array $form_state) {
  return $form['djatoka']['islandora_openseadragon_djatoka_url'];
}

/**
 * Gets a message which describes if Adore-Djatoka is accessible.
 *
 * @param string $path
 *   The path to the Adore Djatoka image resolver.
 *
 * @return string
 *   A message describing the accessibility of the Adore-Djatoka image resolver.
 */
function islandora_openseadragon_admin_form_djatoka_access_message($path) {
  $url = url("{$path}", array(
    'absolute' => TRUE,
    'query' => array(
      'url_ver' => 'Z39.88-2004',
      'rft_id' => 'http://memory.loc.gov/gmd/gmd433/g4330/g4330/np000066.jp2',
      'svc_id' => 'info:lanl-repo/svc/getRegion',
      'svc_val_fmt' => 'info:ofi/fmt:kev:mtx:jpeg2000',
      'svc.format' => 'image/jpeg',
      'svc.level' => '1',
    ),
  ));
  $result = drupal_http_request($url);
  if ($result->code == 200) {
    $confirmation_message = theme_image(array('path' => 'misc/watchdog-ok.png', 'attributes' => array()));
    $confirmation_message .= t('Successfully connected to Adore Djatoka Server.');
  }
  else {
    $confirmation_message = theme_image(array('path' => 'misc/watchdog-error.png', 'attributes' => array()));
    $confirmation_message .= t('Unable to connect to Adore Djatoka Server at !path', array(
      '!path' => $path,
    ));
  }
  return $confirmation_message;
}

/**
 * Validates that the supplied djatoka base URL is a working djatoka install.
 *
 * @param array $element
 *   The element to validate.
 * @param array $form_state
 *   Drupal form state.
 * @param array $form
 *   Drupal form.
 */
function islandora_openseadragon_djatoka_url_validate(array $element, array &$form_state, array $form) {
  $path = $element['#value'];
  if (empty($path)) {
    form_error($element, t('This field is required.'));
  }
  else {
    $url = url("{$path}", array(
      'absolute' => TRUE,
      'query' => array(
        'url_ver' => 'Z39.88-2004',
        'rft_id' => 'http://memory.loc.gov/gmd/gmd433/g4330/g4330/np000066.jp2',
        'svc_id' => 'info:lanl-repo/svc/getRegion',
        'svc_val_fmt' => 'info:ofi/fmt:kev:mtx:jpeg2000',
        'svc.format' => 'image/jpeg',
        'svc.level' => '1',
      ),
    ));
    $result = drupal_http_request($url);
    if ($result->code != 200) {
      form_error($element, t('This does not seem to be a functioning Djatoka server.'));
    }
  }
}

/**
 * Validate the form.
 *
 * @param array $form
 *   Drupal form.
 * @param array $form_state
 *   Drupal form state.
 */
function islandora_openseadragon_admin_validate(array $form, array &$form_state) {
  if ($form_state['values']['islandora_openseadragon_tilesource'] == 'djatoka') {
    $element = $form['djatoka']['islandora_openseadragon_djatoka_url'];
    islandora_openseadragon_djatoka_url_validate($element, $form_state, $form);
  }
}

/**
 * Handles a submit from the admin pane.
 *
 * @param array $form
 *   Drupal form.
 * @param array $form_state
 *   Drupal form state.
 */
function islandora_openseadragon_admin_submit_reset(array $form, array &$form_state) {
  $op = $form_state['clicked_button']['#id'];
  switch ($op) {
    case 'edit-reset':
      variable_del('islandora_openseadragon_settings');
      variable_del('islandora_openseadragon_tile_size');
      variable_del('islandora_openseadragon_tile_overlap');
      break;
  }
}

/**
 * Normalizes the settings so they can be used by Openseadragon.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function islandora_openseadragon_admin_submit_normalize(array $form, array &$form_state) {
  module_load_include('inc', 'islandora_openseadragon', 'includes/utilities');
  _islandora_openseadragon_normalize_settings($form_state['values']['islandora_openseadragon_settings']);
  // Get default to match array formatting.
  $default_settings = islandora_openseadragon_default_options();
  _islandora_openseadragon_filter_settings($form_state['values']['islandora_openseadragon_settings'], $default_settings);
}

/**
 * Casts the settings to appropriate types so they work in javascript.
 *
 * @param array $settings
 *   The Openseadragon settings to be normalized.
 *
 * @return array
 *   Normalized settings.
 */
function _islandora_openseadragon_normalize_settings(array &$settings) {
  foreach ($settings as $key => $value) {
    $settings[$key] = _islandora_openseadragon_normalize_setting($value);
  }
  return $settings;
}

/**
 * Normalizes the given setting.
 *
 * @param mixed $value
 *   The setting to be normalized.
 *
 * @return array|float|int|string
 *   The normalized setting.
 */
function _islandora_openseadragon_normalize_setting($value) {
  if (is_array($value)) {
    return _islandora_openseadragon_normalize_settings($value);
  }
  elseif (filter_var($value, FILTER_VALIDATE_INT) !== FALSE) {
    return (int) $value;
  }
  elseif (filter_var($value, FILTER_VALIDATE_FLOAT) !== FALSE) {
    return (float) $value;
  }
  elseif (filter_var($value, FILTER_VALIDATE_URL) !== FALSE) {
    return check_plain($value);
  }
  return $value;
}

/**
 * Casts the settings to appropriate types so they work in javascript.
 *
 * @param array $settings
 *   The Openseadragon settings to be normalized.
 *
 * @return array
 *   Normalized settings.
 */
function _islandora_openseadragon_filter_settings(array &$settings, array $default_settings) {
  foreach ($settings as $key => $value) {
    // We don't want the nested containers in the settings
    // so we strip them out.
    if (is_array($value) && !isset($default_settings[$key])) {
      $tmp = _islandora_openseadragon_filter_settings($value, $default_settings);
      $settings = array_merge($settings, $tmp);
      unset($settings[$key]);
    }
    elseif (is_array($value)) {
      $settings[$key] = _islandora_openseadragon_filter_settings($value, $default_settings[$key]);
    }
    if (is_string($value) && empty($value)) {
      unset($settings[$key]);
    }
    elseif (is_null($value)) {
      unset($settings[$key]);
    }
  }
  return $settings;
}
