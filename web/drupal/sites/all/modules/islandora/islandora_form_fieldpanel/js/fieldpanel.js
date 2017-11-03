/**
 * @file
 * Provides capability to move field panel element up/down.
 */

(function ($) {

  "use strict";

  Drupal.behaviors.islandora_form_fieldpanel = {
    attach: function (context) {
      $('.ui-fieldpane-move-down-button').css('display', 'inline');
      $('.ui-fieldpane-move-up-button').css('display', 'inline');
      $('.fieldpanel-swap-fieldset').hide();
      $('.ui-field');
      $('.ui-fieldpane-move-down-button .form-submit').click(function () {
        $('.fieldpanel-add').click();
        var position = $(this).data('position');
        var parent = $(this).parent().parent().parent();
        var swap = $(parent).children('.fieldpanel-swap-fieldset');

        var check = false;
        var second_check = false;
        var element = 0;
        var counter = 0;
        $(swap).find('.fieldpanel-swap-fieldset-move-element option').each(function () {
          if (check === true) {
            element = $(this).val();
            check = false;
            second_check = true;
          }
          else if (counter === position && second_check === false) {
            position = $(this).val();
            check = true;
          }
          counter++;
        });
        $(swap).find('.fieldpanel-swap-fieldset-move-element').val("" + element).change();
        $(swap).find('.fieldpanel-swap-fieldset-move-position').val("" + position).change();
        $(swap).find('.form-submit').trigger('mousedown');
        return false;
      });
      $('.ui-fieldpane-move-up-button .form-submit').click(function () {
        $('.fieldpanel-add').click();
        var position = $(this).data('position');
        var parent = $(this).parent().parent().parent();
        var swap = $(parent).children('.fieldpanel-swap-fieldset');

        var check = false;
        var second_check = false;
        var element = 0;
        var counter = 0;
        $(swap).find('.fieldpanel-swap-fieldset-move-element option').each(function () {
          if (counter !== position && check === false) {
            element = $(this).val();
          }
          else if (counter === position && second_check === false) {
            position = $(this).val();
            second_check = true;
            check = true;
          }
          counter++;
        });
        $(swap).find('.fieldpanel-swap-fieldset-move-element').val("" + position).change();
        $(swap).find('.fieldpanel-swap-fieldset-move-position').val("" + element).change();
        $(swap).find('.form-submit').trigger('mousedown');
        return false;
      });
    }
  };
})(jQuery);
