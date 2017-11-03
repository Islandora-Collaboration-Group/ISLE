<?php

/**
 * @file
 * Abstract base class for tests.
 */

abstract class BibutilsBaseTestCase extends DrupalUnitTestCase {
  /**
   * The location of the data directory for the tests.
   *
   * @var string
   */
  protected $data;

  /**
   * Performs any pre-requisite tasks.
   *
   * @see DrupalUnitTestCase::setUp()
   */
  public function setUp() {
    parent::setUp('bibutils');
    $this->data = drupal_get_path('module', 'bibutils') . '/tests/data/';
  }

  /**
   * Test that multiple documents were created.
   *
   * @param mixed $documents
   *   The value to test.
   */
  protected function testCreatedDocuments($documents) {
    $this->assertTrue(is_array($documents), 'Created multiple mods documents.');
    $this->assertEqual(count($documents), 11, 'Created the correct number of mods documents.');
    $valid = TRUE;
    foreach ($documents as $document) {
      if (!$document->schemaValidate($schema)) {
        $valid = FALSE;
        break;
      }
    }
    $this->assertTrue($valid, 'All mods documents are valid.');
  }
}
