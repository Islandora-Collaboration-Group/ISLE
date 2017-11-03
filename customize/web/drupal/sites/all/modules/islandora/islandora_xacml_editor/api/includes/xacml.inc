<?php

/**
 * @file
 * This file defines a set of object for manipulating XACML. Other files in the
 * XACML module provide a lower level access to creating XACML these objects
 * work together to provide a nice high level view of a standard islandora
 * XACML object.
 */

/**
 * This abstract class represents a general XACML Rule. The XACML object
 * contains 4 standard XACML rules, which are all extended from this base class.
 */
abstract class XacmlRule {

  /**
   * Private internal representation of the XACML rule.
   *
   * @var array() containing rules that can be parsed by XacmlWriter
   * and XacmlParser.
   */
  protected $rule;

  /**
   * This points to the Xacml object that this rule is instantiated inside of,
   * so that references to other rules can be made.
   *
   * @var Xacml
   */
  protected $xacml;

  /**
   * Initialized a rule datastructure for XacmlWriter.
   *
   * @param string $id
   *   Takes the ID for the new rule as a string.
   * @param string $effect
   *   The effect of the rule (Permit or Deny)
   *
   * @return array
   *   A structure that is parsable by XacmlWriter.
   */
  protected function initializeRule($id, $effect) {
    $rule = array();

    $rule['ruleid'] = $id;
    $rule['effect'] = $effect;

    $rule['dsids'] = array();
    $rule['mimes'] = array();
    $rule['dsidregexs'] = array();
    $rule['mimeregexs'] = array();
    $rule['methods_default'] = array();
    $rule['methods'] = array();
    $rule['users'] = array();
    $rule['roles'] = array();

    return $rule;
  }

  /**
   * Helper function. Allows strings or arrays of strings to be passed in.
   *
   * @param string $type
   *   Array key to modify in internal $rules datastructure.
   * @param mixed $data
   *   Data to be added.
   */
  protected function setValue($type, $data) {
    if (is_array($data)) {
      $this->rule[$type] = array_merge($this->rule[$type], array_values($data));
    }
    else {
      $this->rule[$type][] = $data;
    }
  }

  /**
   * Helper function. Internal arrays may have repeated values,  fixes this.
   *
   * @param string $type
   *   Array key in internal datastructure to return
   *
   * @return array
   *   Array requested.
   */
  protected function getValues($type) {
    return array_unique($this->rule[$type]);
  }

  /**
   * Uses array_diff to remove data from internal rule representation.
   *
   * @todo This could all be made more efficient.
   *
   * @param string $type
   *   Array key to work on
   * @param mixed $data
   *   Data to be removed.
   */
  protected function removeValues($type, $data) {
    if (!is_array($data)) {
      $data = array($data);
    }

    $this->rule[$type] = array_diff($this->rule[$type], $data);
  }

  /**
   * Constructs new XacmlRule.
   *
   * @note
   *   This generic constructor does not set any methods. It assumes if arg1 is
   *   an array that array is an existing XACML rule datastructure. Concrete
   *   implementations should call parent::__construct then initialize the
   *   datastructure correctly if arg1 is NULL by calling
   *   parent::initializeRule() with the proper methods.
   *
   * @param array $arg1
   *   An array containing an pre-exisitng XACML rule or NULL.
   * @param Xacml $xacml
   *   A reference to the XACML dobject that this datastructure is part of.
   */
  public function __construct($arg1, $xacml) {
    if (is_array($arg1)) {
      $this->rule = $arg1;
      /* remove them now, add them later */
      $this->setValue('users', 'fedoraAdmin');
      $this->setValue('roles', 'administrator');
    }

    $this->xacml = $xacml;
  }

  /**
   * Clear the settings for the given rule.
   *
   * @param mixed $type
   *   Either a type or array of types to clear, or NULL (something which casts
   *   to an empty array) to clear all--except those values which are required--
   *   from the rule array.
   */
  public function clear($type = NULL) {
    $dont_touch = array(
      'ruleid',
      'effect',
      'methods',
      'methods_default',
    );

    $clearable = array_diff(array_keys($this->rule), $dont_touch);
    if (($types = (array) $type) && (count($types) > 0)) {
      $clearable = array_intersect($clearable, $types);
    }

    foreach ($clearable as $to_clear) {
      $this->rule[$to_clear] = array();
    }
  }

