/*jslint browser: true*/
/*global jQuery, Drupal*/
/**
 * @file
 * Defines the Islandora Image Annotation buttons and their Drupal behaviour.
 */
(function ($) {
  'use strict';

  /**
   * Causes the 'Annotate' button to display the Create Annotation Dialog.
   *
   * @type {{attach: attach}}
   */
  Drupal.behaviors.islandoraImageAnnotationCreateAnnotation = {
    attach: function (context) {
      var base = '#islandora-image-annotation-create-annotation-button';
      $(base, context).once('islandoraCreateAnnotation', function () {
        $(this).click(function () {
          Drupal.IslandoraImageAnnotationDialog.getInstance().show();
        });
      });
    }
  };

  /**
   * Toggle the 'Full Window' display of the Image Annotation Widget.
   *
   * @type {{attach: attach}}
   */
  Drupal.behaviors.islandoraImageAnnotationFullWindow = {
    attach: function (context) {
      var base = '#islandora-image-annotation-full-window-button';
      $(base, context).once('islandoraToggleFullWindow', function () {
        var $imageAnnotation = $('#islandora-image-annotation'),
          $fullWindowButton = $(base),
          messageId = 'islandora-exit-full-window-message',
          fullWindowClass = 'islandora-image-annotation-full-window';

        /**
         * Removes the message that ESC can be used to exit full window mode.
         */
        function removeExitFullWindowMessage() {
          var $message = $('#' + messageId);
          if ($message.length > 0) {
            $message.remove();
          }
        }

        /**
         * Displays a message that ESC can be used to exit full window mode.
         */
        function displayExitFullWindowMessage() {
          var $message;

          // Prevent from rendering twice.
          removeExitFullWindowMessage();

          // Add a message to notify user of the 'ESC' button.
          $message = $('<div />', {
            id: messageId
          }).append(
            $('<div />').html(Drupal.t('Press ESC to exit full screen')).hide()
          );
          $('.islandora-image-annotation-right-column').prepend($message);
          $('div:first-child', $message).fadeIn(400, function () {
            var $child = $(this);
            setTimeout(function () {
              $child.fadeOut(400, function () {
                $message.remove();
              });
            }, 3000);
          });
        }

        /**
         * Toggle the Full Window Display.
         */
        function toggleFullWindowDisplay() {
          // Use the class to toggle.
          $imageAnnotation.toggleClass(fullWindowClass);

          // Resize the Canvas.
          Drupal.IslandoraImageAnnotationCanvas.getInstance().resizeCanvasContainer();

          if ($imageAnnotation.hasClass(fullWindowClass)) {
            $fullWindowButton.html(Drupal.t('Exit Full Window'));
            // Don't display over top of the Admin Menu.
            $imageAnnotation.css('top', $('#admin-menu-wrapper').height());
            displayExitFullWindowMessage();
          } else {
            $fullWindowButton.html(Drupal.t('Full Window'));
            removeExitFullWindowMessage();
          }
        }

        // Toggle 'Full Window' when clicked.
        $fullWindowButton.click(function () {
          toggleFullWindowDisplay();
        });

        // Exit 'Full Window' when ESC is pressed.
        $(document).keyup(function (event) {
          // Only preform the 'ESC' functionality if in full window mode.
          if (event.keyCode === 27 && $imageAnnotation.hasClass(fullWindowClass)) {
            toggleFullWindowDisplay();
          }
        });
      });
    }
  };


  /**
   * Toggle the display of all Annotations.
   *
   * @type {{attach: attach}}
   */
  Drupal.behaviors.islandoraImageAnnotationToggleAnnotationDisplay = {
    attach: function (context) {
      var base = '#islandora-image-annotation-toggle-annotation-display-button';
      $(base, context).once('islandoraImageAnnotationToggleAnnotationDisplay', function () {
        var that = this;
        // Conditionally change the label of the button depending on if any
        // annotations are being displayed or not.
        Drupal.IslandoraImageAnnotation.on('showAnnotation', function (event, annotation) {
          $(that).text(Drupal.t('Hide Annotation(s)'));
        });

        Drupal.IslandoraImageAnnotation.on('hideAnnotation', function (event, annotation) {
          var list = Drupal.IslandoraImageAnnotationList.getInstance();
          if (!list.isAnyAnnotationDisplayed()) {
            $(that).text(Drupal.t('Show Annotation(s)'));
          }
        });

        // When clicked toggle the display of annotations depending on if any
        // are showing.
        $(this).click(function () {
          var list = Drupal.IslandoraImageAnnotationList.getInstance();
          if (list.isAnyAnnotationDisplayed()) {
            list.hideAllAnnotations();
          }
          else {
            list.showAllAnnotations();
          }
        });
      });
    }
  };
}(jQuery));
