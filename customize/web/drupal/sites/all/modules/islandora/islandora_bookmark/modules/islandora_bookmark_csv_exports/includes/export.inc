<?php
/**
 * @file
 * Class used for constructing CSVs for export.
 */

class IslandoraBookmarkCSVExportsExport {
  protected $configurationId;
  public $headers;
  public $lines;
  public $style = FALSE;

  /**
   * Constructor.
   */
  public function __construct($config_id) {
    $this->configurationId = $config_id;
  }

  /**
   * Exports as a CSV.
   *
   * @param array $pids
   *   An array of pids to be exported.
   * @param bool|string $style
   *   The style/value of the dependent dropdown from Bookmark.
   */
  public function export($pids, $style = FALSE) {
    // @TODO: Batch this?
    if ($style) {
      $this->style = $style;
    }
    module_load_include('inc', 'islandora_bookmark_csv_exports', 'includes/db');
    $config = islandora_bookmark_csv_exports_get_configuration_by_id($this->configurationId);
    $solr_fields = drupal_map_assoc(unserialize($config['config_fields']));

    if (!empty($solr_fields)) {
      $headers = array();
      $csv_values = array();
      $scrub_csv = function($value) use($config) {
        if (!is_array($value)) {
          $value = (array) $value;
        }
        // Trim off any extra values.
        $value = array_filter(array_map('trim', $value));
        $value = implode($config['delimiter'], $value);
        return $value;
      };
      $added_pid = FALSE;
      if (!isset($solr_fields['PID'])) {
        $solr_fields['PID'] = 'PID';
        $added_pid = TRUE;
      }
      foreach ($pids as $pid) {
        $query_processor = new IslandoraSolrQueryProcessor();
        $query_processor->solrQuery = 'PID:"' . $pid . '"';
        // Solr needs the PID field to be here to avoid warnings.
        $query_processor->solrParams['fl'] = implode(', ', $solr_fields);
        $query_processor->executeQuery(FALSE);
        if ($query_processor->islandoraSolrResult['response']['numFound'] > 0) {
          $solr_results_doc = $query_processor->islandoraSolrResult['response']['objects']['0']['solr_doc'];
          // If we added the PID to appease Solr remove it now.
          if ($added_pid) {
            unset($solr_results_doc['PID']);
          }
          $solr_results_doc = array_filter(array_map($scrub_csv, $solr_results_doc));
          // Ensure we only provide results for what is specified.
          $csv_line = array_intersect_key(drupal_map_assoc($headers), $solr_results_doc);
          $diff = array_diff($solr_results_doc, drupal_map_assoc($headers));
          $csv_line = array_merge($csv_line, $diff);
          $headers = array_merge(drupal_map_assoc($headers), drupal_map_assoc(array_keys($diff)));
          $csv_values[$pid] = $csv_line;
        }
      }
      drupal_add_http_header('Content-Type', 'text/csv');
      drupal_add_http_header('Content-Disposition', "attachment;filename=islandora_bookmark_csv_exports.csv");
      $fp = fopen('php://output', 'w');
      // Allow modules to alter before we write.
      $this->headers = $headers;
      $this->lines = $csv_values;
      drupal_alter('islandora_bookmark_csv_exports_lines', $this);
      fputcsv($fp, $this->headers);
      foreach ($this->lines as $line) {
        fputcsv($fp, $line);
      }
      fclose($fp);
      drupal_exit();
    }
  }
}
