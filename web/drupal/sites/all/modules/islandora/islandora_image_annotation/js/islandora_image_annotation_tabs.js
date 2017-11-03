/**
 * @file
 * Defines the Islandora Image Annotation widget and it's Drupal behaviour.
 *
 * Requires jQuery UI.
 */
(function ($) {
  'use strict';

  /**
   * Initialize the Tabs section of the Image annotation widget.
   *
   * We can only have one Tabs per page.
   */
  Drupal.behaviors.islandoraImageAnnotationTabs = {
    attach: function (context, settings) {
      var base = '#islandora-image-annotation-tabs';
      $(base, context).once('islandoraImageAnnotationTabs', function () {
        $(this).tabs();
      });
    }
  };

}(jQuery));
