<?php

/**
 * @file
 * Translated from citeproc.js.
 */

class CSLDateParser {
  /**
   * Is singleton.
   */
  static public function getInstance($csl_date_parts = NULL, $clean = TRUE) {
    static $instance = NULL;
    if (is_null($instance)) {
      $instance = new CSLDateParser();
    }
    if ($clean) {
      $instance->setOrderMonthDay();
      $instance->resetMonths();
      if (!isset($csl_date_parts)) {
        $instance->cslDateParts = array(
          'year',
          'month',
          'day',
          'season',
        );
      }
    }
    if (isset($csl_date_parts)) {
      $instance->cslDateParts = $csl_date_parts;
    }
    return $instance;
  }

  protected $rexdash;
  protected $rexdashslash;
  protected $rexslashdash;
  protected $seasonrexes;
  protected $mstrings;
  protected $useArray = TRUE;
  protected $monthguess;
  protected $dayguess;
  protected $msets;
  protected $mabbrevs;
  protected $mrexes;
  protected $cslDateParts;

  /**
   * Constructor.
   */
  protected function __construct() {
    // Main parsing regexps.
    // %%NUMD%% and %%DATED%% are templates that will be replaced.
    $yearlast = "(?:[?0-9]{1,2}%%NUMD%%){0,2}[?0-9]{4}(?![0-9])";
    $yearfirst = "[?0-9]{4}(?:%%NUMD%%[?0-9]{1,2}){0,2}(?![0-9])";
    $number = "[?0-9]{1,3}";
    $rangesep = "[%%DATED%%]";
    $fuzzychar = "[?~]";
    $chars = "[a-zA-Z]+";
    $rex = "($yearfirst|$yearlast|$number|$rangesep|$fuzzychar|$chars)";
    $this->rexdash = preg_replace(array('/%%NUMD%%/', '/%%DATED%%/'), array('-', '-'), $rex);
    $this->rexdashslash = preg_replace(array('/%%NUMD%%/', '/%%DATED%%/'), array('-', '\/'), $rex);
    $this->rexslashdash = preg_replace(array('/%%NUMD%%/', '/%%DATED%%/'), array('\/', '-'), $rex);

    $seasons = array('spr', 'sum', 'fal', 'win');
    $this->seasonrexes = array();

    foreach ($seasons as $season) {
      $this->seasonrexes[] = "/$season.*/";
    }

    $this->mstrings = array(
      'january',
      'february',
      'march',
      'april',
      'may',
      'june',
      'july',
      'august',
      'september',
      'october',
      'november',
      'december',
      'spring',
      'summer',
      'fall',
      'winter',
      'spring',
      'summer',
    );

    $this->setOrderMonthDay();
  }

  /**
   * Set date part order interpretation.
   */
  protected function setOrderDayMonth() {
    $this->monthguess = 1;
    $this->dayguess = 0;
  }

  /**
   * Set date part order interpretation.
   */
  protected function setOrderMonthDay() {
    $this->monthguess = 0;
    $this->dayguess = 1;
  }

  /**
   * Reset months to default.
   */
  protected function resetMonths() {
    $this->msets = array();
    foreach ($this->mstrings as $mstring) {
      $this->msets[] = array($mstring);
    }

    $this->mabbrevs = array();
    foreach ($this->msets as $set) {
      $temp = array();
      foreach ($set as $j) {
        $temp[] = substr($j, 0, 3);
      }
      $this->mabbrevs[] = $temp;
    }

    $this->mrexes = array();
    foreach ($this->mabbrevs as $i) {
      $this->mrexes[] = "/(?:" . implode('|', $i) . ')/';
    }
  }

  /**
   * Extend month regexes.
   *
   * Extend the month regexes with an additional set of month strings,
   * extending strings as required to resolve ambiguities.
   */
  protected function addMonths($lst) {
    if (is_string($lst)) {
      $lst = preg_split('/\s+/', $lst);
    }
    // 12 for months, 16 for months and seasons.
    if (count($lst) !== 12 && count($lst) !== 16) {
      // TODO:  Throw some kinda error message...
      return;
    }

    $mab =& $this->mabbrevs;

    foreach ($lst as $key_i => $val_i) {
      $abbrevlen = FALSE;
      $skip = FALSE;
      $insert = 3;
      $extend = array();
      foreach ($mab as $key_j => $val_j) {
        // Mark for skipping if same as an existing abbreviation of same month.
        if ($key_j === $key_i) {
          foreach ($mab[$key_i] as $key_k => $val_k) {
            if ($val_k === substr($val_i, 0, strlen($val_k))) {
              $skip = TRUE;
              break;
            }
          }
        }
        // Mark for extending if same as existing abbreviation of another month.
        else {
          foreach ($mab[$key_j] as $key_k => $val_k) {
            $abbrevlen = strlen($val_k);
            if ($val_k === substr($val_i, 0, $abbrevlen)) {
              $mset_jk =& $this->msets[$key_j][$key_k];
              while (substr($mset_jk, 0, $abbrevlen) === substr($val_i, 0, $abbrevlen)) {
                // Abort when full length is hit, otherwise extend.
                if ($abbrevlen > strlen($val_i) || $abbrevlen > strlen($mset_jk)) {
                  // TODO:  Throw some kinda error about month parsing...
                  break;
                }
                // Mark both new entry and existing abbrev for extension.
                else {
                  $abbrevlen += 1;
                }
              }
              $insert = $abbrevlen;
              $extend[$key_j][$key_k] = $abbrevlen;
            }
          }
        }
        foreach ($extend as $key_1 => $val_1) {
          foreach ($val_1 as $key_2 => $val_2) {
            $mab[$key_1][$key_2] = substr($this->msets[$key_1][$key_2], 0, $val_2);
          }
        }
      }
      if (!$skip) {
        $this->msets[$key_i][] = $val_i;
        $mab[$key_i][] = substr($val_i, 0, $insert);
      }
    }

    $this->mrexes = array();
    foreach ($mab as $val) {
      $this->mrexes[] = '/(?:' . implode('|', $val) . ')/';
    }
  }

