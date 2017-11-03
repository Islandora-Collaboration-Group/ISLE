/*jslint browser: true*/
/*global jQuery, Drupal, Raphael, ScaleRaphael, UUID*/
/**
 * @file
 * Defines the Islandora Image Annotation Canvas and it's Drupal behaviour.
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
  var base = '#islandora-image-annotation-canvas';

  /**
   * Initialize the Islandora Image Annotation Canvas.
   *
   * We can only have one canvas per page.
   */
  Drupal.behaviors.islandoraImageAnnotationCanvas = {
    attach: function (context) {
      $(base, context).once('islandoraImageAnnotationCanvas', function () {
        Drupal.IslandoraImageAnnotationCanvas[base] = new Drupal.IslandoraImageAnnotationCanvas(base);
      });
    },
    detach: function (context) {
      $(base).removeClass('islandoraImageAnnotationCanvas-processed');
      $(base).removeData();
      $(base).off();
      $('#canvases, #svg-wrapper, #annotations').empty();
      delete Drupal.IslandoraImageAnnotationCanvas[base];
    }
  };

  /**
   * Instantiate the Image Annotation Canvas.
   *
   * @param {string} base
   *   The id of the html element in which to bind the canvas to.
   *
   * @constructor
   */
  Drupal.IslandoraImageAnnotationCanvas = function (base) {
    // Private Members.
    var that = this;
    $.extend(this, {
      // RDF.Resource objects representing each canvas.
      canvases: {},
      // Raphaels Paper objects for each annotation type.
      raphaels: {
        image: {},
        text: {},
        audio: {},
        zone: {},
        comment: {}
      },
      // Render order of the various annotations.
      'zOrders': {
        'image': 1,
        'text': 2000,
        'audio': 3000,
        'zone': 4000,
        'comment': 5000
      }
    });

    /**
     * Gets the ID of the current canvas.
     *
     * @returns {string}
     *   The ID of the current canvas, typically something like this:
     *   http://example.com/islandora/object/PID/datastream/DSID/view/Canvas
     */
    this.getCurrentCanvas = function () {
      // Just grabs the first canvas, we don't support switching between them
      // right now.
      return $('.canvas[canvas]', '#canvases').attr('canvas');
    };

    /**
     * Gets the currently rendered shapes for the given type and canvas.
     *
     * @param {string} type
     *   The type of annotation either 'image', 'text', 'audio', 'zone', or
     *   'comment'.
     * @param {string} canvas
     *   The identifier of the canvas.
     *
     * @returns {Array|shapes|*}
     */
    this.getShapes = function (type, canvas) {
      return this.raphaels[type][canvas].shapes;
    };

    /**
     * Gets the canvas element with the given identifier.
     *
     * @param {string} canvas
     *   The identifier of the canvas.
     *
     * @returns {*|HTMLElement}
     *   The Canvas element.
     */
    function getCanvasElement(canvas) {
      return $('.canvas[canvas="' + canvas + '"]', base);
    }

    /**
     * Gets the scaled dimensions of the given canvas.
     *
     * @param {string} canvas
     *   The identifier of the canvas.
     *
     * @returns {{width: number, height: number}}
     *   The scaled dimensions.
     */
    function getCanvasDimensions(canvas) {
      var $canvas, width, height;
      $canvas = getCanvasElement(canvas);
      width = $canvas.width();
      height = $canvas.height();
      return {
        width: width,
        height: height
      };
    }

    /**
     * Gets the scaled dimensions of the given canvas.
     *
     * @param {string} canvas
     *   The identifier of the canvas.
     *
     * @returns {{width: number, height: number}}
     *   The scaled dimensions.
     */
    function getScaledCanvasDimensions(canvas) {
      var dimensions, scale;
      dimensions = getCanvasDimensions(canvas);
      scale = that.canvases[canvas].width / dimensions.width;
      return {
        width: scale * dimensions.width,
        height: scale * dimensions.height
      };
    }

    /**
     * Resize the image annotations to fit within the given width and height.
     *
     * @param {number} width
     *   The width in pixels.
     * @param {number} [height]
     *   The height in pixels.
     */
    function resizeImageAnnotations(width, height) {
      $('.image-annotation').each(function () {
        var $imageWrapper, $image;
        $imageWrapper = $(this);
        //noinspection JSValidateTypes
        $image = $imageWrapper.children('img:first');
        $image.width(width);
        $image.height(height || 'auto');
        $imageWrapper.height($image.height());
      });
    }

    /**
     * Resize the canvas divs to fit within the given width and height.
     *
     * @param {number} width
     *   The width in pixels.
     * @param {number} height
     *   The height in pixels.
     */
    function resizeCanvases(width, height) {
      $('.canvas').each(function () {
        var $canvas = $(this);
        $canvas.width(width);
        $canvas.height(height);
      });
    }

    /**
     * Resize the raphaels shapes to fit within the given width and height.
     *
     * @param {number} width
     *   The width in pixels.
     * @param {number} height
     *   The height in pixels.
     */
    function resizeRaphaels(width, height) {
      $.each(that.raphaels, function (type, canvases) {
        $.each(canvases, function (canvas, paper) {
          paper.changeSize(width, height, false, false);
        });
      });
    }

    /**
     * Resize the canvas and the image inside of it.
     */
    this.resizeCanvasContainer = function () {
      var $canvasBody, width, heights, maxHeight;
      $canvasBody = $('#canvas-body');
      width = $canvasBody.width();
      resizeImageAnnotations(width);
      // Needs to be set explicitly since absolute position elements
      // like the #svg-canvas are not in the normal document flow.
      heights = $('.image-annotation').map(function () {
        return $(this).height();
      }).get();
      maxHeight = Math.max.apply(null, heights);
      $canvasBody.height(maxHeight);
      resizeCanvases(width, maxHeight);
      resizeRaphaels(width, maxHeight);
    };

    /**
     * Binds the resize callback to the window so the canvas stays pretty.
     */
    function bindResizeCallback() {
      var poll = (function () {
        var timer = 0;
        return function (callback, milliseconds) {
          clearTimeout(timer);
          timer = setTimeout(callback, milliseconds);
        };
      }());
      $(window).resize(function () {
        // Poll the window.resize event every 300ms for changes and resize the
        // canvas appropriately.
        poll(function () {
          that.resizeCanvasContainer();
        }, 300);
      });
    }

    /**
     * Creates an SVG element and a Raphael wrapper to manipulate it.
     *
     * There is one Raphael object per type and canvas.
     *
     * @param {string} type
     *   The type of annotation either 'image', 'text', 'audio', 'zone', or
     *   'comment'.
     * @param {string} canvas
     *   The identifier of the canvas.
     *
     * @returns {object}
     *   The Raphael object for drawing.
     */
    function makeRaphael(type, canvas) {
      var svgId, $svg, paper, dimensions, scaled;

      // One Raphael per Annotation Type / Canvas.
      if (that.raphaels[type][canvas] !== undefined) {
        return that.raphaels[type][canvas];
      }
      // Fetch dimensions for rendering and scaling.
      dimensions = that.canvases[canvas];
      scaled = getCanvasDimensions(canvas);
      // svgId = new UUID();
      svgId = 'svg-annotation-' + type;
      $svg = $('<div />', {
        id: svgId,
        type: type,
        canvas: canvas
      });
      $('#svg-wrapper').append($svg);

      // Allow a base image at 1
      $svg.css('z-index', that.zOrders[type] + 1);
      $svg.css('position', 'static');
      $svg.position({
        'of': getCanvasElement(canvas),
        'my': 'left top',
        'at': 'left top',
        'collision': 'none'
      });

      paper = new ScaleRaphael(svgId, dimensions.width, dimensions.height);
      paper.changeSize(scaled.width, scaled.height, false, false);
      //paper.safari() can be called without previous Browser detection. Raphael does this check.
      paper.safari();
      
      // Cache svgCanvas.
      that.raphaels[type][canvas] = paper;
      return paper;
    }

    /**
     * Creates the icon that is used to create the polygon.
     *
     * @param {object} background
     *   The background Raphael Element in which the coordinates are in.
     * @param {object} polygon
     *   The point on the polygon to show the grabber.
     * @param {number} x
     *   The x coordinate.
     * @param {number} y
     *   The y coordinate.
     *
     * @returns {Element}
     *  A circle used as icon to indicate the latest point on the polygon.
     */
    function makeGrabber(background, polygon, x, y) {
      var onEnd, onStart, move, radius, circle;

      // Start the drag event.
      onStart = function () {
        this.start = [this.attr('cx'), this.attr('cy')];
      };

      // End the drag event.
      onEnd = function () {
        if (background.creating === this.polygon && this.index() === 0) {
          background.creating = null;
          this.polygon.attr('path', this.polygon.attr('path') + 'Z');
        }
      };

      // Move the circle.
      move = function (dx, dy) {
        var path, sdx, sdy, index = this.index();
        sdx = Math.floor(dx * background.invertedScale);
        sdy = Math.floor(dy * background.invertedScale);
        this.attr({
          cx: this.start[0] + sdx,
          cy: this.start[1] + sdy
        });
        path = Raphael.parsePathString(this.polygon.attr("path"));
        path[index][1] = Math.floor(this.start[0] + dx);
        path[index][2] = Math.floor(this.start[1] + dy);
        this.polygon.attr('path', path);
      };

      radius = Math.floor(10 * background.invertedScale);
      circle = background.paper.circle(x, y, radius);
      circle.attr({
        fill: '#ffffff',
        opacity: 0.3,
        stroke: 'black',
        'stroke-width': 'none',
        'stroke-dasharray': '- '
      });
      circle.polygon = polygon;
      polygon.set.push(circle);
      circle.start = [x, y];
      circle.moveFn = move;
      circle.drag(move, onStart, onEnd);

      // Gets the grabbers "index" which correlates to the point in the "path"
      // it represents.
      circle.index = function () {
        var me, grabbers, index;
        me = this;
        index = -1;
        // Filter out the removed grabbers an polygon objects.
        grabbers = $.grep(this.polygon.set, function (element) {
          return element.type === 'circle';
        });
        $.each(grabbers, function (i, element) {
          if (me === element) {
            index = i;
            return false;
          }
        });
        return index;
      };

      // Double clicking on the "grabber" will remove it and the point in the
      // "path" it represents.
      circle.dblclick(function () {
        var path = Raphael.parsePathString(this.polygon.attr("path"));
        path.splice(this.index(), 1);
        // A Path must be two or more points, we must also account for the 'Z'
        // terminator point.
        if (path.length <= 2) {
          background.creating = null;
          this.polygon.set.remove();
        } else {
          // Update the polygon's path such that it no longer includes this
          // "grabber". Make sure the first element in the path is marked.
          path[0][0] = 'M';
          this.polygon.attr('path', path);
        }
        this.remove();
      });
      return circle;
    }

    /**
     * Creates a Polygon, by tracking clicks and drawing a path between them.
     *
     * @param {object} background
     *   The background Raphael Element in which the coordinates are in.
     * @param {number} x
     *   The x coordinate.
     * @param {number} y
     *   The y coordinate.
     * @param {object} attributes
     *   HTML element attributes to apply to the polygon, like color or
     *   stroke-width.
     *
     * @returns {Element}
     *  The polygon as a Raphael Element.
     */
    function makePolygon(background, x, y, attributes) {
      var onStart, onEnd, move, resize, addPoint, outer, paper, group, grabber;
      // Start the drag event.
      onStart = function () {
        this.set.tmp = [0, 0];
      };

      // End the drag event.
      onEnd = function () {
        this.set.tmp = undefined;
      };

      // Move a 'node' on the path.
      move = function (dx, dy) {
        var sdx, sdy;
        sdx = Math.floor(dx * background.invertedScale);
        sdy = Math.floor(dy * background.invertedScale);
        this.set.translate(sdx - this.set.tmp[0], sdy - this.set.tmp[1]);
        this.set.tmp = [sdx, sdy];
      };

      // Move a 'node' on the path.
      resize = function (dx, dy) {
        var sdx, sdy;
        sdx = Math.floor(dx * background.invertedScale);
        sdy = Math.floor(dy * background.invertedScale);
        grabber = this.set[this.set.length - 1];
        grabber.moveFn(sdx, sdy);
      };

      // Add a point to the polygon.
      addPoint = function (background, x, y) {
        this.attr('path', this.attr('path') + ('L' + x + ',' + y));
        grabber = makeGrabber(background, this, x, y);
      };

      // Create the starting point.
      paper = background.paper;
      group = paper.set();
      outer = paper.path("M" + x + ',' + y);
      outer.attr(attributes);
      outer.addPoint = addPoint;
      group.push(outer);
      outer.set = group;

      grabber = makeGrabber(background, outer, x, y);
      outer.drag(move, onStart, onEnd);
      outer.resizeFn = resize;
      return outer;
    }

    /**
     * Creates a Circle, where the user clicks, they can resize by dragging.
     *
     * @param {object} background
     *   The background Raphael Element in which the coordinates are in.
     * @param {number} x
     *   The x coordinate.
     * @param {number} y
     *   The y coordinate.
     * @param {object} attributes
     *   HTML element attributes to apply to the polygon, like color or
     *   stroke-width.
     *
     * @returns {Element}
     *  The circle as a Raphael Element.
     */
    function makeCircle(background, x, y, attributes) {
      var inner, outer, group, paper, innerSize,
        onStart, onEnd, move, resize;
      // Make the innerSize large enough so that it's visible at the time of
      // creation.
      innerSize = Math.floor(10 * background.invertedScale);

      // Start the drag event.
      onStart = function () {
        this.start = [this.attr("cx"), this.attr("cy"), this.attr('r')];
      };

      // Finish the drag event.
      onEnd = function () {
        this.start = undefined;
      };

      // Move the Circle.
      move = function (dx, dy) {
        this.set.attr({
          cx: this.start[0] + Math.floor(dx * background.invertedScale),
          cy: this.start[1] + Math.floor(dy * background.invertedScale)
        });
      };

      // Resize the Circle.
      resize = function (dx) {
        var outerRadius = this.start[2] + Math.floor(dx * background.invertedScale),
          innerRadius = this.start[2] + (Math.floor(dx * background.invertedScale) - innerSize);
        this.attr('r', Math.max(0, outerRadius));
        this.inner.attr('r', Math.max(0, innerRadius));
      };

      // Draw the circle and it's icon.
      paper = background.paper;
      group = paper.set();

      // This is the circle the user is drawing.
      outer = paper.circle(x, y, innerSize);
      outer.attr(attributes);
      outer.start = [x, y, innerSize];
      outer.set = group;

      // The inner circle acts as tool to visualize the outer circle the user is
      // drawing / resizing / moving.
      inner = paper.circle(x, y, innerSize / 2);
      inner.attr({
        fill: '#ffffff',
        opacity: 0.3,
        stroke: 'black',
        'stroke-width': 'none',
        'stroke-dasharray': '- '
      });
      inner.toFront();
      inner.set = group;

      // Add the inner circle to the outer and add them to the set.
      outer.inner = inner;
      group.push(outer);
      group.push(inner);

      // Set up behaviours for drag resizing and moving.
      inner.drag(move, onStart, onEnd);
      outer.drag(resize, onStart, onEnd);
      outer.resizeFn = resize;
      // User can remove the annotation by double clicking on it's center.
      inner.dblclick(function () {
        this.set.remove();
      });
      return outer;
    }

    /**
     * Creates a Circle, where the user clicks, they can resize by dragging.
     *
     * @param {object} background
     *   The background Raphael Element in which the coordinates are in.
     * @param {number} x
     *   The x coordinate.
     * @param {number} y
     *   The y coordinate.
     * @param {object} attributes
     *   HTML element attributes to apply to the polygon, like color or
     *   stroke-width.
     *
     * @returns {Element}
     *  The circle as a Raphael Element.
     */
    function makeRectangle(background, x, y, attributes) {
      var inner, outer, group, paper, innerSize, onStart, onEnd, move, resize;
      // Make the innerSize large enough so that it's visible at the time of
      // creation.
      innerSize = Math.floor(14 * background.invertedScale);

      // Start the drag event.
      onStart = function () {
        this.set.start = [
          Math.floor(this.set.outer.attr('x')),
          Math.floor(this.set.outer.attr('y')),
          Math.floor(this.set.outer.attr('height')),
          Math.floor(this.set.outer.attr('width'))
        ];
      };

      // End the drag event.
      onEnd = function () {
        this.set.start = undefined;
      };

      // Move the Rectangle.
      move = function (dx, dy) {
        this.set.outer.attr({
          'x': this.set.start[0] + Math.floor(dx * background.invertedScale),
          'y' : this.set.start[1] + Math.floor(dy * background.invertedScale)
        });
        this.set.inner.attr({
          'x': this.set.start[0] + this.set.start[3] + Math.floor(dx * background.invertedScale) - innerSize,
          'y' : this.set.start[1] + this.set.start[2] + Math.floor(dy * background.invertedScale) - innerSize
        });
      };

      // Resize the Rectangle.
      resize = function (dx, dy) {
        var width, height;
        height = this.set.start[2] + Math.floor(dy * background.invertedScale);
        width = this.set.start[3] + Math.floor(dx * background.invertedScale);
        this.set.outer.attr({
          'width' : Math.max(0, width),
          'height' : Math.max(0, height)
        });
        this.set.inner.attr({
          'x' : this.set.start[0] + this.set.start[3] + Math.floor(dx * background.invertedScale) - innerSize,
          'y': this.set.start[1] + this.set.start[2] + Math.floor(dy * background.invertedScale) - innerSize
        });
      };

      // Draw the rectangle and it's icon.
      paper = background.paper;
      group = paper.set();
      group.start = [x, y, innerSize, innerSize];
      outer = paper.rect(x, y, innerSize, innerSize);
      outer.attr(attributes);
      outer.set = group;
      group.push(outer);
      // The inner circle acts as tool to visualize the outer circle the user is
      // drawing / resizing / moving.
      inner = paper.rect(x, y, innerSize + 1, innerSize + 1);
      inner.attr({
        fill: '#ffffff',
        opacity: 0.3,
        stroke: 'black',
        'stroke-width': 'none',
        'stroke-dasharray': '- '
      });
      inner.toFront();
      inner.set = group;
      group.push(inner);

      // Store references to both the outer and inner for use in onStart().
      group.outer = outer;
      group.inner = inner;

      // Set up behaviours for drag resizing and moving.
      inner.drag(resize, onStart, onEnd);
      outer.drag(move, onStart, onEnd);
      outer.resizeFn = resize;
      inner.dblclick(function () {
        this.set.remove();
      });
      return outer;
    }

    /**
     * Invert the given coordinates within the background.
     *
     * @param {object} background
     *   The background in which the coordinates are in.
     * @param {number} x
     *   The x coordinate.
     * @param {number} y
     *   The y coordinate.
     *
     * @returns {[number, number]}
     *   The inverted XY coordinates.
     */
    function invertXYCoordinates(background, x, y) {
      var $wrap, $window, invertedX, invertedY;
      $wrap = $(background.paper.wrapperElem);
      $window = $(window);

      // This is location of canvas
      invertedX = x - $wrap.offset().left;
      invertedY = y - $wrap.offset().top;

      // Change made to support embedding shared canvas.
      //noinspection JSValidateTypes
      invertedX += $window.scrollLeft();
      //noinspection JSValidateTypes
      invertedY += $window.scrollTop();

      // And now scale for Canvas resizing
      invertedX = Math.floor(invertedX * background.invertedScale);
      invertedY = Math.floor(invertedY * background.invertedScale);
      return [invertedX, invertedY];
    }

    /**
     * Starts annotating process for the given canvas.
     *
     * @param {string} canvas
     *   The canvas identifier, for the canvas to annotate.
     *
     * @param {function} getAnnotationProperties
     *   Since we can change the display attributes while annotating we must
     *   fetch the latest attributes at the time of creating a shape.
     */
    this.startAnnotating = function (canvas, getAnnotationProperties) {
      var onStart, onEnd, raphael, invertedScale, height, width, background;

      // Start the drag event.
      onStart = function (x, y) {
        var invertedCoordinates, invertedX, invertedY, properties;
        invertedCoordinates = invertXYCoordinates(this, x, y);
        invertedX = invertedCoordinates[0];
        invertedY = invertedCoordinates[1];
        properties = getAnnotationProperties();
        switch (properties.shape) {
        case 'rectangle':
          this.creating = makeRectangle(this, invertedX, invertedY, properties.attributes);
          this.shapes.push(this.creating);
          break;
        case 'circle':
          this.creating = makeCircle(this, invertedX, invertedY, properties.attributes);
          this.shapes.push(this.creating);
          break;
        case 'polygon':
          if (this.creating === null) {
            this.creating = makePolygon(this, invertedX, invertedY, properties.attributes);
            this.shapes.push(this.creating);
          } else {
            this.creating.addPoint(this, invertedX, invertedY);
          }
          break;
        }
      };

      // End the drag event.
      onEnd = function () {
        var properties;
        properties = getAnnotationProperties();
        if (this.creating === null) {
          return;
        }
        // We don't do anything for polygons as they are driven by clicks rather
        // than the "main" drag event.
        switch (properties.shape) {
        case 'rectangle':
          if (this.creating.set !== undefined && this.creating.set.start !== undefined) {
            this.creating.set.start = [];
          }
          this.creating = null;
          break;
        case 'circle':
          // Clear all start events if any remain.
          if (this.creating.start !== undefined) {
            this.creating.start = [];
          }
          this.creating = null;
          break;
        }
      };
      // Create the Canvas / paper to render onto.
      raphael = makeRaphael('comment', canvas);
      invertedScale = 1.0 / raphael.newScale;
      height = Math.floor(raphael.height * invertedScale);
      width = Math.floor(raphael.width * invertedScale);
      // Create a background rectangle in which will encompass the shapes drawn
      // by the user.
      background = raphael.rect(0, 0, width, height);
      background.attr({
        'fill': 'white',
        'opacity': 0.15
      });
      background.creating = null;
      background.paper = raphael;
      background.shapes = [];
      background.invertedScale = invertedScale;
      raphael.annotationBackground = background;
      raphael.shapes = background.shapes;
      background.drag(function (dx, dy) {
        // Delegate on resize to the element being resized.
        this.creating.resizeFn(dx, dy);
      }, onStart, onEnd);
    };

    /**
     * Stops the annotation process for the given canvas.
     *
     * @param {string} canvas
     */
    this.stopAnnotating = function (canvas) {
      var raphael;
      if (that.raphaels.comment[canvas] !== undefined) {
        raphael = that.raphaels.comment[canvas];
        if (raphael.annotationBackground !== undefined) {
          $.each(raphael.annotationBackground.shapes, function (index, shape) {
            if (shape.set !== undefined) {
              shape.set.remove();
            } else {
              shape.remove();
            }
          });
          // May not be what we want, may remove all annotations.
          raphael.annotationBackground.remove();
          $(raphael.wrapperElem).remove();
          $(raphael).remove();
        }
        delete that.raphaels.comment[canvas];
      }
    };

    /**
     * Draw the given annotation on the given canvas element.
     *
     * This is much more minimal than what was originally part of the
     * SharedCanvas, it only supports rendering the entire image.
     * Scaled to fit the canvas.
     *
     * @param {RDF.Annotation} annotation
     *   The annotation to draw.
     *
     */
    function drawImageAnnotation(annotation) {
      var canvas, scaled, $imageAnnotation, $image;
      canvas = annotation.getTargetCanvas();
      scaled = getScaledCanvasDimensions(canvas.id);
      $imageAnnotation = $('<div />', {
        annotation: annotation.id,
        canvas: canvas.id,
        class: 'image-annotation'
      });
      $image = $('<img />', {
        src: annotation.getBodyTarget().id,
        width: scaled.width,
        height: 'auto'
      });
      $image.one('load', function () {
        // Update the canvas size to reflect the new image.
        that.resizeCanvasContainer(canvas.id);
      });
      $("#annotations").append($imageAnnotation.append($image));
      // Adjust the canvas size now that we've loaded everything.
      // that.resizeCanvasContainer();
    }

    /**
     * Draws the given comment annotation.
     *
     * @param {RDF.annotation} annotation
     *   The comment annotation to draw.
     */
    function drawCommentAnnotation(annotation) {
      var canvas = annotation.getTargetCanvas();
      $.each(annotation.targets, function (index, target) {
        var shape, svg;
        shape = $(':first-child', $.parseXML(target.constraint.value));
        shape.attr('annotation', annotation.id);
        svg = makeRaphael('comment', canvas.id);
        $(svg.canvas).append(shape);
      });
    }

    /**
     * Draw the given annotation.
     *
     * Original version of shared Canvas apparently supported text and audio
     * annotations. This has been discontinued as we only do 'image' and
     * 'comment' annotations now.
     *
     * @param {RDF.Annotation} annotation
     *   The annotation to draw
     */
    this.drawAnnotation = function (annotation) {
      switch (annotation.getType()) {
      case 'image':
        drawImageAnnotation(annotation);
        break;
      case 'comment':
        drawCommentAnnotation(annotation);
        break;
      }
    };

    /**
     * Hides the given comment annotation.
     *
     * @param {RDF.annotation} annotation
     *   The comment annotation to draw.
     */
    function hideCommentAnnotation(annotation) {
      $('svg *[annotation="' + annotation.id + '"]').remove();
    }

    /**
     * Hides the given annotation.
     *
     * We only allow hiding of 'comment' annotations for now as we don't want
     * the image to every be removed.
     *
     * @param {RDF.Annotation} annotation
     *   The annotation to hide.
     */
    this.hideAnnotation = function (annotation) {
      switch (annotation.getType()) {
      case 'comment':
        hideCommentAnnotation(annotation);
        break;
      }
    };

    // Start up.
    bindResizeCallback();

    // Create the Canvas.
    Drupal.IslandoraImageAnnotation.on('processedSequence', function (event, sequence) {
      if (sequence.isCanvas()) {
        // Cache the original dimensions as we may need to scale the canvas
        // contents later.
        that.canvases[sequence.id] = sequence;
        $('#canvases').append($('<div />', {
          class: 'canvas',
          canvas: sequence.id,
          width: sequence.width,
          height: sequence.height
        }));
      }
    });

    // Draw the image annotation.
    Drupal.IslandoraImageAnnotation.on('processedAnnotation', function (event, annotation) {
      if (annotation.getType() === 'image') {
        // We always draw images right away.
        drawImageAnnotation(annotation);
      }
    });

    // Remove the given annotation.
    Drupal.IslandoraImageAnnotation.on('deleteAnnotation', function (event, annotation) {
      that.hideAnnotation(annotation);
    });

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
  Drupal.IslandoraImageAnnotationCanvas.getInstance = function () {
    return Drupal.IslandoraImageAnnotationCanvas[base];
  };

}(jQuery));