  /**
   * Returns true if the rule is populated with data, otherwise returns false.
   *
   * For example a rule can be created that has no users or roles.
   * This rule has no meaning in XACML. We need Users and Roles associated with
   * the rule. This function lets us know if the rule has be populated.
   *
   * @return bool
   *   Whether this XACML is populated.
   */
  public function isPopulated() {
    return $this->getUsers() || $this->getRoles();
  }

  /**
   * Check if the user with the specified username and roles has permission.
   *
   * @param string $user
   *   A string containing the user being tested.
   * @param array $roles
   *   An array of strings containing the roles being tested. Or NULL.
   * @param bool $default
   *   The default value to return.
   */
  protected function internalHasPermission($user, $roles = NULL, $default = TRUE) {
    // We always allow the administrator role.
    if (in_array('administrator', $roles)) {
      return TRUE;
    }

    // If the rule is not populated we return the default value.
    if (!$this->isPopulated()) {
      return $default;
    }

    // Otherwise we see if they are allowed.
    $boolean_user = in_array($user, $this->getUsers());
    $boolean_role = array_intersect($this->getRoles(), $roles);

    return ($boolean_user || $boolean_role);
  }

  /**
   * Add a user to the XACML rule.
   *
   * @param mixed $user
   *   String or array or strings containing users to add.
   */
  public function addUser($user) {
    $this->setValue('users', $user);
  }

  /**
   * Add roles to the XACML rule.
   *
   * @param mixed $role
   *   String or array of string containing roles to add.
   */
  public function addRole($role) {
    $this->setValue('roles', $role);
  }

  /**
   * Remove users from XACML Rule.
   *
   * @param mixed $user
   *   String or array of strings with users to remove.
   */
  public function removeUser($user) {
    $this->removeValues('users', $user);
  }

  /**
   * Remove roles from XACML rule.
   *
   * @param mixed $role
   *   String or array of string with roles to remove.
   */
  public function removeRole($role) {
    $this->removeValues('roles', $role);
  }

  /**
   * Get users associated with this XACML rule.
   *
   * @return array
   *   Array containing the users.
   */
  public function getUsers() {
    return $this->getValues('users');
  }

  /**
   * Get roles associated with this XACML rule.
   *
   * @return array
   *   Array containing the roles.
   */
  public function getRoles() {
    return $this->getValues('roles');
  }

  /**
   * Return the $rule datastructure associated with this object.
   *
   * @note
   *   This can be parsed by XacmlWriter. While the above functions only give
   *   the users and roles explicitly added to this object, this returns the
   *   datastructure containing all users and roles. It makes sure that the
   *   fedoraAdmin user and administrator role are always added so we don't lock
   *   administrators out of objects.
   *
   * @return array
   *   An array containing the XACML datastructure.
   */
  public function getRuleArray() {
    // Make sure fedoraAdmin can see everything.
    $rule = $this->rule;
    $rule['users'][] = 'fedoraAdmin';
    $rule['roles'][] = 'administrator';
    $rule['users'] = array_unique($rule['users']);
    $rule['roles'] = array_unique($rule['roles']);
    return $rule;
  }

  /**
   * Check if all default methods are set in the rule.
   *
   * @return bool
   *   Whether the rule has all the default methods set or not.
   */
  public function validateDefaultMethods() {
    $return = TRUE;
    if (isset($this->rule['methods_default']) && count($this->rule['methods_default']) > 0) {
      $compare = array_intersect($this->rule['methods_default'], $this->rule['methods']);
      if (count($compare) < count($this->rule['methods_default'])) {
        $return = FALSE;
      }
    }
    return $return;
  }
}

/**
 * Concrete implementation for the rule restricting who can manage an object.
 */
class XacmlManagementRule extends XacmlRule {

