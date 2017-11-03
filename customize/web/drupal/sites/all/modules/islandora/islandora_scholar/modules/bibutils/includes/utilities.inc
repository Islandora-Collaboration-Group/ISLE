<?php

/**
 * @file
 * Class that encompases the Bibutils tools.
 */

class Bibutils {
  /**
   * Gets the bibutils conversion table.
   *
   * A table where the keys are the source file types and their value is an
   * array where the destination file type is the keys and the required
   * command the value.
   *
   * @return array
   *   The conversion table.
   */
  protected static function getConversionTable() {
    return array(
      'EndNoteXML' => array(
        'MODS' => array(
          'type' => 'shell',
          'command' => escapeshellcmd(format_string('!command -i unicode', array(
            '!command' => variable_get('bibutils_endx2xml_path', 'endx2xml'),
          ))),
        ),
      ),
      'RIS' => array(
        'MODS' => array(
          'type' => 'shell',
          'command' => escapeshellcmd(format_string('!command -i unicode', array(
            '!command' => variable_get('bibutils_ris2xml_path', 'ris2xml'),
          ))),
        ),
      ),
      'MODS' => array(
        'RIS' => array(
          'type' => 'callable',
          'callable' => 'RIS::transformFile',
        ),
      ),
    );
  }

  /**
   * Tests if the file format is convertable.
   *
   * @param string $type
   *   The type of file to test.
   *
   * @return bool
   *   TRUE if convertiable by Bibutils FALSE otherwise.
   */
  protected static function IsConvertable($type) {
    return array_key_exists($type, self::getConversionTable());
  }

  /**
   * Gets the Bibutil Command for performing a conversion for the given formats.
   *
   * @param string $src_format
   *   The file format of the source document.
   * @param string $dest_format
   *   The file format the source document will be converted into.
   *
   * @return string
   *   The command that will perform the conversion if possible FALSE otherwise.
   */
  protected static function GetCommand($src_format, $dest_format) {
    $conversion_table = self::getConversionTable();
    $conversion = isset($conversion_table[$src_format]) ? $conversion_table[$src_format] : array();
    if (isset($conversion[$dest_format])) {
      return $conversion[$dest_format];
    }
    return FALSE;
  }

  /**
   * Bibutils produces invalid MODS, this function cleans up the generated MODS.
   *
   * @param string $filename
   *   The name of the file to clean.
   *
   * @return bool
   *   TRUE if succesful FALSE otherwise.
   */
  public static function CleanModsFile($filename) {
    $transform_filename = drupal_get_path('module', 'bibutils') . '/xsl/FixBibutilsModsOutput.xsl';
    $transform = new DOMDocument();
    $transform->load($transform_filename);
    $doc = new DOMDocument();
    $doc->load($filename);

    if (isset($doc->documentElement)) {
      $xslt = new XSLTProcessor();
      $xslt->importStyleSheet($transform);
      $doc = $xslt->transformToDoc($doc->documentElement);
      return $doc->save($filename) !== FALSE;
    }
  }

  /**
   * Executes the given command and the exit status and output of the command.
   *
   * @param string $command
   *   The command to execute.
   *
   * @return array
   *   Where the first parameter was the exit status of the command and the
   *   second is the commands output.
   */
  protected static function Execute($command, $src, $dest) {
    if ($command['type'] == 'shell') {
      $return_value = FALSE;
      ob_start();
      $command = "{$command['command']} $src 2>&- 1> $dest";
      passthru($command, $return_value);
      $output = ob_get_contents();
      ob_end_clean();
      return array($return_value, $output);
    }
    elseif ($command['type'] == 'callable') {
      $return = file_put_contents($dest, call_user_func($command['callable'], $src));
      return array($return, t('Failed to transform in %callable.', array(
        '%callable' => $command['callable'],
      )));
    }
  }

  /**
   * Converts the given file to the destination format, and saves it.
   *
   * @param string $src
   *   The filename of the source file.
   * @param string $src_format
   *   The format of the source file.
   * @param string $dest
   *   The filename of the destination file.
   * @param string $dest_format
   *   The format of the destination file.
   *
   * @return bool
   *   TRUE if successful, FALSE otherwise.
   */
  public static function Convert($src, $src_format, $dest, $dest_format) {
    $command = self::GetCommand($src_format, $dest_format);
    if ($command) {
      module_invoke_all('preprocess_bibutils_convert', $src, $src_format, $dest_format);
      list($exit_status, $output) = self::Execute($command, $src, $dest);
      if ($exit_status !== FALSE) {
        $results = module_invoke_all('postprocess_bibutils_convert', $src, $src_format, $dest, $dest_format);
        // No post process function returned false.
        return !in_array(FALSE, $results, TRUE);
      }
      else {
        drupal_set_message(filter_xss($output), 'warning');
      }
    }
    return FALSE;
  }
}
