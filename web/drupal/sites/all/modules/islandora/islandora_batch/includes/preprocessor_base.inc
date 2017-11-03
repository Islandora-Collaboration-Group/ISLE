<?php

/**
 * @file
 * Abstract preprocessor base class for batch.
 */

/**
 * Lock exception.
 */
class IslandoraBatchFailedToLockException extends Exception {

}

/**
 * Abstract preprocessor base class.
 */
abstract class IslandoraBatchPreprocessor {

  /**
   * @var array $parameters
   *   Should be populated with the parameters passed in.
   */
  protected $parameters = array();
  /**
   * @var IslandoraTuque $connection
   *   An IslandoraTuque object from which we can obtain IDs.
   */
  protected $connection;

  /**
   * @var string|int $setId
   *   A string/int identifying the set of objects.
   */
  protected $setId;

  /**
   * Constructor must be able to receive an associative array of parameters.
   *
   * @param array $parameters
   *   An associative array of parameters for the batch process. These will
   *   probably just be the result of a simple transformation from the
   *   command line, or something which could have been constructed from a
   *   form.
   *   Available parameters are from the particular concrete implementation.
   */
  public function __construct(IslandoraTuque $connection, array $parameters) {
    $this->parameters = $parameters;
    $this->connection = $connection;

    global $user;
    $insert = db_insert('islandora_batch_set')
      ->fields(array(
        'uid' => $user->uid,
        'created' => time(),
      ));
    $this->setId = $insert->execute();

    $this->getLock();
  }

  /**
   * Helper to acquire our lock.
   *
   * @return float
   *   If we managed to acquire the lock, the amount of time in seconds for
   *   which we acquired the lock; otherwise, we throw an exception.
   *
   * @throws IslandoraBatchFailedToLockException
   *   If we failed to obtain the lock.
   */
  protected function getLock() {
    require_once __DIR__ . '/ingest.batch.inc';
    $timer_name = __CLASS__ . "--SET:{$this->setId}";
    timer_start($timer_name);
    // Spend only up to 30 seconds spinning.
    while (timer_read($timer_name) < 30 * 1000) {
      // Attempt to get the lock for our set for the default amount of time.
      $lock = islandora_batch_get_lock($this->setId);
      if (!$lock) {
        throw new IslandoraBatchFailedToLockException(
            t('Failed to acquire lock for batch ingest.')
        );
      }
      return $lock;
    }
  }

  /**
   * PHP Magic function.
   *
   * Ensure we still have/can get our lock when we wake up, as we can be
   * serialized.
   */
  public function __wakeup() {
    $this->getLock();
  }

  /**
   * Actually cause the preprocessing to happen.
   *
   * In most cases, this should be fairly quick--not requiring multiple
   * iterations as a batch process.
   */
  abstract public function preprocess();

  /**
   * Get an identifier we can use for an object.
   *
   * @param AbstractFedoraObject $object
   *   The object to be later processed and added to Fedora.
   *
   * @return string
   *   An id to use in the back-end (Fedora).
   */
  protected function getId(AbstractFedoraObject $object) {
    // Command line argument overrides Collection Policy.
    if (isset($this->parameters['namespace'])) {
      $namespace = $this->parameters['namespace'];
    }
    else {
      $namespace = $this->determineNamespace($object);
    }
    // TODO: Implement some form of caching, so we can get multiple at a time.
    return $this->connection->repository->getNextIdentifier($namespace);
  }


  /**
   * Try to get the namespace from the collection policy.
   *
   * @param AbstractFedoraObject $object
   *   The object to be later processed and added to Fedora.
   *
   * @return string|NULL
   *   The namespace with which the object should be ingested.
   *   NULL if we use the Fedora configured default.
   */
  protected function determineNamespace(AbstractFedoraObject $object) {
    // Get namespace based on collection.
    $collection_policies =& drupal_static(__METHOD__, array());
    // Parameter not set.
    if (!isset($this->parameters['parent'])) {
      return NULL;
    }
    $parent_pid = $this->parameters['parent'];
    // Not specifying a namespace should result in one from the default
    // declared in the fedora.fcfg ("changeme" by default).
    $namespace = NULL;
    if (module_exists('islandora_basic_collection') && !isset($collection_policies[$parent_pid])) {
      $parent = islandora_object_load($parent_pid);
      if (isset($parent['COLLECTION_POLICY'])) {
        $policy_stream = $parent['COLLECTION_POLICY'];
        $collection_policies[$parent_pid] = new CollectionPolicy($policy_stream->content);
      }
    }
    if (isset($collection_policies[$parent_pid])) {
      $info = $collection_policies[$parent_pid]->getContentModels();
      $applicable = array_intersect_key($info, drupal_map_assoc($object->models));
      if ($applicable) {
        $first_applicable = reset($applicable);
        $namespace = $first_applicable['namespace'];
      }
    }
    return $namespace;
  }

  /**
   * Accessor for the set ID.
   */
  public function getSetId() {
    return $this->setId;
  }

  /**
   * Add the provided object to the database.
   *
   * Should be called from inside the "preprocess" method.
   *
   * @param AbstractFedoraObject $object
   *   The object to be later processed and added to Fedora.
   * @param array $resources
   *   An associative array of arrays, mapping types of resources to an array
   *   of relevant resources for the given object.
   *
   * @return string
   *   The PID of the object (one will be allocated if the object does not
   *   already have one; otherwise, that on the object will be returned).
   */
  protected function addToDatabase(AbstractFedoraObject $object, array $resources = array(), $parent = NULL) {
    if (!isset($object->id)) {
      $object->id = $this->getId($object);
    }

    db_insert('islandora_batch_queue')
      ->fields(array('id', 'parent', 'data', 'sid'))
      // Could queue up multiple in a static cache, to dump all together?
      ->values(array($object->id, $parent, serialize($object), $this->setId))
      ->execute();
    db_insert('islandora_batch_state')
      ->fields(array('id', 'state'))
      ->values(array($object->id, $object->initialBatchState()))
      ->execute();

    foreach ($resources as $type => $typed_resources) {
      foreach ($typed_resources as $resource) {
        db_insert('islandora_batch_resources')
          ->fields(array('id', 'type', 'resource'))
          ->values(array($object->id, $type, $resource))
          ->execute();
      }
    }
    return $object->id;
  }

}
