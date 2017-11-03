/**
 * @file
 * Add JAIL for fancy loading of images.
 */

(function ($) {
    Drupal.behaviors.islandora_compound_object_JAIL = {
        attach: function(context, settings) {
            $('img.islandora-compound-object-jail').jail({
                triggerElement:'#block-islandora-compound-object-compound-jail-display',
                event: 'scroll',
                error: function($img, options) {
                    if( $img.attr("src") == Drupal.settings.islandora_compound_object.image_path ) {
                        return;
                    }

                    $img.attr("data-src", Drupal.settings.islandora_compound_object.image_path);
                    $img.trigger('scroll');
                }
            });
        }
    };
})(jQuery.noConflict(true));
