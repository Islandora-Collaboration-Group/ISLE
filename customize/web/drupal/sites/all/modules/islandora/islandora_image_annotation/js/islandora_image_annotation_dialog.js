/*jslint browser: true, devel:true*/
/*global jQuery, Drupal, UUID, IIAUtils*/
/**
 * @file
 * Defines the Islandora Image Annotation widget and it's Drupal behaviour.
 *
 * Requires jQuery 1.7+.
 * Requires jQuery UI.
 */
(function ($) {
  'use strict';
  /**
   * The DOM element that represents the Singleton Instance of this class.
   * @type {string}
   */
  var base = '#islandora-image-annotation-dialog';

  /**
   * Initialize the Create Annotation Dialog Box.
   *
   * We can only have one Create Annotation Dialog Box per page.
   */
  Drupal.behaviors.islandoraImageAnnotationDialog = {
    attach: function (context, settings) {
      $(base, context).once('islandoraImageAnnotationDialog', function () {
        Drupal.IslandoraImageAnnotationDialog[base] = new Drupal.IslandoraImageAnnotationDialog(base, settings.islandoraImageAnnotationDialog);
      });
    },
    detach: function (context) {
      $(base).removeClass('islandoraImageAnnotationDialog-processed');
      $(base).removeData();
      $(base).off();
      delete Drupal.IslandoraImageAnnotationDialog[base];
    }
  };

  /**
   * Instantiate the Create Annotation Dialog Box.
   *
   * @param {string} base
   *   The id of the html element in which to bind the dialog box to.
   *
   * @param {object} settings
   *   The Drupal.settings variable for this widget.
   *   - canSelectColour: boolean, indicates the user can choose the color of
   *     the annotation.
   *   - canLinkEntity: boolean, indicates the user can link an existing entity
   *     to the annotation.
   *
   * @constructor
   */
  Drupal.IslandoraImageAnnotationDialog = function (base, settings) {
    // Private Variables.
    var that, $dialog;
    that = this;
    $dialog = $(base);
    // Public Variables.
    this.defaultDialogProperties = $.extend({
      dialogClass: settings.dialogClass,
      height: 680,
      width: 460,
      resizable: true,
      closeOnEscape: true,
      modal: false,
      position: {
        my: "left top",
        at: "left bottom",
        of: '#islandora-image-annotation'
      }
    }, settings.dialog);

    /**
     * Fetches a random color from the 'svgAreaColors' table.
     *
     * @returns {string}
     */
    function getRandomColour() {
      var svgAreaColors = [
        '#FF0000', '#FF6600', '#FF9400', '#FEC500', '#FFFF00', '#8CC700',
        '#0FAD00', '#00A3C7', '#0064B5', '#0010A5', '#6300A5', '#C5007C'];
      return svgAreaColors[Math.floor(Math.random() * svgAreaColors.length)];
    }

    /**
     * Takes a Date object and converts it into an ISO formatted date.
     *
     * @param {Date} date
     *
     * @returns {string}
     */
    function isoDate(date) {
      var dt = date.getUTCFullYear().toString(),
        m = (date.getUTCMonth() + 1).toString(),
        dy = date.getUTCDate().toString(),
        hr = date.getUTCHours().toString(),
        mn = date.getUTCMinutes().toString(),
        sc = date.getUTCSeconds().toString();
      m = m.length === 1 ? '0' + m : m;
      dy = dy.length === 1 ? '0' + dy : dy;
      hr = hr.length === 1 ? '0' + hr : hr;
      mn = mn.length === 1 ? '0' + mn : mn;
      sc = sc.length === 1 ? '0' + sc : sc;
      return dt + '-' + m + '-' + dy + ' ' + hr + ':' + mn + ':' + sc + ' UTC';
    }

    /**
     * Maps the given DOMNode to an SVG XML string.
     *
     * @param {DOMNode} node
     *
     * @returns {string}
     */
    function nodeToSVG(node) {
      var SVG_NS = 'http://www.w3.org/2000/svg',
        name = node.nodeName,
        attributes;
      attributes = $.map(node.attributes, function (attribute) {
        return attribute.nodeName + "='" + attribute.nodeValue + "'";
      });
      attributes.push(" xmlns:svg='" + SVG_NS + "' ");
      return '<svg:' + name + ' ' + attributes.join(' ') + '></svg:' + name + '>';
    }

    /**
     * Generates RDFa representing the annotation the user has created.
     *
     * The parameters define the information the user has provided in the create
     * annotation form and the canvas and shapes they have drawn.
     *
     * All the parameters are required. Except entityID, entityLabel which are
     * optional.
     *
     * @param {object} properties
     * @param {string} properties.title
     * @param {string} properties.type
     * @param {string} properties.entity
     * @param {string} properties.entityLabel
     * @param {string} properties.color
     * @param {string} properties.content
     * @param {string} properties.canvas
     * @param {[object]} [properties.shapes]
     *
     * @returns {string}
     *   The RDFa representation of this annotation.
     */
    this.createAnnotation = function (properties) {
      var namespaces, title, type, entityID, entityLabel, color, text,
        canvas, shapes, xmlns, rdfa, annotationUUID, contentUUID;
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
      title = properties.title;
      type = properties.type;
      entityID = properties.entity_id;
      entityLabel = properties.entity;
      color = properties.color;
      text = properties.text;
      canvas = properties.canvas;
      shapes = properties.shapes || [];

      // We build the rdfa as a string.
      rdfa = '';
      annotationUUID = new UUID();
      contentUUID = new UUID();

      // Generate namespaces from RDF options
      xmlns = $.map(namespaces, function (value, key) {
        return 'xmlns:' + key + '="' + value + '"';
      });

      // Build the RDFa, open the Root Tag.
      rdfa = '<div ' + xmlns.join(' ') + '>'; // Open Root Div
      var basedoc = '<div ' + xmlns.join(' ') + '></div>';

      if (window.DOMParser) {
        var parser = new DOMParser();
        var xmldoc = parser.parseFromString(basedoc, 'text/xml');
      }
      else {
        var xmldoc = new ActiveXObject("Microsoft.XMLDOM");
        xmldoc.async = false;
        xmldoc.loadXML(basedoc);
      }

      // Build Annotation, include UUID and type and creation date.
      var rootAbout = xmldoc.createElement('div')
      rootAbout.setAttribute('about', 'urn:uuid:' + annotationUUID);
      xmldoc.documentElement.appendChild(rootAbout);

      var rdfType = xmldoc.createElement('a');
      rdfType.setAttribute('rel', 'rdf:type');
      rdfType.setAttribute('href', namespaces.oa + 'Annotation');
      rootAbout.appendChild(rdfType);

      var dcterms = xmldoc.createElement('span');
      dcterms.setAttribute('property', 'dcterms:created');
      dcterms.setAttribute('content', isoDate(new Date()));
      rootAbout.appendChild(dcterms);

      // Include the linked entity if defined.
      if (typeof entityID === 'string' && entityID !== '') {
        var relation = xmldoc.createElement('span');
        relation.setAttribute('property', 'dcterms:relation');
        relation.setAttribute('content', entityID);
        rootAbout.appendChild(relation);

        var hasPart = xmldoc.createElement('span');
        hasPart.setAttribute('property', 'dcterms:hasPart');
        hasPart.setAttribute('content', entityLabel);
        rootAbout.appendChild(hasPart);
      }

      // Include title / type.
      var dctitle = xmldoc.createElement('span');
      dctitle.setAttribute('property', 'dc:title');
      dctitle.setAttribute('content', title);
      rootAbout.appendChild(dctitle);

      var dctype = xmldoc.createElement('span');
      dctype.setAttribute('property', 'dc:type');
      dctype.setAttribute('content', type);
      rootAbout.appendChild(dctype);

      // Include the actual annotation field's content.
      var hasBody = xmldoc.createElement('a');
      hasBody.setAttribute('rel', 'oa:hasBody');
      hasBody.setAttribute('href', 'urn:uuid:' + contentUUID);
      rootAbout.appendChild(hasBody);

      var about = xmldoc.createElement('div');
      about.setAttribute('about', 'urn:uuid:' + contentUUID);
      rootAbout.appendChild(about);

      var contentAsText = xmldoc.createElement('a');
      contentAsText.setAttribute('rel', 'rdf:type');
      contentAsText.setAttribute('href', 'http://www.w3.org/2008/content#ContentAsText');
      about.appendChild(contentAsText);

      var cntChars = xmldoc.createElement('span');
      cntChars.setAttribute('property', 'cnt:chars');
      var charText = xmldoc.createTextNode(text);
      cntChars.appendChild(charText);
      about.appendChild(cntChars);

      var cntCharacterEncoding = xmldoc.createElement('span');
      cntCharacterEncoding.setAttribute('property', 'cnt:characterEncoding');
      cntCharacterEncoding.setAttribute('content', 'utf-8');
      about.appendChild(cntCharacterEncoding);

      // Include shape SVG data, filter out ones that don't exist.
      shapes = $.grep(shapes, function (shape) {
        return shape.removed === undefined;
      });

      $.each(shapes, function (index, shape) {
        var svgXml, constrainedTargetUUID, svgConstraintUUID;
        svgXml = nodeToSVG(shape.node);
        constrainedTargetUUID = new UUID();
        svgConstraintUUID = new UUID();
        svgXml = svgXml.replace("stroke='#000000'", "stroke='" + color +  "'");
        // Escape html entities, probably could use some core
        // javascript for this.
        var hasTarget = xmldoc.createElement('a');
        hasTarget.setAttribute('rel', 'oa:hasTarget');
        hasTarget.setAttribute('href', 'urn:uuid:' + constrainedTargetUUID);
        rootAbout.appendChild(hasTarget);

        var shapeAbout = xmldoc.createElement('div');
        shapeAbout.setAttribute('about', 'urn:uuid:' + constrainedTargetUUID);
        rootAbout.appendChild(shapeAbout);

        var constrainedTarget = xmldoc.createElement('a');
        constrainedTarget.setAttribute('rel', 'rdf:type');
        constrainedTarget.setAttribute('href', 'http://www.w3.org/ns/openannotation/core/ConstrainedTarget');
        shapeAbout.appendChild(constrainedTarget);

        var constrains = xmldoc.createElement('a');
        constrains.setAttribute('rel', 'oa:constrains');
        constrains.setAttribute('href', canvas);
        shapeAbout.appendChild(constrains);

        var constrainedBy = xmldoc.createElement('a');
        constrainedBy.setAttribute('rel', 'oa:constrainedBy');
        constrainedBy.setAttribute('href', 'urn:uuid:' + svgConstraintUUID);
        shapeAbout.appendChild(constrainedBy);

        var svgAbout = xmldoc.createElement('div');
        svgAbout.setAttribute('about', 'urn:uuid:' + svgConstraintUUID);
        shapeAbout.appendChild(svgAbout);

        var svgConstraint = xmldoc.createElement('a');
        svgConstraint.setAttribute('rel', 'rdf:type');
        svgConstraint.setAttribute('href', 'http://www.w3.org/ns/openannotation/core/SvgConstraint');
        svgAbout.appendChild(svgConstraint);

        var svgContentAsText = xmldoc.createElement('a');
        svgContentAsText.setAttribute('rel', 'rdf:type');
        svgContentAsText.setAttribute('href', 'http://www.w3.org/2008/content#ContentAsText');
        svgAbout.appendChild(svgContentAsText);

        var svgContent = xmldoc.createElement('span');
        svgContent.setAttribute('property', 'cnt:chars');
        svgContent.setAttribute('content', svgXml);
        svgAbout.appendChild(svgContent);

        var svgEncoding = xmldoc.createElement('span');
        svgEncoding.setAttribute('property', 'cnt:characterEncoding');
        svgEncoding.setAttribute('content', 'utf-8');
        svgAbout.appendChild(svgEncoding);
      });
      
      // Test browser features and call correct to string method
      var serializeXmlComp = function(xmldoc) {
        if (typeof window.XMLSerializer != "undefined") {
          return (new window.XMLSerializer()).serializeToString( xmldoc );
        } else if (typeof xmldoc.xml != "undefined") {
          return xmldoc.xml;
        }
        return "";
      }
 
      // convert XML DOM document to string
      var xmlAsString = serializeXmlComp( xmldoc );

      return xmlAsString;
    };

    /**
     * Creates an Annotation Object in Fedora from the data provided.
     *
     * @param {string} rdfa
     *   An rdfa xml encoded representation of the annotation.
     */
    this.ingestAnnotation = function (rdfa) {
      $.ajax({
        type: 'POST',
        async: true,
        url: IIAUtils.url('islandora/object/' + settings.pid + '/annotation/add'),
        data: {
          data: rdfa
        },
        success: function (pid) {
          var url = IIAUtils.url('islandora/object/' + settings.pid + '/annotation/get/' + pid);
          Drupal.IslandoraImageAnnotation.getInstance().fetchTriples(url);
        },
        error: function () {
          console.log('Failed to Create Annotation for: ' + settings.pid);
        }
      });
    };

    /**
     * Updates the given annotation server side.
     *
     * @param annotation
     * @param properties
     */
    function updateAnnotation(annotation, properties) {
      // We need to store slash fetch the pid here.
      $.ajax({
        type: 'POST',
        async: false,
        url: IIAUtils.url('islandora/object/' + settings.pid + '/annotation/update/' + annotation.id),
        data: {
          label: properties.title,
          type: properties.type,
          content: properties.text
        },
        success: function () {
          var url = IIAUtils.url('islandora/object/' + settings.pid + '/annotation/get/' + annotation.id);
          // Trigger reprocessing by deleting it first.
          Drupal.IslandoraImageAnnotation.getInstance().deleteAnnotation(annotation.id);
          Drupal.IslandoraImageAnnotation.getInstance().fetchTriples(url);
        }
      });
    }

    /**
     * Populates the form with the given name / values pairs.
     */
    this.setFormValues = function (values) {
      $.each(values, function (name, value) {
        $('*[name="' + name + '"]', $dialog).val(value);
      });
    };

    /**
     * Resets the form to it's defaults.
     */
    this.getFormValues = function () {
      var values, canvas;
      values = {};
      // Get all fields values.
      $('input,select,textarea', $dialog).each(function () {
        var $this, name;
        $this = $(this);
        name = $(this).attr('name');
        values[name] = $this.val();
      });
      canvas = Drupal.IslandoraImageAnnotationCanvas.getInstance();
      // If no specific canvas is given assume the current canvas.
      if (!values.canvas) {
        values.canvas = canvas.getCurrentCanvas();
      }
      values.shapes = canvas.getShapes('comment', values.canvas);
      return values;
    };

    /**
     * Resets the form to it's defaults.
     */
    function populateForm(annotation) {
      // Although there can be multiple targets, we just assume one for this
      // form.
      var $svg = $(annotation.targets[0].constraint.value);
      that.setFormValues({
        title: annotation.title,
        type: annotation.annotationType,
        stroke: $svg.attr('stroke-width'),
        color: $svg.attr('stroke'),
        text: annotation.body.value,
        canvas: annotation.getTargetCanvas().id,
        shape: 'rectangle'
      });
    }

    /**
     * Choose the shape to annotate with.
     *
     * @param {string} shape
     *   Expected to be 'rectangle', 'circle', or 'polygon'.
     */
    function chooseShape(shape) {
      $('.islandora-image-annotation-shape-icon', $dialog).removeClass('selected');
      $('.islandora-image-annotation-shape-icon[shape="' + shape + '"]', $dialog).addClass('selected');
      $('input[name="shape"]', $dialog).val(shape);
    }

    /**
     * Resets the form to it's defaults.
     */
    this.clearForm = function (edit) {
      // Clear All the fields.
      $('input,select,textarea', $dialog).val('');
      // Reset Defaults.
      $('input[name="stroke"]', $dialog).val('.3%');
      chooseShape('rectangle');
      // Set a random color for the annotations.
      $('input[name="color"]', $dialog).attr('value', getRandomColour());
      // In edit mode we can only see some fields.
      if (edit) {
        $dialog.find('.icons').hide();
        $dialog.find('.entity-type').hide();
        $dialog.find('.color').hide();
        $dialog.find('.stroke').hide();
      } else {
        $dialog.find('.icons').show();
        $dialog.find('.entity-type').show();
        $dialog.find('.stroke').show();
        // Set up the color chooser.
        if (settings.canChooseColour) {
          // Destroy the miniColor
          $('input[name="color"]', $dialog).miniColors('destroy');
          $('.color-picker').miniColors();
          $dialog.find('.color').show();
        } else {
          $dialog.find('.color').hide();
        }
      }
    };

    /**
     * Used as a callback to get the display properties of the annotation.
     *
     * @returns {object}
     */
    this.getAnnotationProperties = function () {
      var values = that.getFormValues();
      return {
        shape: values.shape,
        attributes: {
          stroke: values.color,
          'stroke-width': values.stroke
        }
      };
    };

    /**
     * Shows the Create Annotation Dialog.
     *
     * Used for creating new annotations.
     */
    function showCreateDialog() {
      $dialog.dialog($.extend(that.defaultDialogProperties, {
        title: Drupal.t('Annotate'),
        open: function () {
          var canvas = Drupal.IslandoraImageAnnotationCanvas.getInstance();
          // Clear the form to be safe.
          that.clearForm();
          // If we aren't editing an existing annotation we must be creating a
          // new one, so prepare the canvas.
          canvas.startAnnotating(canvas.getCurrentCanvas(), that.getAnnotationProperties);
        },
        close: function () {
          // Stop all annotations.
          var canvas = Drupal.IslandoraImageAnnotationCanvas.getInstance();
          canvas.stopAnnotating(canvas.getCurrentCanvas());
          // Reset to defaults.
          that.clearForm();
        },
        buttons: [{
          text: 'Save',
          // Assumes only one canvas with a valid 'canvas' attribute.
          click: function () {
            var values = that.getFormValues();

            // Minimally we only allow users to create content if they have
            // entered a title and annotation.
            if (!values.text || !values.title) {
              alert('An annotation needs both title and content');
              return 0;
            }
            // Also the user must have actually marked up the image.
            if (values.shapes.length === 0) {
              alert('You must draw a shape around the target.');
              return 0;
            }

            // Set default type if not specified.
            values.type = (values.type === '' || values.type === null) ? Drupal.t('unclassified') : values.type;

            // Create RDFa representing the current annotation, all the
            // parameters are required. Except entityID, entityLabel, it also
            // generates identifiers for the annotation and it's content.
            that.ingestAnnotation(that.createAnnotation(values));
            $dialog.dialog('close');
          }
        }, {
          text: 'Cancel',
          click: function () {
            $dialog.dialog('close');
          }
        }]
      }));
    }

    /**
     * Shows the Create Annotation Dialog.
     *
     * Used for creating new annotations.
     */
    function showEditDialog(annotation) {
      $dialog.dialog($.extend(that.defaultDialogProperties, {
        title: Drupal.t('Update Annotation'),
        height: 550,
        open: function () {
          // Clear the form to be safe.
          that.clearForm(true);
          // Populate the form with the given annotation.
          populateForm(annotation);
        },
        close: function () {
          // Reset to defaults.
          that.clearForm();
        },
        buttons: [{
          text: 'Save',
          // Assumes only one canvas with a valid 'canvas' attribute.
          click: function () {
            var values = that.getFormValues();

            // Minimally we only allow users to create content if they have
            // entered a title and annotation.
            if (!values.text || !values.title) {
              alert('An annotation needs both title and content');
              return 0;
            }

            // Set default type if not specified.
            values.type = (values.type === '' || values.type === null) ? Drupal.t('unclassified') : values.type;

            // Update the annotation.
            updateAnnotation(annotation, values);
            $dialog.dialog('close');
          }
        }, {
          text: 'Cancel',
          click: function () {
            $dialog.dialog('close');
          }
        }]
      }));
    }

    /**
     * Creates a dialog box and displays it.
     *
     * @param {RDF.Annotation} annotation
     */
    this.show = function (annotation) {
      if (annotation !== undefined) {
        showEditDialog(annotation);
      } else {
        showCreateDialog();
      }
    };

    /**
     * Helper function that allows us to theme autocomplete elements.
     */
    function autocompleteOpen () {
      // Add a class to the menu so we can theme it independently.
      /*jshint validthis:true */
      var autocomplete = $(this).data("autocomplete") || $(this).data('uiAutocomplete');
      if (autocomplete.menu !== undefined) {
        autocomplete.menu.element.addClass('islandora-image-annotation');
      }
    }

    // Link the Shapes to the the 'shape' hidden input field.
    $('.islandora-image-annotation-shape-icon', $dialog).click(function () {
      chooseShape($(this).attr('shape'));
    });

    // Auto populate with the title field.
    if (settings.useTitleVocabulary) {
      $('input[name="title"]').autocomplete({
        open: autocompleteOpen,
        source: function (request, response) {
          var type, results = [];
          type = $('select[name="type"] option:selected, input[name="type"]').val();
          $.ajax({
            url: Drupal.settings.basePath + 'islandora/entities/search/' + type + '?entities_query=' + request.term,
            async: false,
            dataType: 'json',
            success: function (data, status, xhr) {
              response(data);
            }
          });
        }
      });
      $('select[name="type"], input[name="type"]').change(function() {
        var type, $title;
        type = $(this).val();
        $title = $('select[name="title"]');
        $.getJSON(Drupal.settings.basePath + 'islandora/annotation/title/' + type, {
          ajax : 'true'
        }, function (options) {
          $title.empty();
          $title.append($("<option>").attr('value', '').text('Select from ' + type));
          $.each(options, function(index, value) {
            $title.append($("<option>").attr('value', value).text(value));
          });
        });
      });
    }

    // Set up autocomplete on Type field. If it's rendered as a select field it
    // won't attache the autocomplete.
    $('input[name="type"]').autocomplete({
      open: autocompleteOpen,
      source: settings.types,
      change: function(event, ui) {
        $('input[name="type"]').trigger('change');
      }
    });

    if (settings.allowEntityLinking) {
      // Set up autocomplete on Entity field.
      $('input[name="entity"]').autocomplete({
        open: autocompleteOpen,
        source: function (request, response) {
          var type, results = [];
          type = $('select[name="entity_type"] option:selected').text();
          $.ajax({
            url: Drupal.settings.basePath + 'islandora/entities/search/' + type + '?entities_query=' + request.term,
            async: false,
            dataType: 'json',
            success: function (data, status, xhr) {
              if ($.isPlainObject(data)) {
                data = [data];
              }
              if (data !== null) {
                results = data;
              } else {
                results = [];
              }
            }
          });
          response($.map(results, function (item) {
            // The server responds with a <a> tag for some god awful reason. The
            // PID is the inner text of this result.
            var pid = $(item.Object).text();
            return {
              label: item.identifier,
              value: item.identifier,
              pid: pid
            };
          }));
        },
        select: function (event, ui) {
          // Set the hidden entity_id field.
          $('input[name="entity_id"]').val(ui.item.pid);
        }
      });
    }
  };

  /**
   * Gets the global singleton of this class.
   *
   * This function is meant to be used after Drupal.attachBehaviors() has been
   * called, otherwise the singleton instance won't exist.
   *
   * @return {Drupal.IslandoraImageAnnotationDialog}
   *   The singleton
   */
  Drupal.IslandoraImageAnnotationDialog.getInstance = function () {
    return Drupal.IslandoraImageAnnotationDialog[base];
  };

}(jQuery));
