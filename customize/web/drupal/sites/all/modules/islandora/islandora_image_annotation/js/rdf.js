/*jslint browser: true*/
/*global jQuery, Drupal, RDF, IIAUtils, he*/
/**
 * @file
 * Models RDF triples as plain objects, makes it a bit easier to process.
 */
var RDF;

(function ($) {
  'use strict';

  RDF = {};

  // Older Browser such as IE8 don't provide Object.create, which we require.
  // So we create it if it's missing.
  // @see https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Object/create#Polyfill
  if (!Object.create) {
    Object.create = (function () {
      var Noop = $.extend({}, $.noop);
      return function (o) {
        if (arguments.length !== 1) {
          throw new Error('Object.create implementation only accepts one parameter.');
        }
        Noop.prototype = o;
        return new Noop();
      };
    }());
  }

  /**
   * Adds a number of helper functions to the dump object.
   *
   * @param {object} dump
   *   A dump from the $.rdf objects databank that is focused on a resource.
   *
   * @returns {object}
   *   The dump object with some additional helper fields.
   */
  function addPropertyFunctions(dump) {

    /**
     * Fetches all values of the given field if defined.
     *
     * @param {string} property
     *   A URI that identifies the property to fetch from the dump.
     *
     * @returns {Array}
     *   The values of the property if found, otherwise an empty array.
     */
    function getProperties(property) {
      return (this[property] === undefined) ? [] : this[property];
    }

    /**
     * Fetches the first value of the given field if defined.
     *
     * @param {string} property
     *   A URI that identifies the property to fetch from the dump.
     *
     * @returns {string|null}
     *   The value of the property if found, otherwise null. Assumes the property
     *   is stored as a single value in an array.
     */
    function getProperty(property) {
      var properties = this.getProperties(property);
      return (properties.length > 0) ? properties[0].value : null;
    }

    /**
     * Fetches the first value of the given field and converts it to a number.
     *
     * Assumes base 10.
     *
     * @param {string} property
     *   A URI that identifies the property to fetch from the dump.
     *
     * @returns {number|null}
     *   The value of the property if found, otherwise null. Assumes the property
     *   is stored as a single value in an array.
     */
    function getPropertyAsNumber(property) {
      var value = this.getProperty(property);
      return (value !== null) ? parseInt(value, 10) : null;
    }

    /**
     * Checks if the property Exists.
     *
     * @param {string} property
     *   A URI that identifies the property to fetch from the dump.
     *
     * @returns {boolean}
     *   True if the property exists in the dump, false otherwise.
     */
    function hasProperty(property) {
      return this.getProperties(property).length !== 0;
    }
    $.extend(dump, {
      getProperties: getProperties,
      getProperty: getProperty,
      getPropertyAsNumber: getPropertyAsNumber,
      hasProperty: hasProperty
    });
    return dump;
  }

  /**
   * Gets the given resource from the dump.
   *
   * @param {string} id
   *   The id of the resource to fetch.
   * @param {object} dump
   *   A dump from the $.rdf objects databank that is focus on a resource.
   *
   * @returns {object|null}
   *   A subset of the triples that relate to this resource if found, otherwise
   *   null.
   */
  function getResourceFromDump(id, dump) {
    return dump[id] !== undefined ? addPropertyFunctions(dump[id]) : null;
  }

  /**
   * Instantiate a RDF.Resource object.
   *
   * @param {string} id
   *   The id of the resource to create.
   * @param {object} dump
   *   A dump from the $.rdf objects databank.
   * @constructor
   */
  RDF.Resource = function (id, dump) {
    var me, rdfType, dcTitle, dcFormat, dcExtent, dcTermsRelation,
      dcTermsHasPart, cntChars, exifHeight, exifWidth, dmsImageType,
      dmsColorSpace, imageType, value;

    // Adds helper functions for fetching properties.
    me = getResourceFromDump(id, dump);
    if (me === null) {
      // We sometimes reference resources we don't have any information for, in
      // such cases we can use the only piece of information we have the id.
      this.id = id;
      return;
    }

    rdfType = IIAUtils.getResourceURL('rdf', 'type');
    dcTitle = IIAUtils.getResourceURL('dc', 'title');
    dcFormat = IIAUtils.getResourceURL('dc', 'format');
    dcExtent = IIAUtils.getResourceURL('dc', 'extent');
    dcTermsRelation = IIAUtils.getResourceURL('dcterms', 'relation');
    dcTermsHasPart = IIAUtils.getResourceURL('dcterms', 'hasPart');
    cntChars = IIAUtils.getResourceURL('cnt', 'chars');
    exifHeight = IIAUtils.getResourceURL('exif', 'height');
    exifWidth = IIAUtils.getResourceURL('exif', 'width');
    dmsImageType = IIAUtils.getResourceURL('dms', 'imageType');
    dmsColorSpace = IIAUtils.getResourceURL('dms', 'colorSpace');

    // Not sure if this is right but it was how it worked before, likely a bug,
    // and there should be a separate field for colorSpace and imageType.
    imageType = me.getProperty(dmsColorSpace);
    imageType = (imageType === null) ? me.getProperty(dmsImageType) : imageType;

    // Value might be encoded SVG or other XML values.
    value = me.getProperty(cntChars) || undefined;
    value = value ? he.decode(value) : undefined;
    $.extend(this, {
      id: id,
      types: IIAUtils.unique($.map(me.getProperties(rdfType), function (type) {
        return type.value;
      })),
      title: me.getProperty(dcTitle) || undefined,
      relation: me.getProperty(dcTermsRelation) || undefined,
      hasPart: me.getProperty(dcTermsHasPart) || undefined,
      value: value,
      format: me.getProperty(dcFormat) || undefined,
      width: me.getPropertyAsNumber(exifWidth) || undefined,
      height: me.getPropertyAsNumber(exifHeight) || undefined,
      extent: me.getPropertyAsNumber(dcExtent) || undefined,
      imageType: imageType || undefined
    });
  };

  /**
   * Checks if this resource is a Canvas.
   *
   * @returns {boolean}
   */
  RDF.Resource.prototype.isCanvas = function () {
    var type = IIAUtils.getResourceURL('dms', 'Canvas');
    return $.inArray(type, this.types) !== -1;
  };

  /**
   * Instantiate a RDF.Annotation object.
   *
   * @param {string} id
   *   The identifier of the annotation to create.
   * @param {object} dump
   *   A dump from the $.rdf objects databank. Contains the relevant data needed
   *   to build the Annotation.
   * @constructor
   */
  RDF.Annotation = function (id, dump) {
    var me, dcType, dcTermsHasPart, oaHasBody, oaHasTarget, targets;

    // Call Parent Constructor.
    RDF.Resource.call(this, id, dump);

    // Adds helper functions for fetching properties.
    me = getResourceFromDump(id, dump);

    // Property look up keys.
    dcType = IIAUtils.getResourceURL('dc', 'type');
    dcTermsHasPart = IIAUtils.getResourceURL('dcterms', 'hasPart');
    oaHasBody = IIAUtils.getResourceURL('oa', 'hasBody');
    oaHasTarget = IIAUtils.getResourceURL('oa', 'hasTarget');


    // We can have zero or more targets.
    targets = $.map(me.getProperties(oaHasTarget), function (target) {
      var tid = target.value;
      return new RDF.BodyTarget(tid, dump);
    });

    // Update this objects properties.
    $.extend(this, {
      body: new RDF.BodyTarget(me.getProperty(oaHasBody), dump),
      hasPart: me.getProperty(dcTermsHasPart) || undefined,
      // Type specified by the user, not expected to be 'zone', 'image',
      // 'audio', 'text' or 'comment'
      annotationType: me.getProperty(dcType) || undefined,
      targets: targets,
      finished: 1,
      painted: 0,
      zOrder: 0
    });
  };
  // RDF.Annotation inherits from RDF.Resource.
  RDF.Annotation.prototype = Object.create(RDF.Resource.prototype);
  RDF.Annotation.prototype.constructor = RDF.Annotation;

  /**
   * Gets the target of this annotation body.
   *
   * If a default option is specified it will be used, otherwise the first
   * option will be used. If no options are found, then the annotations, body
   * itself is used.
   *
   * @returns {RDF.BodyTarget}
   */
  RDF.Annotation.prototype.getBodyTarget = function () {
    if (this.body.options.length > 0) {
      // Display choice with best default
      this.body.options.sort(function (a, b) {
        return a.id > b.id ? 1 : -1;
      });
      return this.body.defaultOpt || this.body.options[0];
    }
    return this.body;
  };

  /**
   * Gets the Canvas of this Annotation, or the canvas it's targets belongs to.
   *
   * @returns {RDF.Resource}
   *   The Canvas.
   */
  RDF.Annotation.prototype.getTargetCanvas = function () {
    var canvas = null;
    $.each(this.targets, function (index, target) {
      if (target.isCanvas()) {
        canvas = target;
        return false;
      }
      if (target.isPartOfCanvas()) {
        canvas = target.partOf;
        return false;
      }
    });
    return canvas;
  };

  /**
   * Gets the type of the this annotation.
   *
   * @returns {string|null}
   *   The annotation type, either (image, text, audio, comment, zone), or
   *   null if it could not be determined.
   */
  RDF.Annotation.prototype.getType = function () {
    var annotationType = null;
    // Annotations can have multiple type values, so we first check for the
    // most generic description and then refine if possible.
    if ($.inArray('http://www.w3.org/ns/openannotation/core/Annotation', this.types) !== -1) {
      annotationType = 'comment';
    }
    // Check the more specific Annotations Types.
    $.each(this.types, function (index, type) {
      var types = {
        'http://dms.stanford.edu/ns/ImageAnnotation': 'image',
        'http://dms.stanford.edu/ns/TextAnnotation': 'text',
        'http://dms.stanford.edu/ns/AudioAnnotation': 'audio',
        'http://dms.stanford.edu/ns/CommentAnnotation': 'comment',
        'http://dms.stanford.edu/ns/ZoneAnnotation': 'zone'
      };
      if (types[type] !== undefined) {
        annotationType = types[type];
        return false;
      }
    });
    if (!annotationType) {
      // Check body type
      $.each(this.body.types, function (index, type) {
        var types = {
          'http://purl.org/dc/dcmitype/Image': 'image',
          'http://purl.org/dc/dcmitype/Text': 'text',
          'http://dms.stanford.edu/ns/ImageBody': 'audio',
          'http://dms.stanford.edu/ns/TextBody': 'comment',
          'http://dms.stanford.edu/ns/ImageSegment': 'zone'
        };
        if (types[type] !== undefined) {
          annotationType = types[type];
          return false;
        }
      });
    }
    if (!annotationType) {
      // Check body fragment type
      annotationType = this.body.fragmentType;
    }
    return annotationType;
  };

  /**
   * Gets the fragment identifier from the given URI.
   *
   * @param {string} uri
   *   The URI to search for a fragment identifier.
   *
   * @returns {string|null}
   *   The fragment identifier if found, otherwise null.
   */
  function getFragmentIdentifier(uri) {
    var matches = uri.match(new RegExp('#(.*)$'));
    return (matches && matches[1] !== undefined) ? matches[1] : null;
  }

  /**
   * Gets the fragment type of the given fragment identifier.
   *
   * Used by BodyTarget.
   *
   * @param {string} fragmentIdentifier
   *   The fragment identifier.
   *
   * @returns {string|null}
   *   The type of fragment identifier if found, null otherwise.
   */
  function getFragmentType(fragmentIdentifier) {
    var fragmentType = null;
    $.each({
      rect: /^xywh/,
      xml: /^xywh/,
      audio: /^t=/
    }, function (type, regex) {
      if (regex.test(fragmentIdentifier)) {
        fragmentType = type;
        return false;
      }
    });
    return fragmentType;
  }

  /**
   * Gets the fragment info of the given fragment identifier and type.
   *
   * Used by BodyTarget.
   *
   * @param {string} fragmentIdentifier
   *   The fragment identifier.
   * @param {string} fragmentType
   *   The fragment type, either 'rect', 'xml', or 'audio'.
   */
  function getFragmentInfo(fragmentIdentifier, fragmentType) {
    var matches;
    switch (fragmentType) {
    case 'rect':
      matches = fragmentIdentifier.match(new RegExp('xywh=([0-9]+),([0-9]+),([0-9]+),([0-9]+)'));
      return [
        parseInt(matches[1], 10),
        parseInt(matches[2], 10),
        parseInt(matches[3], 10),
        parseInt(matches[4], 10)
      ];
    case 'xml':
      return IIAUtils.convertXPointerToJQuerySelector(fragmentIdentifier);
    case 'audio':
      matches = fragmentIdentifier.match(new RegExp('t=(npt:)?([0-9.:]+)?,([0-9.:]+)?'));
      return [
        IIAUtils.normalPlayTimeInSeconds(matches[2]),
        IIAUtils.normalPlayTimeInSeconds(matches[3])
      ];
    }
  }

  /**
   * Instantiate a RDF.BodyType object.
   *
   * @param {string} id
   *   The identifier of the annotation to create.
   * @param {object} dump
   *   A dump from the $.rdf objects databank. Contains the relevant data needed
   *   to build the Annotation.
   * @constructor
   */
  RDF.BodyTarget = function (id, dump) {
    var me, dsmOption, dsmDefault, dmsRotation, dctermsIsPartOf, oaConstrains,
      oaConstrainedBy, createBodyTarget, createResource, fragmentIdentifier,
      fragmentType, fragmentInfo, defaultOption, partOf, constraint, options;

    // Call Parent Constructor.
    RDF.Resource.call(this, id, dump);

    // Adds helper functions for fetching properties.
    me = getResourceFromDump(id, dump);

    // Creates a Body Target.
    createBodyTarget = function (property) {
      var bid = me.getProperty(property);
      return bid ? new RDF.BodyTarget(bid, dump) : null;
    };

    // Creates a Resource if it's defined.
    createResource = function (property) {
      var rid = me.getProperty(property);
      return rid ? new RDF.Resource(rid, dump) : null;
    };

    // Property look up keys.
    dsmOption = IIAUtils.getResourceURL('dms', 'option');
    dsmDefault = IIAUtils.getResourceURL('dms', 'default');
    dmsRotation = IIAUtils.getResourceURL('dms', 'rotation');
    dctermsIsPartOf = IIAUtils.getResourceURL('dcterms', 'isPartOf');
    oaConstrains = IIAUtils.getResourceURL('oa', 'constrains');
    oaConstrainedBy = IIAUtils.getResourceURL('oa', 'constrainedBy');

    if (me.hasProperty(dsmOption)) {
      options = $.map(IIAUtils.unique(me.getProperties(dsmOption)), function (option) {
        var oid = option.value;
        return new RDF.BodyTarget(oid, dump);
      });
    }
    defaultOption = createBodyTarget(dsmDefault);
    partOf = createResource(dctermsIsPartOf);
    if (partOf === null) {
      // Check for constraint
      partOf = createResource(oaConstrains);
      constraint = createResource(oaConstrainedBy);
    }

    // Parse the fragment identifier for either rectangle coordinates, an
    // xpointer, or normal play time.
    fragmentIdentifier = getFragmentIdentifier(id);
    if (fragmentIdentifier) {
      fragmentType = getFragmentType(fragmentIdentifier);
      fragmentInfo = getFragmentInfo(fragmentIdentifier, this.fragmentType);
    }

    // Update this objects properties.
    $.extend(this, {
      options: options || undefined,
      fragmentInfo: fragmentInfo || undefined,
      fragmentType: fragmentType || undefined,
      constraint: constraint || undefined,
      partOf: partOf  || undefined,
      defaultOpt: defaultOption || undefined,
      rotation: me.getProperty(dmsRotation) || 0
    });
  };
  // RDF.Annotation inherits from RDF.Resource.
  RDF.BodyTarget.prototype = Object.create(RDF.Resource.prototype);
  RDF.BodyTarget.prototype.constructor = RDF.BodyTarget;

  //noinspection JSUnusedGlobalSymbols
  /**
   * Gets this targets rectangle.
   *
   * @returns {[x,y,width,height]}
   *   An rectangle representing this target.
   *
   * @public
   */
  RDF.BodyTarget.prototype.getRect =  function () {
    var doc, rect, x, y, width, height;
    if (this.constraint !== undefined) {
      // Extract from SVG
      doc = $($.parseXML(this.constraint.value));
      //noinspection JSValidateTypes
      rect = doc.children().first();
      x = rect.getAttribute('x');
      y = rect.getAttribute('y');
      width = rect.getAttribute('width');
      height = rect.getAttribute('height');
      if (!x || !width) {
        return null;
      }
    } else if (this.fragmentInfo !== undefined && this.fragmentInfo.length === 4) {
      x = this.fragmentInfo[0];
      y = this.fragmentInfo[1];
      width = this.fragmentInfo[2];
      height = this.fragmentInfo[3];
    } else {
      return null;
    }
    return [x, y, width, height];
  };

  /**
   * Gets
   * @returns {*}
   */
  RDF.BodyTarget.prototype.getTargetCanvas = function () {
    if (this.isCanvas()) {
      return this;
    }
    if (this.isPartOfCanvas()) {
      return this.partOf;
    }
    return null;
  };


  /**
   * Checks if this target is part of a Canvas.
   *
   * @returns {boolean}
   */
  RDF.BodyTarget.prototype.isPartOfCanvas = function () {
    return this.partOf !== undefined && this.partOf.isCanvas();
  };

  /**
   * Constructs RDF.Annotation objects for the given annotations identifiers.
   *
   * @param {[]} annotations
   *   An array of all the annotation identifiers.
   * @param {object} dump
   *   A dump from the $.rdf objects databank. Contains the relevant data needed
   *   to build the Annotation.
   *
   * @returns {[RDF.Annotation]}
   *   An array of RDF.Annotation objects, representing the annotations given
   *   to this function.
   */
  RDF.createAnnotations = function (annotations, dump) {
    // Foreach annotation id build an annotation object.
    return $.map(annotations, function (id) {
      // Cache that we have build this annotation.
      return new RDF.Annotation(id, dump);
    });
  };

}(jQuery));