  /**
   * Calls the parent constructor.
   *
   * @note
   *  If $arg1 is NULL instantiates the rule as a new blank rule.
   *
   * @param mixed $arg1
   *   Existing Rule datastructure with ID ISLANDORA_XACML_API_MANAGEMENT_RULE
   *   or NULL.
   * @param Xacml $xacml
   *   Reference to the parent XACML object.
   */
  public function __construct($arg1, $xacml) {
    parent::__construct($arg1, $xacml);
    $methods_default = array(
      'addDatastream',
      'addDisseminator',
      'adminPing',
      'getDisseminatorHistory',
      'getNextPid',
      'ingest',
      'modifyDatastreamByReference',
      'modifyDatastreamByValue',
      'modifyDisseminator',
      'modifyObject',
      'purgeObject',
      'purgeDatastream',
      'purgeDisseminator',
      'setDatastreamState',
      'setDisseminatorState',
      'setDatastreamVersionable',
      'compareDatastreamChecksum',
      'serverShutdown',
      'serverStatus',
      'upload',
      'dsstate',
      'resolveDatastream',
      'reloadPolicies',
    );
    if ($arg1 == NULL) {
      $this->rule = $this->initializeRule(ISLANDORA_XACML_API_MANAGEMENT_RULE, 'Deny');
      $this->rule['methods'] = $methods_default;
    }
    $this->rule['methods_default'] = $methods_default;
  }

  /**
   * Check if the user with the specified username and roles has permission.
   *
   * @param string $user
   *   A string containing the user being tested.
   * @param mixed $roles
   *   An array of strings containing the roles being tested. Or NULL.
   */
  public function hasPermission($user, $roles = NULL) {
    // We pass FALSE because we want to deny management if there is no XACML
    // policy.
    return parent::internalHasPermission($user, $roles, FALSE);
  }
}

/**
 * Concrete implementation for the rule restricting who can view an object.
 */
class XacmlViewingRule extends XacmlRule {

  /**
   * This calls the parent constructor.
   *
   * @note
   *   If $arg1 is NULL instantiates the rule as a new blank rule.
   *
   * @param mixed $arg1
   *   Existing Rule datastructure with ID ISLANDORA_XACML_API_VIEWING_RULE or
   *   NULL.
   * @param Xacml $xacml
   *   Reference to the parent XACML object.
   */
  public function __construct($arg1, $xacml) {
    parent::__construct($arg1, $xacml);
    $methods_default = array(
      'api-a',
      'getDatastreamHistory',
      'listObjectInResourceIndexResults',
    );
    if ($arg1 == NULL) {
      $this->rule = $this->initializeRule(ISLANDORA_XACML_API_VIEWING_RULE, 'Deny');
      $this->rule['methods'] = $methods_default;
    }
    $this->rule['methods_default'] = $methods_default;
  }

  /**
   * Retrieves the ruleArray and adds roles and users.
   *
   * Calls parent::getRuleArray() and then adds the roles and users from the
   * managementRule and datastreamRule datastructues if they are populated.
   * This ensures that our xacml object works as expected. Otherwise it would
   * be possible to have people that could manage an object but not view
   * datastreams. An unexpected behavior.
   *
   * @return array
   *   Rule datastructure parsable by XacmlWriter.
   */
  public function getRuleArray() {
    $rule = parent::getRuleArray();
    if ($this->xacml->managementRule->isPopulated()) {
      $rule['users'] = array_unique(array_merge($rule['users'], $this->xacml->managementRule->getUsers()));
      $rule['roles'] = array_unique(array_merge($rule['roles'], $this->xacml->managementRule->getRoles()));
    }
    if ($this->xacml->datastreamRule->isPopulated()) {
      $rule['users'] = array_unique(array_merge($rule['users'], $this->xacml->datastreamRule->getUsers()));
      $rule['roles'] = array_unique(array_merge($rule['roles'], $this->xacml->datastreamRule->getRoles()));
    }
    return $rule;
  }

  /**
   * Check if the user with the specified username and roles has permission.
   *
   * @param string $user
   *   A string containing the user being tested.
   * @param mixed $roles
   *   An array of strings containing the roles being tested. Or NULL.
   */
  public function hasPermission($user, $roles = NULL) {
    return parent::internalHasPermission($user, $roles);
  }
}

/**
 * Concrete implementaion of a XacmlRule that allows everything.
 *
 * It needs to be added to the end of every XACML policy to allow anything not
 * explicitly forbidden by the policy. Otherwise XACML defaults to
 * denying access.
 *
 * This is entirely managed by Xacml object so not much needs to be said about
 * it.
 */
