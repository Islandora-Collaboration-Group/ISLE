<video id="islandora_videojs" class="video-js vjs-default-skin" controls
 preload="auto" width="640" height="264" 
  <?php if (isset($tn)): ?>
    poster="<?php print $tn; ?>"
  <?php endif; ?>
 data-setup="{}">
  <?php foreach ($sources as $source): ?>
    <source src="<?php print $source['url']; ?>" type='<?php print $source['mime']; ?>'>
  <?php endforeach; ?>
</video>
<?php if (empty($sources)): ?>
<div id="video-js-warning">
No video sources available.
</div>
<?php endif; ?>
