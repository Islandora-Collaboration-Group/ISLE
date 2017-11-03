<?php
/**
 * @file
 * Template file for the PREMIS fixity check message that is added to
 * the Islandora object's audit log for each datastream.
 * 
 * Available variables:
 *   $ds_location string
 *     The value of 'dsLocation' as returned by getDatastream().
 *   $ds_checksum_type string
 *     The value of 'dsChecksumType' as returned by getDatastream();
 *   $outcome string
 *     One of 'valid', 'invalid', or 'disabled'.
 *     
 */
  switch ($outcome) {
    case 'valid':
      $outcome_message = $ds_checksum_type . ' checksum validated.';
      break;
    case 'invalid':
      $outcome_message = 'Invalid ' . $ds_checksum_type . ' detected.';
      break;
    case 'disabled':
      $outcome_message = 'Checksums not enabled.';
      break;
  }
?>
PREMIS:file=<?php print $ds_location; ?>; PREMIS:eventType=fixity check; PREMIS:eventOutcome=<?php print $outcome_message; ?>  