class XacmlPermitEverythingRule extends XacmlRule {
  /**
   * Calls the parent constructor.
   *
   * @param Xacml $xacml
   *   A reference to a Xacml object.
   */
  public function __construct($xacml) {
    parent::__construct(NULL, $xacml);
    $this->rule = $this->initializeRule(ISLANDORA_XACML_API_PERMIT_RULE, 'Permit');
  }

  /**
   * Retrieves the rule array.
   *
   * @return array
   *   Rule datastructure.
   */
  public function getRuleArray() {
    // Make sure fedoraAdmin can see everything.
    return $this->rule;
  }

  /**
   * Check if the user with the specified username and roles has permission.
   *
   * @param string $user
   *   A string containing the user being tested.
   * @param mixed $roles
   *   An array of strings containing the roles being tested. Or NULL.
   */
  public function hasPermission($user, $roles = NULL) {
    // We pass FALSE because we want to deny management if there is no XACML
    // policy.
    return parent::internalHasPermission($user, $roles);
  }
}

/**
 * A concrete implementation to restrict certain mimetypes and datastreams.
 */
class XacmlDatastreamRule extends XacmlRule {

  /**
   * Initialized the rule.
   *
   * @param mixed $arg1
   *   NULL or an existing $rule array with ID
   *   ISLANDORA_XACML_API_DATASTREAM_RULE.
   * @param Xacml $xacml
   *   Reference to parent Xacml object.
   */
  public function __construct($arg1, $xacml) {
    parent::__construct($arg1, $xacml);
    $methods_default = array(
      'getDatastreamDissemination',
    );
    if ($arg1 == NULL) {
      $this->rule = $this->initializeRule(ISLANDORA_XACML_API_DATASTREAM_RULE, 'Deny');
      $this->rule['methods'] = $methods_default;
    }
    $this->rule['methods_default'] = $methods_default;
  }

  /**
   * Retrieves the ruleArray and adds roles and users.
   *
   * Calls parent::getRuleArray() and then adds the roles and users fromt the
   * managementRule object if they are populated. This ensures that our XACML
   * object works as expected. Otherwise it would be possible to have people
   * that could manage an object but not view datastreams.
   * An unexpected behavior.
   *
   * @return array
   *   Rule datastructure parsable by XacmlWriter.
   */
  public function getRuleArray() {
    $rule = parent::getRuleArray();
    $rule['dsids'] = $this->getValues('dsids');
    $rule['mimes'] = $this->getValues('mimes');
    $rule['dsidregexs'] = $this->getValues('dsidregexs');
    $rule['mimeregexs'] = $this->getValues('mimeregexs');

    if ($this->xacml->managementRule->isPopulated()) {
      $rule['users'] = array_unique(array_merge($rule['users'], $this->xacml->managementRule->getUsers()));
      $rule['roles'] = array_unique(array_merge($rule['roles'], $this->xacml->managementRule->getRoles()));
    }

    return $rule;
  }

  /**
   * Add a dsid to the rule.
   *
   * @param mixed $dsid
   *   String or array of strings containing the datastream to add.
   */
  public function addDsid($dsid) {
    $this->setValue('dsids', $dsid);
  }

  /**
   * Add a dsid regex to the rule.
   *
   * @param mixed $regex
   *   String or array of strings containing the datastream to add.
   */
  public function addDsidRegex($regex) {
    $this->setValue('dsidregexs', $regex);
  }

  /**
   * Add a mimetype to the rule.
   *
   * @param mixed $mime
   *   String or array of strings to add to the rule.
   */
  public function addMimetype($mime) {
    $this->setValue('mimes', $mime);
  }

  /**
   * Add a mimetype regex to the rule.
   *
   * @param mixed $regex
   *   String or array of strings to add to the rule.
   */
  public function addMimetypeRegex($regex) {
    $this->setValue('mimeregexs', $regex);
  }

  /**
   * Remove mimetypes from the rule.
   *
   * @param mixed $mime
   *   String or array ofs tring to remove from the rule.
   */
  public function removeMimetype($mime) {
    $this->removeValues('mimes', $mime);
  }

