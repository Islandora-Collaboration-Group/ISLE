<?php
// @codingStandardsIgnoreStart
/**
 * @file
 * Provides a 'packed' array for occasions when PHP array consumes to much memory. Not that this class
 * is about twice as slow as PHP arrays, but around three times as compressed. It should only be used when dealing with large arrays where
 * processing speed isn't as much of a concern as memory usage.
 */
/**
 *
 */
class PackedArray implements ArrayAccess, Serializable {

  protected $array = array();

  public function __construct() {
    $args = func_get_args();
    foreach ($args as $k => $v) {
      $this->offsetSet($k, $v);
    }
  }

  public function offsetExists($offset) {
    return isset($this->array[$offset]);
  }

  public function offsetGet($offset) {
    if ($this->offsetExists($offset)) {
      return unserialize($this->array[$offset]);
    }
    return NULL;
  }

  public function offsetSet($offset, $val) {
    if (isset($offset)) {
      $this->array[$offset] = serialize($val);
    }
    else {
      $this->array[] = serialize($val);
    }
  }

  public function offsetUnset($offset) {
    unset($this->array[$offset]);
  }

  public function serialize() {
    return serialize($this->array);
  }

  public function unserialize($serialized) {
    $this->array = unserialize($serialized);
  }

}
// @codingStandardsIgnoreEnd
