<?php
// @codingStandardsIgnoreStart
/**
 * @file
 */
module_load_include('inc', 'php_lib', 'Backtrace');
module_load_include('inc', 'php_lib', 'Array');

/**
 * Models memeber access.
 */
class Members {

  /**
   * Pointer to the Class that owns these members.
   *
   * @var object
   */
  protected $owner;
  /**
   * The depth in the call stack.
   *
   * @var integer
   */
  protected $depth;
  /**
   * A string of members.
   *
   * @var array
   */
  protected $members;
  /**
   * A map of values where the key's are the member names.
   *
   * @var array
   */
  protected $values;

  /**
   * Create a Members Object.
   *
   * @param array $members
   * @param array $params
   */
  public function __construct(array $members, array $params = NULL) {
    $this->owner = isset($params['owner']) ? $params['owner'] : get_caller_class(1);
    $this->depth = isset($params['depth']) ? $params['depth'] : 2;
    $this->members = array_keys($members);
    $this->values = $members;
  }

  /**
   * Checks to see if this class contains the given member.
   *
   * @param string $name
   *
   * @return boolean
   */
  public function has($name) {
    return array_search($name, $this->members) !== FALSE;
  }

  /**
   * Checks to see if this class contains the given member and
   * the value of the member is set.
   *
   * @param string $name
   *
   * @return boolean
   */
  public function exists($name) {
    if ($this->has($name)) {
      return isset($this->values[$name]);
    }
    return FALSE;
  }

  /**
   * Adds a new member and value to this class.
   *
   * If the member is already defined its value will be overwritten with the new value.
   *
   * @param string $name
   * @param mixed $value
   */
  public function addMember($name, $value = NULL) {
    $this->values[$name] = $value;
    if (!$this->has($name)) {
      $this->members[] = $name;
    }
  }

  /**
   * Adds several members to this class.
   *
   * @param array members
   *   A map of values where the key are the member's names.
   */
  public function addMembers(array $members) {
    foreach ($members as $name => $value) {
      $this->addMember($name, $value);
    }
  }

  /**
   * Removes a member, and its value.
   *
   * @param string $name
   */
  public function removeMember($name) {
    unset($this->members[$name]);
    unset($this->values[$name]);
  }

  /**
   * Get a value for the given member if it exists.
   *
   * Return NULL if the member doesn't exist or isn't set.
   *
   * @param string $name
   *
   * @return mixed
   */
  public function __get($name) {
    if ($this->exists($name)) {
      return $this->values[$name];
    }
    return NULL;
  }

  /**
   * Set the value for the given member if it exists.
   *
   * Note that this doesn't create a new element.
   *
   * @param string $name
   * @param mixed $value
   */
  public function __set($name, $value) {
    if ($this->has($name)) {
      $this->values[$name] = $value;
    }
  }

  /**
   * Checks to see if a member exists and isn't NULL.
   *
   * @param string $name
   *
   * @return boolean
   */
  public function __isset($name) {
    return $this->exists($name);
  }

  /**
   * Unset's a member.
   *
   * @param string $name
   */
  public function __unset($name) {
    unset($this->values[$name]);
  }

}
// @codingStandardsIgnoreEnd
