<?php

/**
 * @file
 * This file has functions for writing the XACML class datastructures as XML.
 */

include_once 'constants.inc';

/**
 * Class for writing XACML editor datastructures out to XML.
 */
class XacmlWriter {

  /**
   * Takes a $xacml array and returns a DOMDocument containing the XACML xml.
   *
   * @param array $xacml
   *   The XACML rules array to be populated by this function.
   *
   * @return DomDocument
   *   A DomDocument containing our constructed XACML policy.
   */
  public static function toDOM($xacml) {
    $dom = new DomDocument("1.0", "UTF-8");

    XacmlWriter::createRootElement($dom, $xacml);
    XacmlWriter::createTarget($dom, $xacml);
    XacmlWriter::createRules($dom, $xacml);

    return $dom;
  }

  /**
   * Takes a $xacml datastructure and returns a string containing the XACML xml.
   *
   * @param array $xacml
   *   An array representing an XACML datastructure.
   * @param bool $pretty_print
   *   Determines if the output XML is formatted. Default: FALSE.
   *
   * @return XML
   *   String containing the XACML as XML.
   */
  public static function toXML($xacml, $pretty_print = FALSE) {
    $dom = XacmlWriter::toDOM($xacml);
    $dom->formatOutput = $pretty_print;

    return $dom->saveXML();
  }

  /**
   * Creates the root tag for the base XACML document.
   */
  protected static function createRootElement(&$dom, $xacml) {
    $root = $dom->createElementNS(XacmlConstants::XACML, 'Policy');
    $root->setAttribute('PolicyId', 'islandora-xacml-editor-v1');
    $root->setAttribute('RuleCombiningAlgId', $xacml['RuleCombiningAlgId']);
    $root->setAttributeNS(XacmlConstants::XMLNS, 'xmlns', XacmlConstants::XACML);
    $root->setAttributeNS(XacmlConstants::XMLNS, 'xmlns:xsi', XacmlConstants::XSI);

    $dom->appendChild($root);
  }

  /**
   * Creates the empty skeleton document that every XACML document should have.
   *
   * @note
   * This is the section that says this rule applies to everything.
   * We do all the narrowing of the scope for rules within the rules themselves.
   *
   * @code
   *  <Target>
   *    <Subjects>
   *      <AnySubject/>
   *    </Subjects>
   *    <Resources>
   *      <AnyResource/>
   *    </Resources>
   *    <Actions>
   *      <AnyAction/>
   *    </Actions>
   *  </Target>
   * @endcode
   */
  protected static function createTarget(&$dom, $xacml) {
    $target = $dom->createElementNS(XacmlConstants::XACML, 'Target');

    $subject = $dom->createElementNS(XacmlConstants::XACML, 'Subjects');
    $subject->appendChild($dom->createElementNS(XacmlConstants::XACML, 'AnySubject'));
    $target->appendChild($subject);

    $resource = $dom->createElementNS(XacmlConstants::XACML, 'Resources');
    $resource->appendChild($dom->createElementNS(XacmlConstants::XACML, 'AnyResource'));
    $target->appendChild($resource);

    $action = $dom->createElementNS(XacmlConstants::XACML, 'Actions');
    $action->appendChild($dom->createElementNS(XacmlConstants::XACML, 'AnyAction'));
    $target->appendChild($action);

    $dom->documentElement->appendChild($target);
  }

  /**
   * Create all of the <Rule> tags.
   */
  protected static function createRules(&$dom, $xacml) {
    foreach ($xacml['rules'] as $rule) {
      XacmlWriter::createRule($dom, $rule);
    }
  }

  /**
   * Creates each individual <Rule> tag.
   */
  protected static function createRule(&$dom, $rule) {
    $root = $dom->createElementNS(XacmlConstants::XACML, "Rule");
    $root->setAttribute('RuleId', $rule['ruleid']);
    $root->setAttribute('Effect', $rule['effect']);

    XacmlWriter::createRuleTarget($dom, $root, $rule);
    XacmlWriter::createRuleCondition($dom, $root, $rule);

    $dom->documentElement->appendChild($root);
  }

