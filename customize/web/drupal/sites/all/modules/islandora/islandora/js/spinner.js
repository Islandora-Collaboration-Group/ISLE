/*jshint browser: true, devel:true*/
/*global jQuery, Drupal, Spinner*/
/**
 * @file
 * Triggers the display of a spinning icon when the form is submitted.
 */
(function ($) {
  'use strict';
  Drupal.behaviors.spinner = {
    attach: function(context, settings) {
      // Store what triggered the submit.
      $('form').once('submit-resolver', function() {
        $(this).click(function(event) {
          $(this).data('clicked', $(event.target));
        });
        $(this).keypress(function(event) {
          // On enter the first submit button is assumed as is most often the
          // case and this is part of the HTML 5 specification, although some
          // Browsers may choose the button with the lowest tab-index.
          if (event.which === 13) {
            $(this).data('clicked', $(':submit', this).first());
          }
        });
      });
      $.each(settings.spinner, function (base, value) {
        var id = '#' + base,
          message = $('<div/>').text(settings.spinner[base].message);
        if (id !== '#edit-hidden-next') {
          $(id, context).once('spinner', function () {
            var spinner = new Spinner(settings.spinner[base].opts);
            $(id).parents('form').submit(function (event) {
              // If some other widget is preventing form submission we should
              // not attempt to submit at this time.
              if (event.isDefaultPrevented()) {
                return;
              }
              if ($(this).data('clicked').is(id) && $(this).data('submitted') === undefined) {
                event.preventDefault();
                // Prevent this from being entered a second time.
                $(this).data('submitted', true);
                // Add Message.
                $(id).after(message);
                // Make UI changes.
                spinner.spin(this);
                $('#edit-next').hide();
                $('#edit-prev').hide();
                $('.islandora-spinner-submit').hide();
                // Submit the form after a set timeout, this handles problems with
                // safari, in that safari submits immediately..
                if (navigator.userAgent.indexOf('Safari') !== -1 && navigator.userAgent.indexOf('Chrome') === -1) {
                  $(':submit').attr('disabled', 'disabled');
                }
                setTimeout(function () {
                  // Allow for the button to be clicked, then click it then
                  // prevent the default behaviour.
                  $(id).removeAttr('disabled')
                    .click()
                    .click(function (event) {
                      event.preventDefault();
                    });
                }, 500);
              }
              return true;
            });
          });
        }
      });
    }
  };
})(jQuery);
