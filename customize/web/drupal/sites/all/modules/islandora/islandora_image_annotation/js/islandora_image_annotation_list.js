/*jslint browser: true, devel:true*/
/*global jQuery, Drupal, IIAUtils*/
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
  var base = '#islandora-image-annotation-list';

  /**
   * Initialize the Tabs section of the Image annotation widget.
   *
   * We can only have one Tabs per page.
   */
  Drupal.behaviors.islandoraImageAnnotationList = {
    attach: function (context, settings) {
      $(base, context).once('islandoraImageAnnotationList', function () {
        Drupal.IslandoraImageAnnotationList[base] = new Drupal.IslandoraImageAnnotationList(base, settings.islandoraImageAnnotationList);
      });
    },
    detach: function (context) {
      $(base).removeClass('islandoraImageAnnotationList-processed');
      $(base).removeData();
      $(base).off();
      $(base).empty();
      delete Drupal.IslandoraImageAnnotationList[base];
    }
  };

  /**
   * Instantiate the Annotation List.
   *
   * @param {string} base
   *   The id of the html element in which bind the list to.
   *
   * @param {object} settings
   *   The Drupal.settings variable for this widget.
   *
   * @constructor
   */
  Drupal.IslandoraImageAnnotationList = function (base, settings) {
    var that = this;
    this.base = base;
    this.settings = settings;

    // Create the comment type containers for the comment annotations.
    // trigger a load of each comment annotation.
    $.each(settings.annotations, function (pid, annotation) {
      that.insertAnnotationTypeContainer(that.createAnnotationTypeContainer(annotation.type));
    });

    // Fetch the comment annotations only after we've processed the canvas
    // they belong to.
    Drupal.IslandoraImageAnnotation.on('processedSequence', function (event, annotation) {
      // Create the comment type containers for the comment annotations.
      // trigger a load of each comment annotation.
      $.each(settings.annotations, function (pid, annotation) {
        var url = IIAUtils.url('islandora/object/' + settings.pid + '/annotation/get/' + pid);
        Drupal.IslandoraImageAnnotation.getInstance().fetchTriples(url);
      });
    });

    // Add any processed comment annotations to the list.
    Drupal.IslandoraImageAnnotation.on('processedAnnotation', function (event, annotation) {
      if (annotation.getType() === 'comment') {
        that.addAnnotation(annotation);
      }
    });

    // Listen for removed 'comment' annotations and update th list accordingly.
    Drupal.IslandoraImageAnnotation.on('deleteAnnotation', function (event, annotation) {
      var $annotation, $content;
      if (annotation.getType() === 'comment') {
        $annotation = that.getAnnotationElement(annotation.id);
        $content = $annotation.parent();
        // Remove the annotation from the list.
        $annotation.remove();
        // If there are no annotations of this type left anymore remove the
        // type, group from the list.
        if ($content.children().length === 0) {
          $content.parent().remove();
        }
      }
    });

    if (settings.editable) {
      this.createContextMenu();
    }
  }

  Drupal.IslandoraImageAnnotationList.prototype = $.extend(Drupal.IslandoraImageAnnotationList.prototype, {
    /**
     * Gets the type container from the given type.
     *
     * @returns {jQuery}
     *   jQuery wrapper around the type container if found, otherwise an empty,
     *   jQuery wrapper.
     */
    getAnnotationTypeContainer: function (type) {
      return $('.comment-type-container[type="' + type + '"]');
    },

    /**
     * The list of comment annotations are grouped by type.
     *
     * @returns {bool}
     *   True if the given type container exists false otherwise.
     */
    hasAnnotationTypeContainer: function (type) {
      return this.getAnnotationTypeContainer(type).length !== 0;
    },

    /**
     * Creates a container in which to put annotations by the given type.
     *
     * @returns {jQuery}
     *   True if the given type container exists false otherwise.
     */
    createAnnotationTypeContainer: function (type) {
      var $type, $title, $content;
      // Annotations are grouped by type.
      $type = $('<div />', {
        class: 'comment-type-container',
        type: type
      });
      $title = $('<div class="comment-type-title">' + type + '</div>');
      // Display the Content when the title is clicked.
      $title.click(function () {
        $('+ .comment-type-content', this).toggle();
      });
      // Each annotation is listed in the content section, which is initially
      // hidden.
      $content = $('<div />', {
        class: 'comment-type-content'
      }).hide();
      return $type.append($title, $content);
    },

    /**
     * Inserts the annotation container in alphabetical order.
     */
    insertAnnotationTypeContainer: function ($element) {
      var title, $containers;
      title = $('.comment-type-title', $element).text().toLowerCase();
      if (this.hasAnnotationTypeContainer(title)) {
        return this.getAnnotationTypeContainer(title);
      }
      $containers = $('.comment-type-container', this.base).filter(function () {
        return $('.comment-type-title', this).text().toLowerCase() > title;
      });
      if ($containers.length > 0) {
        $element.insertBefore($containers.get(0));
      } else {
        $(this.base).append($element);
      }
      return $element;
    },

    /**
     * Gets the annotation's list element given it's annotation id.
     *
     * @param {string} id
     *   The identifier for the annotation expected to be a UUID with the format
     *   "urn:"<NID>":"<NSS>.
     */
    getAnnotationElement: function (id) {
      return $(".canvas-annotation[annotation='" + id + "']", this.base);
    },

    /**
     * Toggles the display of the annotation identified by the given id.
     *
     * Shows the comment text section of the annotation and the annotation color
     * icon, also displays the annotation shapes on the canvas.
     *
     * @param {string} id
     *   The identifier for the annotation.
     */
    showAnnotation: function (id) {
      var $annotation, canvas, annotation;
      $annotation = this.getAnnotationElement(id);
      $('.comment-text, span.color', $annotation).show();
      $('.comment-show-hide', $annotation).text('-');
      $('.comment-title', $annotation).addClass('annotation-opened');
      canvas = Drupal.IslandoraImageAnnotationCanvas.getInstance();
      annotation = Drupal.IslandoraImageAnnotation.getInstance().getAnnotation(id);
      if (annotation) {
        canvas.drawAnnotation(annotation);
      }
      Drupal.IslandoraImageAnnotation.trigger('showAnnotation', annotation || id);
    },

    /**
     * Shows all annotations.
     *
     * Shows the comment text section of the annotation and the annotation color
     * icon, also displays the annotation shapes on the canvas for each
     * annotation.
     */
    showAllAnnotations: function () {
      var that = this;
      $(".canvas-annotation[annotation]", this.base).each(function () {
        var id = $(this).attr('annotation');
        that.showAnnotation(id);
      });
      // Open type containers as well.
      $('.comment-type-content', this.base).show();
    },

    /**
     * Hides the given annotation from view.
     *
     * @param {string} id
     *   The identifier for the annotation.
     */
    hideAnnotation: function (id) {
      var $annotation, canvas, annotation;
      $annotation = this.getAnnotationElement(id);
      $('.comment-text, span.color', $annotation).hide();
      $('.comment-show-hide', $annotation).text('+');
      $('.comment-title', $annotation).removeClass('annotation-opened');
      canvas = Drupal.IslandoraImageAnnotationCanvas.getInstance();
      annotation = Drupal.IslandoraImageAnnotation.getInstance().getAnnotation(id);
      if (annotation) {
        canvas.hideAnnotation(annotation);
      }
      Drupal.IslandoraImageAnnotation.trigger('hideAnnotation', annotation || id);
    },

    /**
     * Hides all annotations.
     *
     * Hides the comment text section of the annotation and the annotation color
     * icon, also the annotation shapes on the canvas for each annotation.
     */
    hideAllAnnotations: function () {
      var that = this;
      $(".canvas-annotation[annotation]", this.base).each(function () {
        var id = $(this).attr('annotation');
        that.hideAnnotation(id);
      });
      // Close type containers as well.
      $('.comment-type-content', this.base).hide();
    },

    /**
     * Toggles the display of the annotation identified by the given urn.
     *
     * @param id
     *   The id of the annotation to display.
     */
    toggleAnnotationDisplay: function (id) {
      var that = this;
      var $annotation = this.getAnnotationElement(id);
      $('.comment-text, span.color', $annotation).toggle();
      $('.comment-title', $annotation).toggleClass('annotation-opened');
      if ($('span.color', $annotation).is(':visible')) {
        $('.comment-show-hide', $annotation).text('-');
        that.showAnnotation(id);
      } else {
        $('.comment-show-hide', $annotation).text('+');
        that.hideAnnotation(id);
      }
    },

    /**
     * Checks if the given annotation is currently visible.
     *
     * @param id
     *   The id of the annotation to check.
     */
    isAnnotationDisplayed: function (id) {
      var $annotation = this.getAnnotationElement(id);
      return $('.comment-title', $annotation).hasClass('annotation-opened');
    },

    /**
     * Checks if any annotation is currently being displayed.
     */
    isAnyAnnotationDisplayed: function () {
      var that = this,
          any_displayed = false;
      $(".canvas-annotation[annotation]", this.base).each(function () {
        var id = $(this).attr('annotation');
        if (that.isAnnotationDisplayed(id)) {
          any_displayed = true;
          return false;
        }
      });
      return any_displayed;
    },

    /**
     * Gets the color of the given annotation.
     *
     * @param {RDF.BodyTarget} target
     *   The target to get the color from.
     *
     * @returns {string|null}
     *   The color in #ffffff format if found, otherwise null.
     */
    getAnnotationTargetColor: function (target) {
      if (target.constraint !== undefined && target.constraint.value !== undefined) {
        return $(target.constraint.value).attr('stroke');
      }
      return null;
    },

    /**
     * Creates an Annotation element to insert into the list.
     *
     * @param {RDF.Annotation} annotation
     *   The annotation in which we will render into HTML.
     *
     * @returns {jQuery}
     *   The HTML as a jQuery object.
     */
    createAnnotation: function (annotation) {
      var $annotation, $annotationTitle, $annotationText, canvas, entity_url;
      var that = this;
      canvas = annotation.getTargetCanvas();
      // Create annotation block.
      $annotation = $('<div />', {
        annotation: annotation.id,
        // Make the assumption that only one canvas will ever exist, and we'll
        // always have at least one target.
        canvas: canvas && canvas.id,
        class: 'canvas-annotation'
      });
      // Add Title, including the color icon.
      $annotationTitle = $('<div />', {
        class: 'comment-title'
      }).append('<span class="comment-show-hide">+</span>');
      $annotationTitle.append('<span>' + annotation.title + '</span>');
      // Append Color Icons
      $.each(annotation.targets, function (index, target) {
        var color = that.getAnnotationTargetColor(target);
        $annotationTitle.append($('<span />', {
          color: color,
          class: 'color',
          style: 'background:' + color + ';'
        }).hide());
      });
      // Display the annotation when the title is clicked.
      $annotationTitle.click(function () {
        that.toggleAnnotationDisplay(annotation.id);
      });
      // Add Text / Entity, initially hidden.
      $annotationText = $('<div class="comment-text" />').hide();
      $annotationText.append('<div class="comment-type">' + annotation.annotationType + '</div>');
      $annotationText.append('<div class="comment-content">' + annotation.body.value + '</div>');
      if (annotation.relation && annotation.hasPart) {
        entity_url = Drupal.settings.basePath + 'islandora/object/' + annotation.relation;
        $annotationText.append('<div class="comment-entity"><a href="' + entity_url + '">' + annotation.hasPart + '</a></div>');
      }
      $annotation = $annotation.append($annotationTitle, $annotationText);
      return $annotation;
    },

    /**
     * Adds the given annotation to the list.
     *
     * @param {RDF.Annotation} annotation
     *   The annotation in to add to the list.
     */
    addAnnotation: function (annotation) {
      var type, $container;
      type = annotation.annotationType;
      if (this.hasAnnotationTypeContainer(type)) {
        $container = this.getAnnotationTypeContainer(type);
      } else {
        $container = this.insertAnnotationTypeContainer(this.createAnnotationTypeContainer(type));
      }
      $('.comment-type-content', $container).append(this.createAnnotation(annotation));
    },

    /**
     * Deletes the given Annotation from Fedora and the interface.
     *
     * @param {string} id
     *   The id of the annotation.
     */
    deleteAnnotation: function (id) {
      $.ajax({
        type: 'POST',
        url: IIAUtils.url('islandora/object/' + this.settings.pid + '/annotation/delete/' + id),
        success: function () {
          Drupal.IslandoraImageAnnotation.getInstance().deleteAnnotation(id);
        }
      });
    },


    // Set up context menu to display for each annotation title element.
    createContextMenu: function () {
      var that = this;

      $.contextMenu({
        selector : '.comment-title',
        items: {
          edit: {
            name: 'Edit',
            icon: 'edit'
          },
          delete: {
            name : 'Delete annotation',
            icon : 'delete'
          }
        },
        callback: function (key) {
          var $title, $annotation, id, annotation, title;
          $title = $(this);
          $annotation = $title.parent('.canvas-annotation');
          id = $annotation.attr('annotation');
          title = $('span:nth-child(2)', $title).text().trim();

          switch (key) {
          case 'edit':
            that.showAnnotation(id);
            annotation = Drupal.IslandoraImageAnnotation.getInstance().getAnnotation(id);
            Drupal.IslandoraImageAnnotationDialog.getInstance().show(annotation);
            break;

          case 'delete':
            if (confirm("Permanently Delete Annotation '" + title + "'")) {
              that.deleteAnnotation(id);
            }
            break;
          }
        }
      });
    }
  });

  /**
   * Gets the global singleton of this class.
   *
   * This function is meant to be used after Drupal.attachBehaviors() has been
   * called, otherwise the singleton instance won't exist.
   *
   * @return {Drupal.IslandoraImageAnnotationList}
   *   The singleton
   */
  Drupal.IslandoraImageAnnotationList.getInstance = function () {
    return Drupal.IslandoraImageAnnotationList[base];
  };

}(jQuery));
