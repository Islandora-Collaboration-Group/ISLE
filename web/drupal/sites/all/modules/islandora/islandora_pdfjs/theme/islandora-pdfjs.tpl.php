<?php

/**
 * @file
 *
 * Theme template file for the islandora pdf.js Reader.
 */

?>
<div id="islandora-pdfjs">
  <iframe class="pdf" webkitallowfullscreen mozallowfullscreen allowfullscreen frameborder="no" width="99%" height="800px" src="<?php print $viewer_url; ?>"><?php print $file_url; ?></iframe>
</div>
