jQuery(document).ready(function() {
  videojs("islandora_videojs").setup({
    file: Drupal.settings.islandora_videojs.url,
    image: Drupal.settings.islandora_videojs.thumbnail
  });
});
