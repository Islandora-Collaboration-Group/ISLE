<?php
/**
 * @file
 * This file documents hook functions for the Islandora Checksum Checker module.
 */

/**
 * Notify modules of the outcome of the checksum validation.
 *
 * @param string $pid
 *   The current object's PID.
 * @param string $dsid
 *   The current datastream's DSID. Will be empty if the exception is thrown
 *   from the initial Tuque connection (i.e., there is no current datastream).
 * @param array $ds_info
 *   Output of Tuque's API-M getDatastream() method, with 'validateChecksum'
 *   set to TRUE. See islandora_checksum_checker_validate_checksum() for
 *   details on the content of this array. Will be empty if an exception was
 *   caught.
 * @param string $outcome
 *   One of 'valid', 'invalid', or 'disabled'. Will be empty if an exception
 *   was caught.
 * @param array $exceptions
 *   An array of all exception objects caught during the validation. Exceptions
 *   will already be logged to the watchdog.
 */
function hook_islandora_checksum_checker_outcome($pid, $dsid, $ds_info, $outcome, $exceptions) {
  if ($outcome == 'disabled') {
    watchdog('my_module', 'You should enable checksums for !pid !dsid.',
      array(
        '!pid' => $pid,
        '!dsid' => $dsid,
      ),
    WATCHDOG_WARNING);
  }
}
