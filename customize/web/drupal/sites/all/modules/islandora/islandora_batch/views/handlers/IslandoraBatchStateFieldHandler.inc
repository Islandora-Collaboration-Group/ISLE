<?php
/**
 * @file
 * Class implementation for state field.
 */

class IslandoraBatchStateFieldHandler extends views_handler_field {
  /**
   * Inherits.
   */
  //@codingStandardsIgnoreStart
  // XXX: Views code uses underscores... Shrug?
  function add_self_tokens(&$tokens, $item) {
    $map = array(
      ISLANDORA_BATCH_STATE__ERROR => t('Error'),
      ISLANDORA_BATCH_STATE__NOT_READY => t('Not ready to ingest'),
      ISLANDORA_BATCH_STATE__READY => t('Ready to ingest'),
      ISLANDORA_BATCH_STATE__PENDING_CHILDREN => t('Not ready to ingest; children pending'),
      ISLANDORA_BATCH_STATE__DONE => t('Ingested'),
    );
    if (isset($tokens['[state]'])) {
      $tokens['[state-text]'] = $map[$tokens['[state]']];
    }
  }

  /**
   * Inherits.
   */
  function document_self_tokens(&$tokens) {
    $tokens['[state-text]'] = t('A human-readable string representing the state.');
  }
  //@codingStandardsIgnoreEnd
}
