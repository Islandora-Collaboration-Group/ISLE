<?php

/**
 * @file
 * Template for generating the bookmark list column view.
 *
 * Available variables:
 * - $name: The name of this bookmark list.
 * - $user: The name of the user who created the list.
 * - $description: The description of the bookmark list.
 * - $link: A link to the bookmark list page.
 */
?>

<div>
  <h1><?php print $name; ?></h1>
  <hr/>
  <h3><?php print t("By"); ?>: <?php print $user; ?></h3>
  <?php if ($description): ?>
    <h3><?php print t("Description"); ?>: </h3>
    <p><?php print $description; ?></p>
    <hr/>
  <?php endif; ?>
  <label for="list_url"><?php print t('URL'); ?></label>
  <input id="list_url_link" name="list_url" value="<?php print $link; ?>">
  <br>
</div>
