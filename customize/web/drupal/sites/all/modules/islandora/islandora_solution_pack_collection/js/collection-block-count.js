(function ($) {
  Drupal.behaviors.islandora_basic_collection_count_block = {
    attach: function (context, settings) {
      $('span#' + settings.islandora_basic_collection.count_block.id, context).once('islandora_basic_collection_count_block', function() {
        $.ajax({
          url: settings.islandora_basic_collection.count_block.callback,
          success: function (data, textStatus, jqXHR) {
            $(this).html(data);
          },
          context: this
        });
      });
    }
  }
})(jQuery);
