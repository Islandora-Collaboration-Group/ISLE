<?php

/**
 * @file
 * Defines a class that helps in the construction of COINS url's.
 */

/**
 * Class to parse relevant data from MODS, initially used to
 * construct the COINS url but has found use elsewhere.
 */
class Coins {

  public $pid = NULL;
  public $mods = NULL;
  public $type = NULL;

  public $issn;
  public $volume;
  public $issue;
  public $date;
  public $startPage;
  public $endPage;
  public $title;
  public $subTitle;

  public $journalTitle;
  public $abstract;
  public $authors;
  public $contributors;

  public $topic;
  public $note;
  public $embargoDate;
  public $genre;

  public $username;
  public $department;

  /**
   * Create the object properties from the MODS datastream.
   *
   * @param AbstractObject $object
   *   The object that you want to extract the MODS data from
   */
  public function __construct(AbstractObject $object) {
    $this->pid = $object->id;
    $xmlstr = $object['MODS']->content;

    if ($xmlstr == NULL || strlen($xmlstr) < 5) {
      drupal_set_message(t('No MODS datastream found!'), 'warning');
      return " ";
    }
    $xml = new SimpleXMLElement($xmlstr);
    $xml->registerXPathNamespace('mods', 'http://www.loc.gov/mods/v3');

    $this->mods = $xml;

    $authors = array();
    $contributors = array();

    foreach ($xml->name as $name_xml) {
      $name_parts = array();
      $role = (string) @$name_xml->role->roleTerm;
      if ($role) {
        if ($role == 'author' || $role == 'creator' || $role = 'contributor') {
          foreach ($name_xml->namePart as $name_part) {
            if ((string) $name_part != '') {
              if ($role == 'contributor') {
                $name_parts['contributor'] = TRUE;
              }
              else {
                $name_parts['contributor'] = FALSE;
              }
              // Strip periods off the end.
              $np = (string) rtrim($name_part, '.');
              if ($name_part['type'] == 'given') {
                $name_parts['initial'] = (string) substr($np, 0, 1) . '. ';
                $name_parts['first_name'] = (string) $name_part;
              }
              if ($name_part['type'] == 'family') {
                $name_parts['last_name'] = $np;
              }
            }
          }
          if (!empty($name_parts) && isset($name_parts['initial']) && isset($name_parts['last_name'])) {
            if ($name_parts['contributor'] == FALSE) {
              $authors[] = $name_parts['initial'] . $name_parts['last_name'];
            }
            else {
              $contributors[] = $name_parts['initial'] . $name_parts['last_name'];
            }
          }
        }
      }
    }
    $this->contributors = $contributors;
    $this->authors = $authors;

    $issn = $xml->xpath('//mods:identifier[@type="issn"]');
    if (isset($issn[0])) {
      $this->issn = str_replace(array("&", "=", ",", ";"), array('', '', '', ''), (string) $issn[0]);
    }

    $origin_date = $xml->xpath('//mods:originInfo/mods:dateIssued');
    $part_date = $xml->xpath('//mods:part/mods:date');
    $related_date = $xml->xpath('//mods:relatedItem[@type="host"]//mods:date');
    $created_date = $xml->xpath('//mods:originInfo/mods:dateCreated');

    if ($origin_date) {
      $date = (string) reset($origin_date);
    }
    elseif ($part_date) {
      $date = (string) reset($part_date);
    }
    elseif ($related_date) {
      $date = (string) reset($related_date);
    }
    elseif ($created_date) {
      $date = (string) reset($created_date);
    }

    if (isset($date) && $date) {
      $this->date = $date;
    }

    $volume = $xml->xpath('//mods:part/mods:detail[@type="volume"]/mods:number');
    if ($volume) {
      $this->volume = $volume[0];
    }

    $issue = $xml->xpath('//mods:part/mods:detail[@type="issue"]/mods:number');
    if ($issue) {
      $this->issue = $issue[0];
    }

    $start_page = $xml->xpath('//mods:extent[@unit="page"]/mods:start');
    if ($start_page) {
      $this->startPage = $start_page[0];
    }

    $end_page = $xml->xpath('//mods:extent[@unit="page"]/mods:end');
    if ($end_page) {
      $this->endPage = (string) reset($end_page);
    }

    $title = $xml->xpath('//mods:mods[1]/mods:titleInfo/mods:title');
    if ($title) {
      $this->title = $title[0];
    }

    $sub_title = $xml->xpath('//mods:subTitle');
    if ($sub_title) {
      $this->subTitle = $sub_title[0];
    }

    $journal_title = $xml->xpath('//mods:relatedItem[@type="host"]//mods:title');
    if ($journal_title) {
      $this->journalTitle = $journal_title[0];
    }

    $abstract = $xml->xpath('//mods:abstract');
    if ($abstract) {
      $this->abstract = $abstract[0];
    }

    $topic = array();
    foreach ($xml->subject as $subject) {
      $topic[] = (string) $subject->topic;
    }
    $this->topic = implode(', ', array_filter(array_map('trim', $topic)));

    $notes = array();
    foreach ($xml->note as $note) {
      $notes[] = (string) $note;
    }
    $this->note = implode(', ', array_filter(array_map('trim', $notes)));

    $embargo_date = $xml->xpath('//mods:dateOther[@type="embargo"]');
    if ($embargo_date) {
      $this->embargoDate = $embargo_date[0];
    }

    $genre = $xml->xpath('//mods:genre');
    if ($genre) {
      $this->genre = $genre;
    }

    $username = $xml->xpath('//mods:identifier[@type="u1"]');
    if ($username) {
      $this->username = $username;
    }

    $department = $xml->xpath('//mods:identifier[@type="u2"]');
    if ($department) {
      $this->department = $department;
    }
  }

