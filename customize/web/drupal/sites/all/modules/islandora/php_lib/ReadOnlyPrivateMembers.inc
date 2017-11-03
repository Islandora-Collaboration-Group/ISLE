<?php
// @codingStandardsIgnoreStart
/**
 * @file
 */
module_load_include('inc', 'php_lib', 'Backtrace');

/**
 *
 */
class ReadOnlyPrivateMembers {

  /**
   *
   * @var string
   */
  protected $owner;
  /**
   * Call depth.
   *
   * @var int
   */
  protected $depth;
  /**
   *
   * @var array
   */
  protected $members;
  /**
   *
   * @var type
   */
  protected $values;

  /**
   *
   * @param array $members
   * @param array $params
   *   Optional parameters that set what the owning class is.
   */
  public function __construct(array $members, array $params = NULL) {
    $this->owner = isset($params['owner']) ? $params['owner'] : get_caller_class(1);
    $this->depth = isset($params['depth']) ? $params['depth'] : 2;
    $this->members = array_keys($members);
    $this->values = $members;
  }

  /**
   * Clone this object, deeply.
   */
  public function __clone() {
    $this->members = $this->members;
    $this->values = $this->values;
  }

  /**
   *
   * @param string $name
   */
  public function has($name) {
    if (array_search($name, $this->members)) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   *
   * @param string $name
   * @return boolean
   */
  public function exists($name) {
    if ($this->has($name)) {
      return isset($this->values[$name]);
    }
    return FALSE;
  }

  /**
   *
   */
  public function add($name, $value = NULL) {
    array_push($this->members, $name);
    $this->values[$name] = $value;
  }

  /**
   * Removes a member.
   *
   * @param string $name
   */
  public function remove($name) {
    unset($this->members[$name]);
  }

  /**
   * Any one can access this member.
   */
  public function __get($name) {
    if ($this->exists($name)) {
      return $this->values[$name];
    }
    return NULL;
  }

  /**
   *
   * @param string $name
   * @param mixed $value
   */
  public function __set($name, $value) {
    if ($this->has($name)) {
      if (get_caller_class($this->depth) == $this->owner) {
        $this->values[$name] = $value;
      }
    }
  }

  public function __isset($name) {
    return $this->exists($name);
  }

  public function __unset($name) {
    unset($this->values[$name]);
  }

}
// @codingStandardsIgnoreEnd