  /**
   * Remove mimetype regexs from the rule.
   *
   * @param mixed $regex
   *   String or array ofs tring to remove from the rule.
   */
  public function removeMimetypeRegex($regex) {
    $this->removeValues('mimeregexs', $regex);
  }

  /**
   * Remove dsids from the rule.
   *
   * @param mixed $dsid
   *   String or array of strings to remove from the rule.
   */
  public function removeDsid($dsid) {
    $this->removeValues('dsids', $dsid);
  }

  /**
   * Remove dsid regexs from the rule.
   *
   * @param mixed $regex
   *   String or array ofs tring to remove from the rule.
   */
  public function removeDsidRegex($regex) {
    $this->removeValues('dsidregexs', $regex);
  }

  /**
   * Mimetypes associated with this rule.
   *
   * @return array
   *   Array of mimetypes.
   */
  public function getMimetypes() {
    return $this->getValues('mimes');
  }

  /**
   * Mimetypes associated with this rule.
   *
   * @return array
   *   Array of mimetype regexs.
   */
  public function getMimetypeRegexs() {
    return $this->getValues('mimeregexs');
  }

  /**
   * Dsids associated with this rule.
   *
   * @return array
   *   Array of dsids.
   */
  public function getDsids() {
    return $this->getValues('dsids');
  }

  /**
   * Dsid regexs associated with this rule.
   *
   * @return array
   *   Array of dsid regexs.
   */
  public function getDsidRegexs() {
    return $this->getValues('dsidregexs');
  }

  /**
   * Returns TRUE if the rule is populated with data, otherwise returns FALSE.
   *
   * For example a rule can be created that has no users, roles, dsids or
   * mimetypes. This makes sure there is at least on role or user and at least
   * one mimtype or dsid.
   *
   * @return bool
   *   Whether the rule is populated or not.
   */
  public function isPopulated() {
    return parent::isPopulated() && ($this->getMimetypes() || $this->getDsids() || $this->getDsidRegexs() || $this->getMimetypeRegexs());
  }

  /**
   * Check if the user with the specified username and roles has permission.
   *
   * @param string $user
   *   A string containing the user being tested.
   * @param mixed $roles
   *   An array of strings containing the roles being tested. Or NULL.
   * @param string $mime
   *   String containing the mime
   * @param string $dsid
   *   String containing the DSID
   *
   * @return bool
   *   Whether the user has permission or not.
   */
  public function hasPermission($user, $roles, $mime, $dsid) {
    // We need to check the isPopulated function for this one because it is
    // overridden.
    if (!$this->isPopulated()) {
      return TRUE;
    }
    if (!parent::internalHasPermission($user, $roles)) {
      $boolean_mime = $mime ? in_array($mime, $this->getMimetypes()) : FALSE;
      $boolean_dsid = $dsid ? in_array($dsid, $this->getDsids()) : FALSE;
      $boolean_mimeregex = FALSE;
      $boolean_dsidregex = FALSE;

      $mimeregexs = $this->getMimetypeRegexs();

      if (isset($mimeregexs)) {
        foreach ($mimeregexs as $key => $value) {
          preg_match('/' . $value . '/', $mime, $match);

          if (count($match) > 0) {
            $boolean_mimeregex = TRUE;
            break;
          }
        }
      }

      $dsidregexs = $this->getDsidRegexs();

      if (isset($dsidregexs)) {
        foreach ($dsidregexs as $key => $value) {
          preg_match('/' . $value . '/', $dsid, $match);

          if (count($match) > 0) {
            $boolean_dsidregex = TRUE;
            break;
          }
        }
      }

      if ($boolean_mime || $boolean_dsid || $boolean_mimeregex || $boolean_dsidregex) {
        return FALSE;
      }
      else {
        return TRUE;
      }
    }
    return TRUE;
  }


}

/**
 * This class is how programmers should interact with Xacml objects. It takes
 * either XACML XML as a string or no arguements and creates a blank XACML
 * object. The interaction with the rules takes place through member object of
 * this class. For instance to add roles that can manage the object:
 * @code
 *   xacml = new Xacml()
 *   // allow userA to manage the object
 *   xacml->managementRule->addUser('userA')
 *   // allow roleC and roleD to manage the object
 *   xacml->managementRule->addRole(array('roleC', 'roleD'))
 * @endcode
 */
