<?php
/**
 * @file
 * This file contains a class that specifies some constants that are used
 * through the Islandora Xacml Module.
 */

/**
 * Helper class specifying some useful constants.
 */
class XacmlConstants {
  const XACML = "urn:oasis:names:tc:xacml:1.0:policy";
  const MIME = "urn:fedora:names:fedora:2.1:resource:datastream:mimeType";
  const DSID = "urn:fedora:names:fedora:2.1:resource:datastream:id";
  const ONEMEMBEROF = "urn:oasis:names:tc:xacml:1.0:function:string-at-least-one-member-of";
  const STRINGEQUAL = "urn:oasis:names:tc:xacml:1.0:function:string-equal";
  const LOGINID = "urn:fedora:names:fedora:2.1:subject:loginId";
  const XSI = 'http://www.w3.org/2001/XMLSchema-instance';
  const XMLNS = 'http://www.w3.org/2000/xmlns/';
  const REGEXEQUAL = 'urn:oasis:names:tc:xacml:1.0:function:regexp-string-match';
}