  /**
   * Try to parse a string into date info.
   */
  public function parse($txt) {
    $slash = $dash = FALSE;
    $range_delim = $date_delim = NULL;
    $txt = preg_replace('/\s*-\s*$/', '', $txt);
    $txt = preg_replace('/\s*-\s*\//', '/', $txt);
    $txt = preg_replace('/\.\s*$/', '', $txt);
    $txt = preg_replace('/\.(?! )/', '', $txt);
    $slash = strpos($txt, '/');
    $dash = strpos($txt, '-');

    // Drop punctuation from a.d., b.c.
    $txt = preg_replace('/([A-Za-z])\./', '$1', $txt);

    $number = $note = NULL;
    $thedate = array();
    $matches = array();
    if (substr($txt, 0, 1) === '"' && substr($txt, -1) === '"') {
      $thedate['literal'] = substr($txt, 1, -1);
      return $thedate;
    }
    elseif ($slash !== FALSE && $dash !== FALSE) {
      $slashes = explode('/', $txt);
      if (count($slashes) > 3) {
        $range_delim = '-';
        $date_delim = '/';
        preg_match_all($this->rexslashdash, $txt, $matches);
        $lst = reset($matches);
      }
      else {
        $range_delim = '/';
        $date_delim = '-';
        preg_match_all($this->rexdashslash, $txt, $matches);
        $lst = reset($matches);
      }
    }
    else {
      $txt = preg_replace('/\//', '-', $txt);
      $range_delim = '-';
      $date_delim = '-';
      preg_match_all($this->rexdash, $txt, $matches);
      $lst = reset($matches);
    }

    // Normalize to alphanumeric values.
    $ret = array();
    foreach ($lst as $val) {
      $match = array();
      if (preg_match('/^\s*([\-\/]|[a-zA-Z]+|[\-~?0-9]+)\s*$/', $val, $match)) {
        $ret[] = $match[1];
      }
    }

    $delim_pos = array_search($range_delim, $ret);
    $delims = array();
    $is_range = FALSE;
    if ($delim_pos !== FALSE) {
      $delims[] = array(0, $delim_pos);
      $delims[] = array($delim_pos + 1, count($ret));
      $is_range = TRUE;
    }
    else {
      $delims[] = array(0, count($ret));
    }

    // For each range divide.
    $suff = '';
    foreach ($delims as $delim) {
      $date = array_slice($ret, $delim[0], $delim[1]);
      foreach ($date as $element) {
        $lc = strtolower($element);
        // If it's a numeric date, process it.
        if (strpos($element, $date_delim) !== FALSE) {
          $this->parseNumericDate($thedate, $date_delim, $suff, $element);
          continue;
        }
        // If it's an obvious year, record it.
        elseif (preg_match('/[0-9]{4}/', $element) > 0) {
          $thedate["year$suff"] = preg_replace('/^0*/', '', $element);
        }

        $breakme = FALSE;
        foreach ($this->mrexes as $key => $mrex) {
          // If it's a month, record it. The last of the 16 months specified in
          // the date string will win. Granularity can be lost. The csl system
          // doesn't take a separate season param but maybe it should.
          if (preg_match($mrex, $lc) > 0) {
            $thedate["month$suff"] = '' . ($key + 1);
            $breakme = TRUE;
            break;
          }
          elseif ($breakme) {
            continue;
          }
          // If it's a number, note it.
          elseif (preg_match('/^[0-9]+$/', $lc)) {
            $number = intval($element);
          }
          // If it's a BC or AD marker, make a year of
          // any note.  Separate, reverse the sign of the year
          // if it's BC.
          if ($number && preg_match('/^bc/', $lc) > 0) {
            $thedate["year$suff"] = '' . ($number * -1);
            $number = '';
            continue;
          }
          elseif ($number && preg_match('/^ad/', $lc) > 0) {
            $thedate["year$suff"] = '' . $number;
            $number = '';
            continue;
          }
        }
        // If it's a season record it.
        $breakme = FALSE;
        foreach ($this->seasonrexes as $key => $srex) {
          if (preg_match($srex, $lc) > 0) {
            $thedate["season$suff"] = '' . ($key + 1);
            $breakme = TRUE;
            break;
          }
        }
        if ($breakme) {
          continue;
        }
        // If it's a fuzzy marker, record it.
        if ($element === '~' || $element === '?' || $element === 'c' || preg_match('/^cir/', $element) > 0) {
          $thedate['circa'] = 1;
          continue;
        }
        // If it's cruft, make a note of it.
        if (preg_match('/(?:mic|tri|hil|eas)/', $lc) > 0 && !array_key_exists("season$suff", $thedate)) {
          $note = $element;
          continue;
        }
        // Try to grab a day.
        if ($number && $number <= 31) {
          $thedate["day$suff"] = $number;
          $number = NULL;
          continue;
        }
        // If at the end of the string there's cruft lying
        // around, and the season field is empty, put the
        // cruft there.
        if (isset($note) && !array_key_exists("season$suff", $thedate)) {
          $thedate["season$suff"] = $note;
          $note = NULL;
          continue;
        }
      }
      $suff = '_end';
    }
    // Update any missing elements on each side of the divide from the other.
    if ($is_range) {
      foreach ((array) $this->cslDateParts as $item) {
        if (array_key_exists($item, $thedate) && !array_key_exists($item . '_end', $thedate)) {
          $thedate[$item . '_end'] = $thedate[$item];
        }
        elseif (!array_key_exists($item, $thedate) && array_key_exists($item . '_end', $thedate)) {
          $thedate[$item] = $thedate[$item . '_end'];
        }
      }
    }
    // If there's no year, it's a failure; pass through the literal.
    if (!array_key_exists('year', $thedate)) {
      $thedate = array('literal' => $txt);
    }

    if ($this->useArray) {
      return $this->toArray($thedate);
    }
    else {
      return $thedate;
    }
  }

