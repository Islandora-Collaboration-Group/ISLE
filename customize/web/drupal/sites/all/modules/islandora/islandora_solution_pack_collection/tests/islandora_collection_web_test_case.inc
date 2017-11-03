<?php

/**
 * @file
 * Test class for extending classes that use islandora_basic_collection.
 */

class IslandoraCollectionWebTestCase extends IslandoraWebTestCase {

  /**
   * Adds a child collection with the given params to the top-level collection.
   *
   * Use in lieu of throwing test objects into a standard collection.
   *
   * @param string $label
   *   The label that should be assigned to the collection
   * @param string|array $models
   *   The collection's content models; also accepts a single-model string
   * @param string $pid
   *   The PID that should be assigned to the collection
   *
   * @see IslandoraWebTestCase::deleteRootTestCollection()
   */
  public function createTestCollection($label, $models, $pid = '') {
    // If there is no logged in user, create one so that this collection auto-
    // purges itself.
    $user = FALSE;
    if (!$this->loggedInUser) {
      $user = $this->drupalCreateUser(array_keys(module_invoke_all('permission')));
      $this->drupalLogin($user);
    }

    // Set up the object and ingest it.
    $properties = array(
      'label' => $label,
      'pid' => $pid,
      'models' => 'islandora:collectionCModel',
      'parent' => 'islandora:root',
    );
    $policy = CollectionPolicy::emptyPolicy();
    foreach ((array) $models as $model) {
      $policy->addContentModel($model, 'New Object', 'islandora:1');
    }
    $datastreams = array(
      array(
        'dsid' => 'COLLECTION_POLICY',
        'string' => $policy->getXML(),
        'control_group' => 'X',
        'mimetype' => 'text/xml',
      ),
      array(
        'dsid' => 'TN',
        'path' => drupal_get_path('module', 'islandora_basic_collection') . '/images/folder.png',
        'control_group' => 'M',
        'mimetype' => 'image/png',
      ),
    );
    $object = $this->ingestConstructedObject($properties, $datastreams);

    // If we had to create a user, get rid of it.
    if ($user !== FALSE) {
      $this->drupalLogout();
    }

    // Assert that the object ingested correctly.
    $pid = $object == FALSE ? 'N/A' : $object->id;
    $this->assertFalse($pid === 'N/A', "Ingested collection object (PID: $pid)");
  }

  /**
   * Deletes a child collection from the root collection directly via PID.
   *
   * @param string $pid
   *   The PID of the collection to be deleted
   */
  public function deleteTestCollection($pid) {
    $path = 'islandora/object/' . $pid . '/delete';
    $this->drupalPost($path, array(), t('Delete'));
    $tuque = islandora_get_tuque_connection();
    $tuque->cache->resetCache();
    $object = islandora_object_load($pid);
    if (is_object($object)) {
      $this->fail(t("Tuque reports that the collection object %pid still exists after attempting to purge it through the interface. This test will attempt to purge the collection directly through tuque.", array('%pid' => $pid)), 'Islandora');
      try {
        islandora_delete_object($object);
        islandora_object_load($pid);
      }
      catch (Exception $e) {
        $this->fail(t("Tuque reports that it was unable to purge the collection %pid directly. This likely means that it does not actually exist. If verbose messaging is enabled in the testing module, you can check the last verbose message before this error to see whether or not it was actually purged.", array('%pid' => $pid)), 'Islandora');
      }
      $this->fail(t("Collection object %pid reports that deletion via tuque has failed. It may need to be deleted manually.", array('%pid' => $pid)), 'Islandora');
    }
    else {
      $this->pass(t("Collection object %pid successfully deleted.", array('%pid' => $pid)), 'Islandora');
    }
  }

}
