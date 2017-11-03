<?php
// @codingStandardsIgnoreStart
/**
 * @file
 */

/**
 * Checks the given string to see if it could repersent a boolean value based on the specified matches.
 *
 * @param string $string
 *   The string to check.
 * @param array $matches
 *   The matches to compare the string with, if none are provided 'true' and 'false' will be used.
 * @param bool $case_sensitiveDo a case sensitive compare? Defaults to false.
 *   Do a case sensitive compare? Defaults to false.
 *
 * @return boolean
 *   TRUE if the string repersents a boolean value defined by matches, FALSE other wise.
 */
function is_boolean_string($string, array $matches = NULL, $case_sensitive = FALSE) {
  $matches = isset($matches) ? $matches : array('true', 'false');
  $cmp_func = ($case_sensitive) ? 'strcmp' : 'strcasecmp';
  foreach ($matches as $match) {
    if ($cmp_func($string, $match) === 0) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Attempts to cast the given string to a boolean value based on matched values.
 *
 * @param string $string
 *   The string to cast to a bool.
 * @param array $matches
 *   The matches to compare the string with, where the key is used to compare and the value is the boolean
 *   value it repersents. If none are provided 'true' and 'false' will be used.
 * @param bool $case_sensitiveDo a case sensitive compare? Defaults to false.
 *   Do a case sensitive compare? Defaults to false.
 *
 * @return mixed
 *   If the conversion is possible a boolean value repersented by the string is returned.
 *   If not possible then the string is returned intact.
 */
function cast_string_to_boolean($string, array $matches = NULL, $case_sensitive = FALSE) {
  $matches = isset($matches) ? $matches : array(
    'true' => TRUE,
    'false' => FALSE,
  );
  $cmp_func = ($case_sensitive) ? 'strcmp' : 'strcasecmp';
  foreach ($matches as $match => $value) {
    if ($cmp_func($string, $match) === 0) {
      return $value;
    }
  }
  return $string;
}

/**
 * Attempts to cast the given string to a particular type value based on matched values.
 *
 * @param string $string
 *   The string to cast to a type.
 *
 * @return mixed
 *   If the string repersents a literal type than convert to that type, other wise return the given string intact.
 */
function cast_string_to_type($string) {
  if (is_string($string)) {
    $matches = array(
      'true' => TRUE,
      'false' => FALSE,
      'null' => NULL,
    );
    foreach ($matches as $match => $value) {
      if (strcasecmp($string, $match) === 0) {
        return $value;
      }
    }
  }
  return $string;
}

/**
 * Attempts to cast the given type to a string.
 *
 * @param string $string
 *   The string to cast to a type.
 *
 * @return mixed
 *   If the string repersents a literal type than convert to that type, other wise return the given string intact.
 */
function cast_type_to_string($type) {
  if (is_bool($type)) {
    $type = $type ? 'TRUE' : 'FALSE';
  }
  if (!isset($type)) {
    $type = 'NULL';
  }
  return (string) $type;
}

/**
 * Appends a value to each new line in the given string.
 *
 * @param string $string
 * @param string $appendage
 *
 * @return string
 */
function append_to_new_line($string, $appendage) {
  $output = '';
  foreach (preg_split("/(\r?\n)/", $string) as $line) {
    $output .= $line . $appendage;
  }
  return $output;
}

/**
 *
 * @param string $string
 * @return boolean
 */
function is_non_empty_string($string) {
  return is_string($string) && trim($string) != '';
}
// @codingStandardsIgnoreEnd
