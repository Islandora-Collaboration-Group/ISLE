<?php

/**
 * @file
 * Handles schema caching so that they don't need to be downloaded each request.
 */

define('XML_SCHEMA_API_CACHE_LOCATION', variable_get('file_public_path', conf_path() . '/files') . '/schemas');

/**
 * Stores/retrieves schema documents from the file location on disk.
 *
 * The mappings of urls to actual files are stored in the Drupal cache.
 */
class XmlSchemaCache {

  /**
   * The Drupal cache ID where the mappings of urls to actual files are stored.
   */
  const ID = 'xml-schema-table';
  const TYPE = 'cache';

  // @deprecated Constants
  // @codingStandardsIgnoreStart
  const Type = self::TYPE;
  // @codingStandardsIgnoreEnd

  /**
   * Checks if the cache exists.
   *
   * @return bool
   *   TRUE if the cache exists; FALSE otherwise.
   */
  public static function Exists() {
    return cache_get(self::ID, self::TYPE) && self::LocationExists();
  }

  /**
   * Creates the cache if it does not exist.
   *
   * @return bool
   *   TRUE if the cache was created or already exists; FALSE otherwise.
   */
  public static function Create() {
    if (!self::Exists()) {
      // Clear will initialize the table and files location.
      return self::Clear();
    }
    return TRUE;
  }

  /**
   * Clears the cache table and removes the cached files.
   *
   * @return bool
   *   TRUE if the cache was successfully cleared or initialized; FALSE if it
   *   fails.
   */
  public static function Clear() {
    // Assumed to always work.
    self::ClearCacheTable();
    return self::ClearCachedFiles();
  }

  /**
   * Clears the Cache table.
   */
  protected static function ClearCacheTable() {
    // Empty to start with.
    cache_set(self::ID, array(), self::TYPE, CACHE_PERMANENT);
  }

  /**
   * Removes the Cached files if they exist.
   *
   * Creates the cached file location if it doesn't exist.
   *
   * @return bool
   *   TRUE if the cached files were successfully cleared, or if the cached file
   *   location was successfully created.
   */
  protected static function ClearCachedFiles() {
    if (self::LocationExists()) {
      $files = scandir(XML_SCHEMA_API_CACHE_LOCATION);
      foreach ($files as $file) {
        if ($file != "." && $file != "..") {
          $full_filename = XML_SCHEMA_API_CACHE_LOCATION . "/" . $file;
          if (filetype($full_filename) != "dir") {
            unlink($full_filename);
          }
        }
      }
      return TRUE;
    }
    return @mkdir(XML_SCHEMA_API_CACHE_LOCATION, 0775, FALSE);
  }

  /**
   * Checks to see if the cache location exists.
   *
   * @param string $filename
   *   The name of the cached file to retrieve.
   *
   * @return bool
   *   TRUE if the cache exists; FALSE otherwise. Hmm ... this comment was also
   *   unfinished.
   */
  public static function LocationExists($filename = NULL) {
    $filename = empty($filename) ? XML_SCHEMA_API_CACHE_LOCATION : $filename;
    return file_exists($filename);
  }

  /**
   * Get the cached document.
   *
   * @param string $filename
   *   The filename used by schemas internally to refer to specific files. May
   *   also be a url.
   *
   * @return DOMDocument
   *   The cached DOMDocument if found; FALSE otherwise.
   */
  public static function Get($filename) {
    $cached_filename = self::GetCachedFilename($filename);
    if ($cached_filename !== FALSE) {
      $doc = new DOMDocument();
      if (!@$doc->load($cached_filename)) {
        // Failed to load clear instance from the cache.
        $cache = cache_get(self::ID, self::TYPE)->data;
        unset($cache[$filename]);
        cache_set(self::ID, $cache, self::TYPE);
        return FALSE;
      }
      // Success.
      return $doc;
    }
    return FALSE;
  }

  /**
   * Gets the cached document's filename.
   *
   * @param string $filename
   *   The filename used by schemas internally to refer to specific files. May
   *   also be a url.
   *
   * @return string
   *   The location of the cached if found; FALSE otherwise.
   */
  public static function GetCachedFilename($filename) {
    // Only Creates if none existed.
    if (self::Create()) {
      $cache = cache_get(self::ID, self::TYPE)->data;
      if (isset($filename) && isset($cache[$filename])) {
        if (self::LocationExists($cache[$filename])) {
          return $cache[$filename];
        }
        // Failed to find the file clear instance from the cache.
        unset($cache[$filename]);
        cache_set(self::ID, $cache, self::TYPE);
      }
    }
    return FALSE;
  }


  /**
   * Caches the given document with the given filename.
   *
   * @param string $filename
   *   The name of the schema file.
   * @param DOMDocument $doc
   *   The schema file.
   */
  public static function Set($filename, DOMDocument $doc) {
    // Only Creates if none existed.
    if (self::Create()) {
      $cached_filename = file_create_filename('schema.xsd', XML_SCHEMA_API_CACHE_LOCATION);
      if (@$doc->save($cached_filename) != FALSE) {
        $cache = cache_get(self::ID, self::TYPE)->data;
        $cache[$filename] = $cached_filename;
        cache_set(self::ID, $cache, self::TYPE);
      }
    }
  }

}
