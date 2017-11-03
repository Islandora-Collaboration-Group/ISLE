<?php
// @codingStandardsIgnoreStart
/**
 * @file
 * A class to model Enumerated types. This is modeled after SplEnum which is only availible via PECL
 * PECL Restriction: http://de2.php.net/manual/en/spl-types.installation.php
 * SplEnum Class: http://de2.php.net/manual/en/class.splenum.php.
 *
 * Instead of forcing users to have PECL extensions installed I'm rewriting the class here.
 */

/**
 * Only supports one level of extension, all child classes should be declared final.
 *
 * Since Operator overloading is only availible via PECL extensions we need to compare the values directly or
 * via objects. So Statements like Enum::Const == new Enum(Enum::Const) won't work like they would with SplEnum.
 *
 * To compare and object and const value we can do something like Enum::Const == (new Enum(Enum::Const))->val;
 */
abstract class Enum {

  /**
   * The value for the instantiated Enum Object.
   *
   * @var mixed
   */
  protected $protected;

  /**
   * Create an Enumerated instance.
   *
   * @throws InvalidArgumentException
   * @param string or int or object... $value
   */
  public function __construct($value = NULL) {
    $this->protected = new ReadOnlyProtectedMembers(array('val' => NULL));
    $consts = $this->getConstList(TRUE);
    $use_default = $value === NULL;
    if ($use_default) {
      $has_default = isset($consts['__default']);
      if ($has_default) {
        $this->val = $consts['__default'];
      }
      else {
        $class_name = get_class($this);
        throw new InvalidArgumentException("No value provided, and no __default value defined for the class '{$class_name}'.");
      }
    }
    elseif (array_search($value, $consts) !== FALSE) {
      $this->val = $value;
    }
    else {
      $expected = implode(' or ', $this->getConstList(FALSE));
      throw new InvalidArgumentException("Invalid value '$value' provided. Expected $expected.");
    }
  }

  /**
   * Gets the list of the defined constants.
   *
   * @param bool $include_defaultInclude __default in the output?
   *   Include __default in the output?
   *
   * @return array
   */
  public function getConstList($include_default = FALSE) {
    $reflection = new ReflectionClass($this);
    $consts = $reflection->getConstants();
    if ($include_default === FALSE) {
      unset($consts['__default']);
    }
    return $consts;
  }

  public function __get($name) {
    return $this->protected->$name;
  }

  public function __set($name, $value) {
    $this->protected->$name = $value;
  }

  public function __isset($name) {
    return isset($this->protected->$name);
  }

  public function __unset($name) {
    unset($this->protected->$name);
  }

  public function __toString() {
    return $this->val;
  }

  public static function __callStatic($name, $arguments) {
    $consts = $this->getConstList(FALSE);
    if (isset($consts[$name])) {
      return $consts[$name];
    }
    else {
      $class_name = __CLASS__;
      throw new Exception("Constant '$name' is not defined in '$class_name'.");
    }
  }

}

