(function($) {
  Drupal.behaviors.islandora_paged_content = {
    attach: function(context, settings) {
      $("a.preview").click(function() {
        $('div.ref_image').dialog(({position: { my: "left top", at: "top", collision:'none', of:'#edit-ocr' }, width: Drupal.settings.width}));
      });
    }
  };
})(jQuery);
