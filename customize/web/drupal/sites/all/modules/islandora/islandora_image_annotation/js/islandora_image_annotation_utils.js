/*jslint browser: true*/
/*global jQuery, Drupal*/
/**
 * @file
 * Adds a number of helper functions used by various image annotation widgets.
 */
var IslandoraImageAnnotationUtils, IIAUtils;
IslandoraImageAnnotationUtils = {};
IIAUtils = IslandoraImageAnnotationUtils;

(function ($) {
  'use strict';

  /**
   * Returns the given array with all duplicate values removed.
   *
   * @param {[]} array
   *   The array to reduce.
   *
   * @returns {[]}
   *   The given array with all duplicates removed.
   */
  IslandoraImageAnnotationUtils.unique = function (array) {
    return $.grep(array, function (value, index) {
      return index === $.inArray(value, array);
    });
  };

  /**
   * Helper function to push a a value onto a object's given property array.
   *
   * Handles the case when the property is an array or not yet defined, but if
   * it is defined and not an array then an exception will be thrown.
   */
  IslandoraImageAnnotationUtils.push = function (object, key, value) {
    if (object[key] === undefined) {
      object[key] = [value];
    } else if (Array.isArray(object[key])) {
      object[key].push(value);
    } else {
      throw new Error('Invalid property ' + key + ' is not an array or undefined');
    }
  };

  /**
   * Creates a full URL to the site with the given path.
   *
   * @param {string} path
   * @returns {string}
   */
  IslandoraImageAnnotationUtils.url = function (path) {
    var origin;
    // Handle IE missing feature.
    if (!window.location.origin) {
      origin = window.location.protocol + "//" + window.location.hostname;
      origin += (window.location.port ? ':' + window.location.port : '');
    } else {
      origin = window.location.origin;
    }
    return origin + Drupal.settings.basePath + path;
  };

  /**
   * Splits the given string into it's urn components if found.
   *
   * Uses a regex and in theory can extract urns out of blocks of arbitrary
   * text.
   *
   * @see https://www.ietf.org/rfc/rfc2141.txt
   *
   * @param {string} urn
   *   A urn, that follows this structure "urn:"<NID>":"<NSS>.
   *
   * @returns {{urn: string, nid: string, nss: string}|null}
   *   Returns the components of the given string that match
   */
  IslandoraImageAnnotationUtils.urnComponents = function (urn) {
    // Optionally matches against the prefix, and the nid.
    var matches = urn.match(/(urn:)?([0-9a-z][0-9a-z\-]{1,31})?:?([0-9a-f]{8}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{4}-[0-9a-f]{12})/i);
    return (matches === null) ? null : {
      toString: function () {
        return matches[0];
      },
      urn: matches[1],
      nid: matches[2],
      nss: matches[3]
    };
  };

  /**
   * Generates a url to the given resource in the given namespace prefix.
   *
   * @param {string} namespacePrefix
   *   The namespace prefix used to fetch the base of the URL.
   * @param {string} [resource]
   *   The path suffix of the URL, in which to append to the URL identified by
   *   the given namespace prefix. It is optional, and will default to the empty
   *   string if not provided.
   *
   * @returns {string}
   *   The URL within the given namespace to the given resource.
   */
  IslandoraImageAnnotationUtils.getResourceURL = function (namespacePrefix, resource) {
    var namespaces, namespace;
    namespaces = {
      dc: 'http://purl.org/dc/elements/1.1/',
      dcterms: 'http://purl.org/dc/terms/',
      dctype: 'http://purl.org/dc/dcmitype/',
      oa: 'http://www.w3.org/ns/openannotation/core/',
      cnt: 'http://www.w3.org/2008/content#',
      dms: 'http://dms.stanford.edu/ns/',
      rdf: 'http://www.w3.org/1999/02/22-rdf-syntax-ns#',
      ore: 'http://www.openarchives.org/ore/terms/',
      exif: 'http://www.w3.org/2003/12/exif/ns#'
    };
    namespace = namespaces[namespacePrefix];
    return (namespace !== undefined) ? namespace + (resource || '') : '';
  };

  /**
   * Given a Normal Play Time (NPT) return the time in seconds elapsed.
   *
   * Expected format [hh:]mm:ss[.xx], without the leading 'npt='.
   *
   * @see http://www.ietf.org/rfc/rfc2326.txt
   * @param {string} time
   *   String containing a time, in the following format [hh:]mm:ss[.xx].
   * @returns {Number}
   *   Returns the time in seconds elapsed.
   */
  IslandoraImageAnnotationUtils.normalPlayTimeInSeconds = function (time) {
    var hours, minutes, seconds, array;
    if (time.indexOf(':') > -1) {
      array = time.split(':');
      seconds = parseFloat(array.pop());
      minutes = parseInt(array.pop(), 10);
      hours = (array.length > 0) ? parseInt(array.pop(), 10) : 0;
      return seconds + (minutes * 60) + (hours * 3600);
    }
    return parseFloat(time);
  };

  /**
   * Converts the given XPointer into a CSS Selector usable by jQuery.
   *
   * @param {string} xpointer
   *   An xpointer string in the form of 'xpointer(...)'.
   *
   * @returns {[string, bool|[number, number]]}
   *   An array where the first element is the CSS selector generated from the
   *   given XPointer. The second is either a bool indicating that all or no
   *   text should be selected, or it is an array representing the range of text
   *   to be selected.
   */
  IslandoraImageAnnotationUtils.convertXPointerToJQuerySelector = function (xpointer) {
    var selector, matches, start, end, text, whiteSpaceRegex, textRegex, slashRegex;
    // Strip whitespace.
    whiteSpaceRegex = new RegExp('^\\s+|\\s+$', 'g');
    selector = xpointer.replace(whiteSpaceRegex, '');
    // Strip xpointer(...).
    matches = selector.match(new RegExp('^#?xpointer\\((.+)\\)$'));
    if (matches) {
      selector = matches[1];
    }
    // Strip whitespace.
    selector = selector.replace(whiteSpaceRegex, '');
    // We want to support string-range(xp, start, end)
    matches = selector.match(new RegExp('^string-range\\((.+),([0-9]+),([0-9]+)\\)'));
    if (matches) {
      selector = matches[1];
      start = parseInt(matches[2], 10);
      end = parseInt(matches[3], 10);
      text = [start, end];
    } else {
      // /text() --> return that we want .text()
      textRegex = new RegExp('/text\\(\\)$');
      text = false;
      matches = selector.match(textRegex);
      if (matches) {
        selector = selector.replace(textRegex, '');
        text = true;
      }
    }
    //strip initial slashes
    slashRegex = new RegExp('^[/]+');
    selector = selector.replace(slashRegex, '');
    // Parent and Descendant axes
    selector = selector.replace(new RegExp('//', 'g'), ' ');
    selector = selector.replace(new RegExp('/', 'g'), ' > ');
    // Ensure quotes in attribute values
    selector = selector.replace(new RegExp('\\[([^\\]]+)=([^\\]"]+)\\]', 'g'), '[$1="$2"]');
    // id(bla) --> #bla
    selector = selector.replace(new RegExp('id\\((.+)\\)', 'g'), '#$1');
    // Final trim whitespace.
    selector = selector.replace(whiteSpaceRegex, '');
    return [selector, text];
  };
}(jQuery));