/**
 * Enum() - a function for generating type safe, iterable, singleton enumerations.
 *
 * Enum() will create count($args) + 2 classes in the global namespace. It creates
 * an abstract base class with the name $base_class. This class is given static
 * methods with the names of each of the enum values.
 *
 * A class is created for each enum value that extends $base_class. Each of these
 * classes are singletons and contain a single private field: a string containing
 * the name of the class.
 *
 * Finally, an iterator is created (accessible via the ::iterator() method on
 * $base_class). This method returns a singleton iterator for the enum, usable
 * with foreach.
 *
 * C Example:
 *     typedef enum { Male, Female } Gender;
 *     Gender g = Male;
 *     switch (g) {
 *     case Male: printf("it's a dude.\n"); break;
 *     case Female: printf("it's a lady\n"); break;
 *     }
 *
 * PHP Equivalent:
 *     enum('Gender', array('Male', 'Female'));
 *     $g = Gender::Male();
 *     switch ($g) {
 *     case Gender::Male(): echo 'it\'s a dude', PHP_EOL; break;
 *     case Gender::Female(): echo 'it\'s a lady', PHP_EOL; break;
 *     }
 *
 * You can also extend Enums to more specif values. You may have care makes and
 * would like specific models:
 *
 *     enum('CarType', array('Audi', 'BMW', 'Mercedes'));
 *     enum('AudiType extends CarType', array('A4', SR6'));
 *
 * Looping through will only include the immediate decendents of an enum type
 *
 *     php% foreach (CarType::iterator() as $type) { echo $type, ' '; }
 *     => Audi, 'BMW', 'Mercedes'
 *     php% foreach (AudiType::iterator() as $type) { echo $type, ' '; }
 *     => A4, SR6
 *
 * By default, the values of each of the enums are integers 0 through count - 1,
 * like C's default enumeration values. You can however, specify any scalar value,
 * by setting the key of the array of enum values. For example, if you wanted to
 * use the strings 'm' and 'f' as the values for the respective values in
 * the Gender enumeration, you would call enum with the following parameters:
 *
 *     enum('Gender', array('f' => 'Female', 'm' => 'Male'));
 *
 * Values are limited to numeric and string data. If future versions of PHP
 * support array keys of additional data types, enum() automatically support those
 * as well.
 *
 * You can compare by value or class
 *     $g = Gender::male();
 *     $g === Gender::male();   # true
 *     $g === Male::instance(); # true
 *     $g instanceof Gender;    # true
 *     $g instanceof Male;      # true
 *     $g === Gender::Female()  # false
 *
 * And you can use any of classes generated as type hints in function signatures
 *     class Person {
 *         private $gender;
 *         public function __construct(Gender $g) { $this->gender = $g; }
 *         public function gender() { return $this->gender }
 *     }
 *     function for_guys_only(Male $gender) {
 *         // runtime triggers error on instanceof Female
 *     }
 *
 * @author Jonathan Hohle, http://hohle.net
 *
 * @since 5.June.2008
 *
 * @license MIT License
 *
 * @author Nigel Banks, http://nigelbanks.ca
 *
 * @since 12.10.2011
 *  Changed the function so that type values get 'namespaced' to prevent polution of the global namespace.
 *
 * @param $base_class enumeration name
 * @param $args array of enum values
 *
 * @return nothing
 */
function enum($base_class, array $args) {
  $class_parts = preg_split('/\s+/', $base_class);
  $base_class_name = array_shift($class_parts);
  $enums = array();
  $type_classes = array();

  foreach ($args as $k => $enum) {
    $type_class = $base_class . '_' . $enum;
    // Prevent Collisions.
    $type_classes[] = $type_class;
    $static_method = 'public static function ' . $enum . '() { return ' . $type_class . '::instance(); }';
    $enums[$static_method] = '
            class ' . $type_class . ' extends ' . $base_class_name . '{
                private static $instance = null;
                protected $value = "' . addcslashes($k, '\\') . '";
                private function __construct() {}
                private function __clone() {}
                public static function instance() {
                    if (self::$instance === null) { self::$instance = new self(); }
                    return self::$instance;
                }
            }';
  }

  $base_class_declaration = sprintf('
        abstract class %s {
            protected $value = null;
            %s
            public static function iterator() { return %sIterator::instance(); }
            public function value() { return $this->value; }
            public function __toString() { return (string) $this->value; }
        };', $base_class, implode(PHP_EOL, array_keys($enums)), $base_class_name);

  $iterator_declaration = sprintf('
        class %sIterator implements Iterator {
            private static $instance = null;
            private $values = array(\'%s\');
            private function __construct() {}
            private function __clone() {}
            public static function instance() {
                if (self::$instance === null) { self::$instance = new self(); }
                return self::$instance;
            }
            public function current() {
                $value = current($this->values);
                if ($value === false) { return false; }
                return call_user_func(array(\'%s\', $value));
            }
            public function key() { return key($this->values); }
            public function next() {
                next($this->values);
                return $this->current();
            }
            public function rewind() { return reset($this->values); }
            public function valid() { return (bool) $this->current(); }
        };', $base_class_name, implode('\',\'', $args), $base_class_name);

  eval($base_class_declaration);
  eval($iterator_declaration);
  eval(implode(PHP_EOL, $enums));
}
// @codingStandardsIgnoreEnd
