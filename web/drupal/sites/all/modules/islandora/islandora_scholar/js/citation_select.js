/**
 * @file
 * Lets the select ajax in a new citation.
 */

(function ($) {
  Drupal.behaviors.islandora_scholar_citation_ajax = {
    attach: function(context, settings) {
      $(".islandora-scholar-citation-select").once("islandora-scholar-change-csl", function() {
        $(this).change(function() {
          var last_value = this.value;
          $.ajax({
            url: Drupal.settings.basePath + "islandora/object/" +
              settings.islandora_scholar.pid + "/islandora_scholar_citation/",
            cache: true,
            data: {"citation_style": this.value},
            context: this,
            success: function(citation_info) {
              // Only update the citation if this is the style selected.
              if (this.value == last_value) {
                $(".citation").html(citation_info.citation);
              }
            }
          });
        });
      });
    }
  }
})(jQuery);
