/**
 * @file
 * Islandora Newspaper scripts file.
 */

(function ($) {
  // Select page
  Drupal.behaviors.islandoraNewspaperSelectPage = {
    attach: function(context, settings) {
      if (!$(".page-select").hasClass('processed')) {
        $(".page-select").change(function(e) {
          var pid = $("option:selected", this).attr('value');
          window.location = Drupal.settings.basePath + 'islandora/object/' + pid; // check plain?
        });
        $(".page-select").addClass('processed');
      }
    }
  };
})(jQuery);