  /**
   * Creates the target for a <Rule> to apply to.
   */
  protected static function createRuleTarget(&$dom, &$root, $rule) {
    $target = $dom->createElementNS(XacmlConstants::XACML, "Target");

    XacmlWriter::createRuleTargetSubjects($dom, $target, $rule);
    XacmlWriter::createRuleTargetResources($dom, $target, $rule);
    XacmlWriter::createRuleTargetActions($dom, $target, $rule);

    $root->appendChild($target);
  }

  /**
   * Creates the target subject for a <Rule>.
   */
  protected static function createRuleTargetSubjects(&$dom, &$target, $rule) {
    $subjects = $dom->createElementNS(XacmlConstants::XACML, "Subjects");
    $subjects->appendChild($dom->createElementNS(XacmlConstants::XACML, "AnySubject"));
    $target->appendChild($subjects);
  }

  /**
   * Creates target actions for a <Rule>.
   */
  protected static function createRuleTargetActions(&$dom, &$target, $rule) {
    $actions = $dom->createElementNS(XacmlConstants::XACML, "Actions");
    if (!empty($rule['methods'])) {
      foreach ($rule['methods'] as $method) {
        XacmlWriter::createRuleTargetAction($dom, $actions, $method);
      }
    }
    else {
      $actions->appendChild($dom->createElementNS(XacmlConstants::XACML, "AnyAction"));
    }

    $target->appendChild($actions);
  }

  /**
   * Creates an individual target action for a <Rule>.
   */
  protected static function createRuleTargetAction(&$dom, &$actions, $method) {
    $action = $dom->createElementNS(XacmlConstants::XACML, 'Action');
    $action_match = $dom->createElementNS(XacmlConstants::XACML, 'ActionMatch');
    $action_match->setAttribute('MatchId', XacmlConstants::STRINGEQUAL);
    $action->appendChild($action_match);

    if ($method == 'api-a' || $method == 'api-m') {
      $attributevalue = "urn:fedora:names:fedora:2.1:action:$method";
      $attributeid = 'urn:fedora:names:fedora:2.1:action:api';
    }
    else {
      $attributevalue = "urn:fedora:names:fedora:2.1:action:id-$method";
      $attributeid = "urn:fedora:names:fedora:2.1:action:id";
    }

    $attribute_value = $dom->createElementNS(XacmlConstants::XACML, "AttributeValue", $attributevalue);
    $attribute_value->setAttribute("DataType", "http://www.w3.org/2001/XMLSchema#string");
    $action_attribute_designator = $dom->createElementNS(XacmlConstants::XACML, "ActionAttributeDesignator");
    $action_attribute_designator->setAttribute("AttributeId", $attributeid);
    $action_attribute_designator->setAttribute("DataType", "http://www.w3.org/2001/XMLSchema#string");
    $action_match->appendChild($attribute_value);
    $action_match->appendChild($action_attribute_designator);
    $actions->appendChild($action);
  }

  /**
   * Creates target resources for a <Rule>.
   */
  protected static function createRuleTargetResources(&$dom, &$target, $rule) {
    $resources = $dom->createElementNS(XacmlConstants::XACML, "Resources");

    if (empty($rule['mimes']) && empty($rule['dsids']) && empty($rule['dsidregexs']) && empty($rule['mimeregexs'])) {
      $resources->appendChild($dom->createElementNS(XacmlConstants::XACML, "AnyResource"));
    }
    else {
      foreach ($rule['mimes'] as $mime) {
        XacmlWriter::createRuleTargetResource($dom, $resources, $mime, 'mime');
      }
      foreach ($rule['dsids'] as $dsid) {
        XacmlWriter::createRuleTargetResource($dom, $resources, $dsid, 'dsid');
      }
      foreach ($rule['dsidregexs'] as $dsidregex) {
        XacmlWriter::createRuleTargetResource($dom, $resources, $dsidregex, 'dsidregex');
      }
      foreach ($rule['mimeregexs'] as $mimeregex) {
        XacmlWriter::createRuleTargetResource($dom, $resources, $mimeregex, 'mimeregex');
      }
    }

    $target->appendChild($resources);
  }

