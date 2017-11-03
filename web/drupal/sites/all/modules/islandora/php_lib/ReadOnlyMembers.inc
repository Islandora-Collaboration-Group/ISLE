<?php
// @codingStandardsIgnoreStart
/**
 * @file
 */
module_load_include('inc', 'php_lib', 'ReadOnlyPrivateMembers');
module_load_include('inc', 'php_lib', 'ReadOnlyProtectedMembers');

/**
 *
 */
class ReadOnlyMembers {

  /**
   * Private members that are read only.
   *
   * @var ReadOnlyPrivateMembers
   */
  public $private;
  /**
   * Protected members that are read only.
   *
   * @var ReadOnlyProtectedMembers
   */
  public $protected;

  /**
   *
   * @param array $private
   * @param array $protected
   */
  public function __construct(array $private, array $protected, array $params = NULL) {
    $owner = isset($params['owner']) ? $params['owner'] : get_caller_class(1);
    $depth = isset($params['depth']) ? $params['depth'] : 3;
    $properties = array(
      'owner_class' => $owner_class,
      'depth' => $depth,
    );
    // Account for this class in the depth.
    $this->private = new ReadOnlyPrivateMembers($private, $properties);
    $this->protected = new ReadOnlyProtectedMembers($owner_class, $properties);
  }

  /**
   * Clone this object, deeply.
   */
  public function __clone() {
    $this->private = clone $this->private;
    $this->protected = clone $this->protected;
  }

  /**
   *
   * @param string $name
   */
  public function has($name) {
    return $this->private->has($name) || $this->protected->has($name);
  }

  /**
   *
   * @param string $name
   * @return boolean
   */
  public function exists($name) {
    return $this->private->exists($name) || $this->protected->exists($name);
  }

  /**
   * Any one can access this member.
   */
  public function __get($name) {
    if ($this->private->has($name)) {
      return $this->private->$name;
    }
    if ($this->protected->has($name)) {
      return $this->protected->$name;
    }
    return NULL;
  }

  /**
   *
   * @param string $name
   * @param mixed $value
   */
  public function __set($name, $value) {
    if ($this->private->has($name)) {
      return $this->private->$name = $value;
    }
    if ($this->protected->has($name)) {
      return $this->protected->$name = $value;
    }
  }

  public function __isset($name) {
    return $this->exists($name);
  }

  public function __unset($name) {
    if ($this->private->exists($name)) {
      unset($this->private->$name);
    }
    if ($this->protected->has($name)) {
      unset($this->protected->$name);
    }
  }

}
// @codingStandardsIgnoreEnd
