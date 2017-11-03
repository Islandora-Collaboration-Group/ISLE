/*jslint browser: true*/
/*global jQuery, Drupal, RDF, IIAUtils*/
/**
 * @file
 * Core class acts as a central store of annotations.
 *
 * Makes use of the jQuery RDF plugin, which we seem to have a later version 1.1
 * in which no one else in the world has...
 * @see https://code.google.com/p/rdfquery/
 */
(function ($) {
  'use strict';

  /**
   * The DOM element that represents the Singleton Instance of this class.
   * @type {string}
   */
  var base = '#islandora-image-annotation';

  /**
   * Initialize the Create Annotation Dialog Box.
   *
   * We can only have one Create Annotation Dialog Box per page.
   */
  Drupal.behaviors.islandoraImageAnnotation = {
    attach: function (context, settings) {
      if (Drupal.IslandoraImageAnnotation[base] === undefined) {
        $(base, document).once('islandoraImageAnnotation', function () {
          Drupal.IslandoraImageAnnotation[base] = new Drupal.IslandoraImageAnnotation(base, settings.islandoraImageAnnotation);
        });
      }
    },
    detach: function (context) {
      $(base).removeClass('islandoraImageAnnotation-processed');
      $(base).removeData();
      $(base).off();
      delete Drupal.IslandoraImageAnnotation[base];
    }
  };

  /**
   * Creates an instances of the Islandora Image Annotation Widget.
   *
   * @param {string} base
   *   The element ID that this class is bound to.
   * @param {object} settings
   *   Drupal.settings for this object widget.
   * @param {string} settings.manifestUrl
   *   The url to the manifest document.
   *
   * @constructor
   */
  Drupal.IslandoraImageAnnotation = function (base, settings) {
    // Private Members.
    var that, manifestUrl;

    // Reference to this object for use in private functions, that might be
    // called from a different scope.
    that = this;

    // Manifest URL.
    manifestUrl = IIAUtils.url('islandora/anno/serve/' + settings.pid + '/Manifest/manifest.xml');

    // Public Members.
    $.extend(this, {
      // Keep track of all external resources we have fetched triples from so that
      // we don't make any more requests than we need to.
      fetched: [],
      // A list of all of the processed annotations, grouped by type.
      annotations: {
        text: {},
        audio: {},
        image: {},
        comment: {},
        zone: {}
      },
      // Acts as a triple-store.
      rdf: $.rdf({
        base: 'http://localhost/EmicShared/impl/',
        namespaces: {
          dc: 'http://purl.org/dc/elements/1.1/',
          dcterms: 'http://purl.org/dc/terms/',
          dctype: 'http://purl.org/dc/dcmitype/',
          oa: 'http://www.w3.org/ns/openannotation/core/',
          cnt: 'http://www.w3.org/2008/content#',
          dms: 'http://dms.stanford.edu/ns/',
          rdf: 'http://www.w3.org/1999/02/22-rdf-syntax-ns#',
          ore: 'http://www.openarchives.org/ore/terms/',
          exif: 'http://www.w3.org/2003/12/exif/ns#'
        }
      })
    });

    /**
     * Take RDF Resource and convert its RDF list to a javascript array.
     *
     * @see http://www.w3.org/TR/rdf-schema/#ch_collectionvocab
     *
     * @param resource
     *   An $.rdf object that "contains" the list we will fetch.
     *
     * @returns {Array}
     *   The RDF list as a javascript array of $.rdf resources.
     */
    function rdfListToArray(resource) {
      var list, firsts, rests, next;
      list = [];
      firsts = {};
      rests = {};

      // Fetch all collection style triples, even if we are not interested in
      // them.
      that.rdf.where('?what rdf:first ?first')
        .where('?what rdf:rest ?rest')
        .each(function () {
          firsts[this.what.value] = this.first.value;
          rests[this.what.value] = this.rest.value;
        });

      // Start the list from the given resources first list item.
      list.push(firsts[resource]);
      next = rests[resource];

      // Iterate through linked list.
      while (next) {
        if (firsts[next] !== undefined) {
          list.push(firsts[next]);
        }
        next = rests[next];
      }
      return list;
    }

    /**
     * Gets the URI to the given Aggregation's Resource Map.
     *
     * @see http://www.openarchives.org/ore/terms/
     * @see https://code.google.com/p/rdfquery/
     *
     * @param {object|string} aggregation
     *   Either the URI representing the aggregation or an $.rdf object
     *   representing the aggregation.
     *
     * @returns {string}
     *   The URI of the Resource Map if found.
     */
    function getAggregationResourceMapURI(aggregation) {
      var resourceMapURI = '';
      that.rdf.reset();
      that.rdf.where('<' + aggregation.toString() + '> ore:isDescribedBy ?resourceMap')
        .where('?resourceMap dc:format "application/rdf+xml"')
        .each(function () {
          resourceMapURI = this.resourceMap.value.toString();
        });
      return resourceMapURI;
    }

    /**
     * Given a Resource Map, fetch the Aggregation's resources and their order.
     *
     * @param {object|string} resourceMap
     *   Either the URI representing the Resource Map or an $.rdf object
     *   representing the Resource Map.
     *
     * @returns {object}
     *   An associative array in which the key's denote the Aggregation's
     *   Resources, and the value is the order in which they were defined.
     */
    function getOrderOfAggregationResources(resourceMap) {
      var zorder = {}, aggregation = null;
      that.rdf.reset();
      that.rdf.where('<' + resourceMap + '> ore:describes ?aggregation')
        .each(function () {
          aggregation = this.aggregation.value.toString();
        });
      if (aggregation !== null) {
        $.each(rdfListToArray(aggregation), function (index, value) {
          zorder[value] = parseInt(index, 10) + 1;
        });
      }
      return zorder;
    }

    /**
     * Gets all of the annotation identifiers in the databank.
     *
     * @returns {[string]}
     *   The identifiers of the annotations.
     */
    function getAnnotationIdentifiers() {
      var identifiers = [];
      that.rdf.reset();
      that.rdf.where('?annotation rdf:type <http://www.w3.org/ns/openannotation/core/Annotation>')
        .each(function () {
          var annotation = this.annotation.value.toString();
          identifiers.push(annotation);
        });
      return identifiers;
    }

    /**
     * Gets annotation from the given identifier.
     *
     * @param {string} identifier
     *   The identifier used to look up the annotation object
     *
     * @returns {object|null}
     *   The annotation object if found, null otherwise.
     */
    this.getAnnotation = function (identifier) {
      var result = null;
      $.each(that.annotations, function (type, typeAnnotations) {
        $.each(typeAnnotations, function (canvas, canvasAnnotations) {
          $.each(canvasAnnotations, function (index, annotation) {
            if (annotation.id === identifier) {
              result = annotation;
              return false;
            }
          });
          return result === null;
        });
        return result === null;
      });
      return result;
    };

    /**
     * Gets all the annotations that match the given type..
     *
     * @param {string} type
     *   The type of annotations either 'image', 'text', 'audio', 'zone', or
     *   'comment'.
     *
     * @returns {[object]}
     *   The annotation objects if found, otherwise an empty array.
     */
    this.getAnnotationsByType = function (type) {
      var results = [];
      if (that.annotations[type] !== undefined) {
        $.each(that.annotations[type], function (canvas, canvasAnnotations) {
          $.each(canvasAnnotations, function (index, annotation) {
            results.push(annotation);
          });
        });
      }
      return results;
    };

    /**
     * Deletes the given annotation's triples and it's related triples.
     *
     * @param {string} identifier
     *   The identifier used to look up the annotation object
     */
    function deleteAnnotationTriples(identifier) {
      var body, target, constrainedBy;
      // Remove the annotation's body's triples.
      body = IIAUtils.getResourceURL('oa', 'hasBody');
      that.rdf.where('<' + identifier + '> <' + body + '> ?subject')
        .where('?subject ?predicate ?object')
        .remove('?subject ?predicate ?object');
      // Remove the annotation's target's triples, including their constraints..
      target = IIAUtils.getResourceURL('oa', 'hasTarget');
      constrainedBy = IIAUtils.getResourceURL('oa', 'constrainedBy');
      // Remove target constraint's triples.
      that.rdf.where('<' + identifier + '> <' + target + '> ?target')
        .where('?target <' + constrainedBy + '> ?subject')
        .where('?subject ?predicate ?object')
        .remove('?subject ?predicate ?object');
      // Remove target triples.
      that.rdf.where('<' + identifier + '> <' + target + '> ?subject')
        .where('?subject ?predicate ?object')
        .remove('?subject ?predicate ?object');
      // Remove Immediate Relations to the annotation.
      that.rdf.where('<' + identifier + '> ?predicate ?object')
        .remove('<' + identifier + '> ?predicate ?object');
    }

    /**
     * Deletes the given annotation.
     *
     * @param {string} identifier
     *   The identifier used to look up the annotation object
     */
    this.deleteAnnotation = function (identifier) {
      var found = null;
      $.each(that.annotations, function (type, typeAnnotations) {
        $.each(typeAnnotations, function (canvas, canvasAnnotations) {
          $.each(canvasAnnotations, function (index, annotation) {
            if (annotation.id === identifier) {
              found = {
                type: type,
                canvas: canvas,
                index: index,
                annotation: annotation
              };
            }
            return found === null;
          });
          return found === null;
        });
        return found === null;
      });
      if (found) {
        deleteAnnotationTriples(identifier);
        that.annotations[found.type][found.canvas].splice(found.index, 1);
        that.constructor.trigger('deleteAnnotation', found.annotation);
      }
    };

    /**
     * Gets a annotations of the give type if given otherwise all annotations.
     *
     * @returns {[RDF.Annotation]}
     *   Gets the annotations for the given.
     */
    function getUnprocessedAnnotations() {
      var identifiers = getAnnotationIdentifiers();
      // Filter out the annotations which have already been processed.
      identifiers = $.grep(identifiers, function (id) {
        return that.getAnnotation(id) === null;
      });
      return RDF.createAnnotations(IIAUtils.unique(identifiers), that.rdf.databank.dump());
    }

    /**
     * Process the annotations associated with the given aggregation.
     *
     * @param {string} uri
     *   The Resource Map URI for the annotations we are to process.
     */
    function processAnnotations(uri) {
      var unprocessedAnnotations, xmlFiles, zorder;

      unprocessedAnnotations = getUnprocessedAnnotations();
      zorder = (uri === undefined) ? {} : getOrderOfAggregationResources(uri);

      xmlFiles = {};
      $.each(unprocessedAnnotations, function (index, annotation) {
        var type, pid, canvas, canvasId;
        type = annotation.getType();

        // We can't process any annotations that don't have a type.
        if (!type) {
          return;
        }

        // Use '*' as a placeholder to say it's not annotating any specific
        // canvas, and is likely not annotate anything at all. We need to still
        // process these annotations though.
        canvas = annotation.getTargetCanvas();
        canvasId = (canvas && canvas.id) || '*';

        // Update the zOrder of the annotation, as defined by the annotation's
        // manifest.
        if (zorder && zorder[annotation.id] !== undefined) {
          annotation.zOrder = zorder[annotation.id];
        }

        // Type specific features:
        // 1) Fetch XML docs (eg TEI transcription).
        if (annotation.body !== undefined) {
          if (annotation.body.fragmentType === 'xml') {
            pid = annotation.body.partOf.id;
            // Need to fetch XML before painting.
            xmlFiles[pid] = (xmlFiles[pid] !== undefined) ? xmlFiles[pid] : [];
            xmlFiles[pid].push([annotation, annotation.body]);
            // Delay processed event until all document(s) are fetched.
            annotation.finished -= 1;
          }
          // 2) Fetch SVG doc if external.
          if (annotation.body.constraint !== undefined && !annotation.body.constraint.value) {
            pid = annotation.body.constraint.id;
            // Need to fetch XML before painting.
            xmlFiles[pid] = (xmlFiles[pid] !== undefined) ? xmlFiles[pid] : [];
            xmlFiles[pid].push([annotation, annotation.body.constraint]);
            // Delay processed event until all document(s) are fetched.
            annotation.finished -= 1;
          }
        }

        // Cache all the processed annotations and group them by type and
        // canvas.
        IIAUtils.push(that.annotations[type], canvasId, annotation);

        // If this annotation requires no further processing notify the world
        // it's been processed.
        if (annotation.finished) {
          that.constructor.trigger('processedAnnotation', annotation);
        }
      });

      // And launch AJAX queries for any external XML docs, each uri is based on
      // the annotations PID.
      $.each(xmlFiles, function (uri, annotations) {
        $.ajax({
          url: uri,
          dataType: 'xml',
          success: function (data) {
            // Update the body target value if it's found elsewhere.
            $.each(annotations, function (index, info) {
              var annotation, bodyTarget, selector, textSelection, text;
              annotation = info[0];
              bodyTarget = info[1];
              // Fetch the text content of the body.
              if (bodyTarget.fragmentType === 'xml') {
                selector = bodyTarget.fragmentInfo[0];
                textSelection = bodyTarget.fragmentInfo[1];
                if (textSelection) {
                  text = $(data).find(selector).text().substring(textSelection[0], textSelection[1]);
                } else {
                  text = $(data).find(selector); // leave it up to Paint to deal with.
                }
              } else {
                text = data;
              }
              bodyTarget.value = text;
              annotation.finished += 1;
              // If this annotation requires no further processing notify the
              // world it's been processed.
              if (annotation.finished) {
                that.constructor.trigger('processedAnnotation', annotation);
              }
            });
          },
          error: function () {
            console.log('Can not fetch data from ' + uri);
          }
        });
      });
    }

    /**
     * Processes the Given Sequence and extract a list of Canvases.
     *
     * @param {string} uri
     *   The Resource Map URI for the sequence we are to process.
     */
    function processSequence(uri) {
      var sequence;
      // Fetch the Aggregation for the Resource Map.
      that.rdf.where('?sequence rdf:type dms:Sequence')
        .where('?sequence ore:isDescribedBy <' + uri + '>')
        .each(function () {
          sequence = this.sequence.value;
        });
      // If we don't find one that matches the given Resource map, just grab the
      // first one.
      if (sequence === undefined) {
        that.rdf.where('?sequence rdf:type dms:Sequence')
          .each(function () {
            sequence = this.sequence.value;
          });
      }
      if (sequence !== undefined) {
        $.each(rdfListToArray(sequence), function (index, sequence) {
          var id, dump;
          id = sequence.toString();
          dump = that.rdf.databank.dump();
          that.constructor.trigger('processedSequence', new RDF.Resource(id, dump));
        });
      }
    }

    /**
     * Pull rdf/xml file and parse to triples with rdfQuery.
     *
     * @param {string} url
     *   The uri to fetch the triples from.
     * @param {function} [callback]
     *   The callback to execute upon success.
     */
    this.fetchTriples = function (url, callback) {
      // If no callback is provided assume processAnnotations.
      var finished = callback || processAnnotations;
      if ($.inArray(that.fetched, url) !== -1) {
        return;
      }
      // Store that we've fetched this URI to prevent duplicate requests.
      that.fetched.push(url);
      $.ajax({
        type: 'GET',
        async: true,
        url: url,
        success: function (data, status, xhr) {
          var contentType, triples;
          contentType = xhr.getResponseHeader("content-type") || "";
          try {
            if (contentType === 'application/rdf+xml' || contentType === 'text/xml') {
              that.rdf.databank.load(data);
            } else {
              triples = $(data).rdf().databank.triples();
              $.each(triples, function (index, triple) {
                that.rdf.databank.add(triple);
              });
            }
            finished(url);
          } catch (exception) {
            console.log('Failed to fetch triples from: ' + url);
            console.log(exception);
            console.log(exception.stack);
          }
        },
        error:  function (XMLHttpRequest, status, errorThrown) {
          console.log('Can not fetch any data from ' + url + ': ' + errorThrown);
        }
      });
    };

    /**
     * Processes the Manifest of other Aggregations of Resources.
     *
     * This function is responsible for populating this object with data about
     * the image and it's annotations.
     *
     * At the moment we only support / create image and comment annotations.
     *
     * @param {string} uri
     *   The Resource Map URI of the Manifest.
     */
    function processManifest(uri) {
      var sequences, aggregations, storeAggregation;

      // Build up the Sequences, typically only one NormalSequence.
      sequences = [];
      that.rdf.where('<' + uri + '> ore:describes ?aggregation')
        .where('?aggregation ore:aggregates ?sequence')
        .where('?sequence rdf:type dms:Sequence')
        .each(function () {
          sequences.push(this.sequence.value);
        });
      that.rdf.reset();

      // Fetch the Image Annotations, Text Annotations, Audio Annotations,
      // Zone Annotations, and Comment Annotations for this Aggregation.
      aggregations = {
        text: {
          '*': []
        },
        audio: {
          '*': []
        },
        image: {
          '*': []
        },
        comment: {
          '*': []
        },
        zone: {
          '*': []
        }
      };
      // Store the aggregation.
      storeAggregation = function (type) {
        return function () {
          var canvas, sequence;
          // '*' stands for those annotations which are not associated with a
          // specific canvas.
          canvas = (this.canvas === undefined) ? '*' : this.canvas.value;
          sequence = this.sequence.value;
          IIAUtils.push(aggregations[type], canvas, sequence);
        };
      };
      // @todo At some point we could make an aggregate for comments to reduce
      // the number of requests we are currently making to fetch each one.
      that.rdf.where('<' + uri + '> ore:describes ?aggregation')
        .where('?aggregation ore:aggregates ?sequence')
        .optional('?sequence dms:forCanvas ?canvas')
        .where('?sequence rdf:type dms:ImageAnnotationList')
        .each(storeAggregation('image')).end()
        .where('?sequence rdf:type dms:TextAnnotationList')
        .each(storeAggregation('text')).end()
        .where('?sequence rdf:type dms:AudioAnnotationList')
        .each(storeAggregation('audio')).end()
        .where('?sequence rdf:type dms:ZoneAnnotationList')
        .each(storeAggregation('zone')).end()
        .where('?sequence rdf:type dms:CommentAnnotationList')
        .each(storeAggregation('comment')).end();
      that.rdf.reset();

      // Process all of the Annotation Aggregations, after the sequence has been
      // processed.
      that.constructor.on('processedSequence', function () {
        $.each(aggregations, function (type, typeAggregations) {
          $.each(typeAggregations, function (canvas, canvasAggreations) {
            $.each(canvasAggreations, function (index, aggregation) {
              that.fetchTriples(getAggregationResourceMapURI(aggregation), processAnnotations);
            });
          });
        });
      });
      // Process the Sequences, typically there will be only one NormalSequence.
      $.each(sequences, function (index, sequence) {
        that.fetchTriples(getAggregationResourceMapURI(sequence), processSequence);
      });
    }

    // Manifest initialization, this will kick off the process of load all the
    // required data to render the image and it's annotations.
    that.fetchTriples(manifestUrl, processManifest);
  };

  /**
   * Execute all handlers and behaviors attached to the matched elements for the
   * given event type.
   *
   * @param {string} event
   *   A string containing a JavaScript event type, such as click or submit.
   * @param {Array|Object} extraParameters
   *   Additional parameters to pass along to the event handler.
   */
  Drupal.IslandoraImageAnnotation.trigger = function (event, extraParameters) {
    $(base).trigger(event, extraParameters);
  };

  /**
   * Registered callback for the given event.
   */
  Drupal.IslandoraImageAnnotation.on = function (event, callback) {
    $(base).on(event, callback);
  };

  /**
   * Unregistered callback for the given event.
   */
  Drupal.IslandoraImageAnnotation.off = function (event, callback) {
    $(base).off(event, callback);
  };

  /**
   * Gets the global singleton of this class.
   *
   * @return {IslandoraImageAnnotation}
   */
  Drupal.IslandoraImageAnnotation.getInstance = function () {
    return Drupal.IslandoraImageAnnotation[base];
  };

}(jQuery));