class Xacml {

  /**
   * The $xacml datastructure parsable by XacmlWriter and XacmlParser.
   * @var array()
   */
  protected $xacml;
  /**
   * Rule to allow anything. Users shouldn't need to interact with this.
   * @var XacmlPermitEverythingRule
   */
  protected $permitEverythingRule;

  /**
   * Rule controling who can manage the object with this XACML policy.
   * @var XacmlManagementRule
   */
  public $managementRule;
  /**
   * Rule controlling who can view the specific datastreams and mimetypes that
   * are in this rule.
   * @var XacmlDatastreamRule
   */
  public $datastreamRule;
  /**
   * Rule controlling who can view datastreams in this object.
   * @var XacmlViewingRule
   */
  public $viewingRule;

  /**
   * Initializes the $xacml datastructure that can be parsed with XacmlWriter.
   * @return array
   *   Shell array representing an XACML object.
   */
  protected function initializeXacml() {
    // Create the rule array.
    $xacml = array(
      'RuleCombiningAlgId' => 'urn:oasis:names:tc:xacml:1.0:rule-combining-algorithm:first-applicable',
      'rules'              => array(),
    );
    return $xacml;
  }

  /**
   * The constructor for the XACML object. Initialize new XACML object.
   *
   * @param (optional) $xacml
   *   The XACML XML as a string. If this isn't passed the constructor will
   *   instead create a new XACML object that permits everything.
   *
   * @throws XacmlException if the XML cannot be parsed
   */
  public function __construct($xacml = NULL) {

    $management_rule = NULL;
    $datastream_rule = NULL;
    $viewing_rule = NULL;

    if ($xacml != NULL) {
      $this->xacml = XacmlParser::parse($xacml);

      // Decide what is enabled.
      foreach ($this->xacml['rules'] as $rule) {
        if ($rule['ruleid'] == ISLANDORA_XACML_API_MANAGEMENT_RULE) {
          $management_rule = $rule;
        }
        elseif ($rule['ruleid'] == ISLANDORA_XACML_API_DATASTREAM_RULE) {
          $datastream_rule = $rule;
        }
        elseif ($rule['ruleid'] == ISLANDORA_XACML_API_VIEWING_RULE) {
          $viewing_rule = $rule;
        }
      }
    }
    else {
      $this->xacml = $this->initializeXacml();
    }

    $this->datastreamRule = new XacmlDatastreamRule($datastream_rule, $this);
    $this->managementRule = new XacmlManagementRule($management_rule, $this);
    $this->viewingRule = new XacmlViewingRule($viewing_rule, $this);
    $this->permitEverythingRule = new XacmlPermitEverythingRule($this);
  }

  /**
   * Updates the rules array before it is passed to XacmlWriter.
   *
   * @note
   *   It takes into account which rules have been populated.
   */
  protected function updateRulesArray() {
    $this->xacml['rules'] = array();

    if ($this->datastreamRule->isPopulated()) {
      $this->xacml['rules'][] = $this->datastreamRule->getRuleArray();
    }
    if ($this->managementRule->isPopulated()) {
      $this->xacml['rules'][] = $this->managementRule->getRuleArray();
    }
    if ($this->viewingRule->isPopulated()) {
      $this->xacml['rules'][] = $this->viewingRule->getRuleArray();
    }
    $this->xacml['rules'][] = $this->permitEverythingRule->getRuleArray();
  }

  /**
   * Returns the DomDocument that is associated with this Xacml Rule.
   *
   * @return DomDocument
   *   The DomDocument associated with this Xacml rule.
   */
  public function getDomDocument() {
    $this->updateRulesArray();
    return XacmlWriter::toDOM($this->xacml);
  }

  /**
   * Returns a string containing the XML for this XACML policy.
   *
   * @param bool $pretty_print
   *   If set to TRUE the function will return a prettyprinted xacml policy.
   *
   * @return string
   *   String containing XACML XML.
   */
  public function getXmlString($pretty_print = TRUE) {
    $this->updateRulesArray();
    return XacmlWriter::toXML($this->xacml, $pretty_print);
  }
}
