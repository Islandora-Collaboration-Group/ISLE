<?php
/**
 * @file
 * Batch functionality for the Islandora XACML Editor.
 */

/**
 * Batch callback function which updates the POLICY for a target pid.
 *
 * @param XML $xml
 *   The XML content defining an XACML policy.
 * @param string $pid
 *   The PID of the object we are going to update.
 * @param array $query_array
 *   An associative array where the key is the unique ID and contains:
 *   -type: The type of query, either sparql or itql.
 *   -query: The defined query string.
 *   -description: The human-readable description of the query.
 * @param array $context
 *   Context array used in the batch.
 */
function islandora_xacml_editor_batch_function($xml, $pid, $query_array, &$context) {
  if (empty($context['sandbox'])) {
    $query = new IslandoraXacmlEditorQuery($pid, $query_array);
    $context['sandbox'] = array();
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['pids'] = $query->getPids();
    $context['sandbox']['items'] = count($context['sandbox']['pids']);
    $context['results']['redirect'] = $pid;
    $context['results']['success'] = array();
    $context['results']['fail'] = array();
  }
  $targetpid = array_pop($context['sandbox']['pids']);
  $context['sandbox']['progress']++;

  $policy_update = new IslandoraUpdatePolicy($targetpid, $xml);
  $success = $policy_update->updatePolicy();

  if ($success) {
    $context['results']['success'][] = $targetpid;
  }
  else {
    $context['results']['fail'][] = $targetpid;
  }

  // Make sure we don't divide by zero.
  $context['finished'] = $context['sandbox']['items'] == 0 ? 1 : $context['sandbox']['progress'] / $context['sandbox']['items'];
}

/**
 * Finished function for the update policy batch.
 *
 * @param bool $success
 *   Whether the batch was successful or not.
 * @param array $results
 *   An array containing the results of the batch operations.
 * @param array $operations
 *   The operations array that was used in the batch.
 */
function islandora_xacml_editor_batch_finished($success, $results, $operations) {
  if ($success) {
    $message = format_plural(count($results['success']), 'One policy updated.', '@count policies updated.');
  }
  else {
    $message = t('Finished with an error.');
  }
  drupal_set_message($message);

  if ($results['fail']) {
    foreach ($results['fail'] as $fail) {
      drupal_set_message(t("Failed to update: @failed_object. You do not have permission to update this object.", array('@failed_object' => $fail)), 'error');
    }
  }
}

/**
 * Class used in the batch updating of POLICY datastreams on objects.
 */
class IslandoraUpdatePolicy {
  protected $pid;

  /**
   * Constructorsaurusrex.
   *
   * @param string $pid
   *   The pid of the object we are batching.
   * @param XML $xml
   *   The XACML XML.
   */
  public function __construct($pid, $xml) {
    // Used at a couple different points...  Let's just load this here?
    $this->pid = $pid;
    $this->xml = $xml;
    $this->object = islandora_object_load($pid);
  }

  /**
   * Updates the POLICY datastream of the object.
   *
   * @return bool
   *   The success of the operation.
   */
  public function updatePolicy() {
    global $user;
    $success = FALSE;
    if (isset($this->object)) {
      try {
        $xacml = new IslandoraXacml($this->object);
        if (!isset($this->object['POLICY']) || !$xacml->managementRule->isPopulated() || $xacml->managementRule->hasPermission($user->name, $user->roles)) {
          $success = $this->addOrUpdateAllPolicies();
        }
      }
      catch (XacmlException $e) {
      }
    }
    return $success;
  }

  /**
   * Updates or adds the new POLICY datastream back to the object.
   */
  protected function addOrUpdateAllPolicies() {
    $object_policy = new IslandoraXacml($this->object, $this->xml);
    $object_policy->writeBackToFedora();
    return TRUE;
  }
}

/**
 * Class used in executing queries to Fedora.
 */
class IslandoraXacmlEditorQuery {
  protected $pid;
  protected $query;

  /**
   * Constructorsaurusrex.
   *
   * @param string $pid
   *   The PID of the object using as a base for queries.
   * @param array $query_array
   *   An associative array where the key is the unique ID and contains:
   *   -type: The type of query, either sparql or itql.
   *   -query: The defined query string.
   *   -description: The human-readable description of the query.
   */
  public function __construct($pid, $query_array) {
    $this->pid = $pid;
    $this->query = $query_array;
    $this->result_pids = array();
  }

  /**
   * Executes a defined query.
   *
   * @param array $query_array
   *   An associative array where the key is the unique ID and contains:
   *   -type: The type of query, either sparql or itql.
   *   -query: The defined query string.
   *   -description: The human-readable description of the query.
   * @param string $object_pid
   *   The pid of the object we are using as a base for queries.
   *
   * @return array
   *   An array containing the results of our query.
   */
  public function query($query_array, $object_pid) {
    module_load_include('inc', 'islandora', 'includes/utilities');
    $object = islandora_object_load($object_pid);
    if ($query_array['type'] == 'itql') {
      $content = $object->repository->ri->itqlQuery($query_array['query']);
    }
    else {
      $content = $object->repository->ri->sparqlQuery($query_array['query']);
    }
    foreach ($content as $result) {
      // We need to potentially recurse down even more to cover differing data
      // structure for example books and newspapers.
      $result_pid = $result['object']['value'];
      if (!isset($this->result_pids[$result_pid])) {
        $this->result_pids[$result_pid] = TRUE;
        $restricted_cmodels = array();
        if (isset($query_array['restricted_cmodels'])) {
          $restricted_cmodels = $query_array['restricted_cmodels'];
        }
        $result_object = islandora_object_load($result_pid);
        if ($result_object) {
          $object_models = array_diff($result_object->models, $restricted_cmodels);
          foreach (islandora_build_hook_list('islandora_xacml_editor_child_query', $object_models) as $hook) {
            $query_implementations = module_invoke_all($hook, $result_object);
            if (!empty($query_implementations)) {
              foreach ($query_implementations as $query_choice) {
                $this->query($query_choice, $result['object']['value']);
              }
            }
          }
        }
      }
    }
  }

  /**
   * Helper function that retrieves all results of our query.
   * @return array
   *   An array of the PIDs returned from the defiend query.
   */
  public function getPids() {
    IslandoraXacmlEditorQuery::query($this->query, $this->pid);
    $result_pids = array_keys($this->result_pids);
    return $result_pids;
  }
}
