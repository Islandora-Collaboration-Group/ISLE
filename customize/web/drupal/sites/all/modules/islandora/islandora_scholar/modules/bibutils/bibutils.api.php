<?php

/**
 * @file
 * This file documents all available hook functions to manipulate data.
 */

/**
 * Preprocess any input to the bibutils convert.
 */
function hook_preprocess_bibutils_convert($src, $src_format, $dest_format) {
}

/**
 * Postprocess any output from the bibutils convert.
 *
 * Post process must return TRUE or FALSE on success or failure, it determines whats returned by the * Bibutils::Convert function().
 */
function hook_postprocess_bibutils_convert($src, $src_format, $dest, $dest_format) {
}