  /**
   * Creates an individual target resource for a <Rule>.
   */
  protected static function createRuleTargetResource(&$dom, &$resources, $name, $type) {
    $resource = $dom->createElementNS(XacmlConstants::XACML, 'Resource');
    $resource_match = $dom->createElementNS(XacmlConstants::XACML, 'ResourceMatch');

    if ($type == 'mime' || $type == 'dsid') {
      $resource_match->setAttribute('MatchId', XacmlConstants::STRINGEQUAL);
    }
    else {
      $resource_match->setAttribute('MatchId', XacmlConstants::REGEXEQUAL);
    }
    $resource->appendChild($resource_match);

    $attribute_value = $dom->createElementNS(XacmlConstants::XACML, 'AttributeValue', $name);
    $attribute_value->setAttribute('DataType', "http://www.w3.org/2001/XMLSchema#string");

    $resource_attribute_designator = $dom->createElementNS(XacmlConstants::XACML, 'ResourceAttributeDesignator');
    $resource_attribute_designator->setAttribute("DataType", "http://www.w3.org/2001/XMLSchema#string");

    switch ($type) {
      case 'mime':
      case 'mimeregex':
        $resource_attribute_designator->setAttribute("AttributeId", "urn:fedora:names:fedora:2.1:resource:datastream:mimeType");
        break;

      case 'dsid':
      case 'dsidregex':
        $resource_attribute_designator->setAttribute("AttributeId", "urn:fedora:names:fedora:2.1:resource:datastream:id");
        break;

      default:
        return FALSE;
    }

    $resource_match->appendChild($attribute_value);
    $resource_match->appendChild($resource_attribute_designator);

    $resources->appendChild($resource);
  }

  /**
   * Creates a condition for a <Rule>.
   */
  protected static function createRuleCondition(&$dom, &$target, $rule) {
    $condition = $dom->createElementNS(XacmlConstants::XACML, "Condition");
    $condition->setAttribute("FunctionId", "urn:oasis:names:tc:xacml:1.0:function:not");

    if (!empty($rule['users'])) {
      $users = XacmlWriter::createRuleConditionApply($dom, $rule['users'], 'user');
    }
    if (!empty($rule['roles'])) {
      $roles = XacmlWriter::createRuleConditionApply($dom, $rule['roles'], 'role');
    }

    if (isset($users) && isset($roles)) {
      $apply = $dom->createElementNS(XacmlConstants::XACML, "Apply");
      $apply->setAttribute("FunctionId", "urn:oasis:names:tc:xacml:1.0:function:or");
      $condition->appendChild($apply);
      $apply->appendChild($users);
      $apply->appendChild($roles);
    }
    elseif (isset($users)) {
      $condition->appendChild($users);
    }
    elseif (isset($roles)) {
      $condition->appendChild($roles);
    }
    else {
      return NULL;
    }

    $target->appendChild($condition);
  }

  /**
   * Applies a condition to a <Rule>.
   */
  protected static function createRuleConditionApply(&$dom, $attributes, $type) {
    $apply = $dom->createElementNS(XacmlConstants::XACML, 'Apply');
    $apply->setAttribute("FunctionId", "urn:oasis:names:tc:xacml:1.0:function:string-at-least-one-member-of");

    $subject_attribute_designator = $dom->createElementNS(XacmlConstants::XACML, 'SubjectAttributeDesignator');
    $subject_attribute_designator->setAttribute("DataType", "http://www.w3.org/2001/XMLSchema#string");
    $subject_attribute_designator->setAttribute("MustBePresent", "false");

    switch ($type) {
      case 'role':
        $subject_attribute_designator->setAttribute('AttributeId', "fedoraRole");
        break;

      case 'user':
        $subject_attribute_designator->setAttribute('AttributeId', "urn:fedora:names:fedora:2.1:subject:loginId");
        break;

      default:
        return NULL;
    }

    $apply->appendChild($subject_attribute_designator);

    $string_bag = $dom->createElementNS(XacmlConstants::XACML, "Apply");
    $string_bag->setAttribute("FunctionId", "urn:oasis:names:tc:xacml:1.0:function:string-bag");
    $apply->appendChild($string_bag);

    foreach ($attributes as $attribute) {
      $attrib = $dom->createElementNS(XacmlConstants::XACML, "AttributeValue", $attribute);
      $attrib->setAttribute("DataType", "http://www.w3.org/2001/XMLSchema#string");
      $string_bag->appendChild($attrib);
    }

    return $apply;
  }
}