  /**
   * Simplify returns from parse to a simple array.
   */
  public function returnAsArray() {
    $this->useArray = TRUE;
  }

  /**
   * Returns from parse give all raw date info.
   */
  public function returnAsKeys() {
    $this->useArray = FALSE;
  }

  /**
   * Turn some date data into a simplified array.
   */
  protected function toArray($thedate) {
    $to_return = array('date-parts' => array());

    if (array_key_exists('literal', $thedate)) {
      $to_return = array('literal' => $thedate['literal']);
    }
    else {
      $start_parts = array('year', 'month', 'day');
      $end_parts = array('year_end', 'month_end', 'day_end');
      $start = array();
      $end = array();
      foreach ($start_parts as $part) {
        if (!array_key_exists($part, $thedate)) {
          break;
        }
        else {
          $start[] = $thedate[$part];
        }
      }
      foreach ($end_parts as $part) {
        if (!array_key_exists($part, $thedate)) {
          break;
        }
        else {
          $end[] = $thedate[$part];
        }
      }
      $to_return['date-parts'][] = $start;
      if (count($start) === count($end)) {
        $to_return['date-parts'][] = $end;
      }
    }

    return $to_return;
  }

  /**
   * Try to turn a numeric date string into a useful array.
   */
  protected function parseNumericDate(&$ret, $delim, $suff, $txt) {
    $escaped_delim = preg_quote($delim);
    $lst = preg_split("/$escaped_delim/", $txt);
    foreach ($lst as $key => $val) {
      if (strlen($val) === 4) {
        $ret["year$suff"] = preg_replace('/^0*/', '', $val);
        if ($key === 0) {
          $lst = array_slice($lst, 1);
        }
        else {
          $lst = array_slice($lst, 0, $key);
        }
        break;
      }
    }
    foreach ($lst as &$val) {
      $val = intval($val);
    }

    // Month and day parse.
    if (count($lst) === 1) {
      $ret["month$suff"] = '' . $lst[0];
    }
    elseif (count($lst) === 2) {
      // Handle months being out of bounds.
      if ($lst[$this->monthguess] > 12 && $lst[$this->dayguess] <= 12) {
        $ret["month$suff"] = '' . $lst[$this->dayguess];
        $ret["day$suff"] = '' . $lst[$this->monthguess];
      }
      elseif ($lst[$this->monthguess] > 12) {
        $ret["month$suff"] = '';
        $ret["day$suff"] = '' . $lst[$this->dayguess];
      }
      else {
        $ret["month$suff"] = '' . $lst[$this->monthguess];
        $ret["day$suff"] = '' . $lst[$this->dayguess];
      }
    }
  }
}