  /**
   * Function to return citation details in a table-ready form.
   *
   * @param string $type
   *   Type of reference that is being used. Default value is citation but
   * thesis and book are also valid. This parameter will change the values
   * that are returned when the renderView function is called
   *
   * @return array
   *   An array containing the headers and rows ready to put into a table
   * describing the reference
   */
  public function renderView($type = 'citation') {
    global $base_url;
    $admin = user_access('administer scholarly research');
    $fedora_url = variable_get('fedora_base_url', 'http://localhost:8080/fedora');

    $headers = array('', '');
    $rows = array();

    $full_name = $this->topic;
    if (!empty($this->names)) {
      $names = implode(', ', $this->names);
    }
    else {
      $names = '';
    }

    if (!empty($this->authors)) {
      $authors = implode(', ', $this->authors);
    }
    else {
      $authors = '';
    }

    if (!empty($this->contributors)) {
      $contributors = implode(', ', $this->contributors);
    }
    else {
      $contributors = '';
    }

    if (!empty($this->topic)) {
      $topics = $this->topic;
    }
    else {
      $topics = '';
    }

    $genre = $this->genre;
    if (stripos((string) $genre[0], 'book') !== FALSE) {
      $type = 'book';
    }

    if ($this->embargoDate != NULL) {
      $embargo_date = date('r', tstamptotime((string) $this->embargoDate));
    }
    else {
      $embargo_date = NULL;
    }

    if (((string) $this->title != NULL) || $admin) {
      $rows[] = array(t('Title'), $this->title);
    }

    if ($type == 'thesis') {
      if ($authors != NULL || $admin) {
        $rows[] = array(t('Authors'), rtrim($authors, ', '));
      }
      if ($contributors != NULL || $admin) {
        $rows[] = array(t('Contributors'), rtrim($contributors, ', '));
      }
      $url = l(t('Download PDF'), "islandora/object/{$this->pid}/datastream/PDF/download");
      $rows[] = array(t('URL'), $url);
    }
    else {
      if ($authors != NULL || $admin) {
        $rows[] = array(t('Authors'), rtrim($authors, ', '));
      }
    }

    if (((string) $this->abstract != NULL) || $admin) {
      $rows[] = array(t('Abstract'), $this->abstract);
    }

    if ($type == 'citation') {
      if (((string) $this->journalTitle != NULL) || $admin) {
        $rows[] = array(t('Journal'), $this->journalTitle);
      }
    }

    if (((string) $this->date != NULL) || $admin) {
      $rows[] = array(t('Date'), $this->date);
    }

    if ($type == 'citation') {
      if (((string) $this->volume != NULL) || $admin) {
        $rows[] = array(t('Volume'), $this->volume);
      }

      if (((string) $this->issue != NULL) || $admin) {
        $rows[] = array(t('Issue'), $this->issue);
      }

      if (((string) $this->startPage != NULL) || $admin) {
        $rows[] = array(t('Start page'), $this->startPage);
      }

      if (((string) $this->startPage != NULL) || $admin) {
        $rows[] = array(t('End page'), $this->endPage);
      }

      if (((string) $this->issn != NULL) || $admin) {
        $rows[] = array(t('ISSN'), $this->issn);
      }

      if (((string) $topics != NULL) || $admin) {
        $rows[] = array(t('Subjects'), rtrim($topics, ', '));
      }

      if (((string) $this->note != NULL) || $admin) {
        $rows[] = array(t('Note'), $this->note);
      }
    }

    if ($embargo_date != NULL) {
      $rows[] = array(t('Embargo date'), $embargo_date);
    }
    return array(
      'headers' => $headers,
      'rows' => $rows,
    );
  }

}

/**
 * Generate a table containing the contents of the MODS stream (if one exists).
 *
 * Uses the Coins class to perform extraction.
 *
 * @param AbstractObject $object
 *   An object containing a MODS stream.
 *
 * @return array
 *   A renderable array.
 */
function islandora_scholar_details($object) {
  if (isset($object['MODS']) && islandora_datastream_access(ISLANDORA_VIEW_OBJECTS, $object['MODS'])) {
    $coins = new Coins($object);
    extract($coins->renderView());
    return array(
      '#type' => 'item',
      '#title' => t('MODS Metadata'),
      '#markup' => theme('table', array(
        'header' => $headers,
        'rows' => $rows,
      )),
    );
  }
}
