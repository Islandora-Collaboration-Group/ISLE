<?php

/**
 * @file
 * Subclass Xacml to facilitate communication to Islandora/Fedora.
 *
 * Need to have Drupal bootstrapped, due to use of module_load_includes
 */

class IslandoraXacml extends Xacml {
  /**
   * The idenitfy the datastream from which this policy was obtained (and to
   * which it should be written back to).
   *
   * @var $object
   * @var $pid
   */
  protected $object;
  protected $pid;

  /**
   * Constructor.
   *
   * @param AbstractObject $object
   *   A AbstractObject.
   * @param mixed $xacml
   *   A string containing XACML XML, or NULL to attempt to load from the given
   *   PID and DSID.
   */
  public function __construct($object, $xacml = NULL) {
    if ($xacml === NULL && isset($object['POLICY'])) {
      $xacml = $object['POLICY']->content;
    }

    parent::__construct($xacml);
    $this->pid = $object->id;
    $this->object = $object;
  }

  /**
   * Write our XACML stream to our PID/DSID pair.
   */
  public function writeBackToFedora() {
    // Only add relationships on POLICY datastream.
    if (variable_get('islandora_xacml_api_save_relationships', TRUE)) {
      $this->writeRelations();
    }

    $xml = $this->getXmlString();
    if (isset($this->object['POLICY'])) {
      $this->object['POLICY']->content = $xml;
    }
    else {
      $xacml_datastream = $this->object->constructDatastream('POLICY', 'M');
      $xacml_datastream->label = 'XACML Policy Stream';
      $xacml_datastream->mimetype = 'application/xml';
      $xacml_datastream->setContentFromString($xml);
      $this->object->ingestDatastream($xacml_datastream);
    }

    return TRUE;
  }

  /**
   * Writes our relations to Fedora.
   */
  protected function writeRelations() {
    $view_relationships = array(
      'isViewableByUser',
      'isViewableByRole',
    );
    list($viewable_by_user, $viewable_by_role) = $view_relationships;

    $manage_relationships = array(
      'isManageableByUser',
      'isManageableByRole',
    );
    list($manageable_by_user, $manageable_by_role) = $manage_relationships;

    $all_relationships = array_merge($view_relationships, $manage_relationships);

    $this->object->relationships->autoCommit = FALSE;

    // Remove all existing relationships.
    foreach ($all_relationships as $relationship) {
      $this->object->relationships->remove(ISLANDORA_RELS_EXT_URI, $relationship);
      foreach ($this->object as $dsid => $value) {
        $this->object[$dsid]->relationships->remove(ISLANDORA_RELS_INT_URI, $relationship);
      }
    }

    // Add Object Viewing Relationships.
    if ($this->viewingRule->isPopulated()) {
      $view_data = $this->viewingRule->getRuleArray();
      // Recompute the new values from the policy.
      foreach ($view_data['users'] as $user) {
        $this->object->relationships->add(ISLANDORA_RELS_EXT_URI, $viewable_by_user, $user, TRUE);
      }

      foreach ($view_data['roles'] as $role) {
        $this->object->relationships->add(ISLANDORA_RELS_EXT_URI, $viewable_by_role, $role, TRUE);
      }
    }

    // Add Datastream Viewing Relationships.
    if ($this->datastreamRule->isPopulated()) {
      $datastream_data = $this->datastreamRule->getRuleArray();
      foreach ($datastream_data['dsids'] as $dsid) {
        // Recompute the new values from the policy.
        foreach ($datastream_data['users'] as $user) {
          if (isset($this->object[$dsid])) {
            $this->object[$dsid]->relationships->add(ISLANDORA_RELS_INT_URI, $viewable_by_user, $user, TRUE);
          }
        }
        foreach ($datastream_data['roles'] as $role) {
          if (isset($this->object[$dsid])) {
            $this->object[$dsid]->relationships->add(ISLANDORA_RELS_INT_URI, $viewable_by_role, $role, TRUE);
          }
        }
      }
    }

    // Add Object Management Relationships.
    if ($this->managementRule->isPopulated()) {
      $management_data = $this->managementRule->getRuleArray();
      // Recompute the new values from the policy.
      foreach ($management_data['users'] as $user) {
        $this->object->relationships->add(ISLANDORA_RELS_EXT_URI, $manageable_by_user, $user, TRUE);
      }
      foreach ($management_data['roles'] as $role) {
        $this->object->relationships->add(ISLANDORA_RELS_EXT_URI, $manageable_by_role, $role, TRUE);
      }
    }

    $this->object->relationships->commitRelationships();
  }
}
