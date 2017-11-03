/**
 * Created by whikloj on 2017-01-27.
 */

(function(jQuery) {
    Drupal.ajax.prototype.commands.islandoraAltmetricsRenderBadge = function() {
        _altmetric_embed_init();
    };
}(jQuery));