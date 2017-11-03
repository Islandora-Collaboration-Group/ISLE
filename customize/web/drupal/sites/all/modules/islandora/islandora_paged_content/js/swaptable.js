(function ($) {
  "use strict";

  /**
   * jQuery Helper function for determining order.
   *
   * Very dumb method doesn't test for the same element or not siblings etc.
   */
  $.fn.isAfter = function (sel) {
    return this.index() > $(sel).index();
  };

  /**
   * jQuery Helper function for determining order.
   *
   * Very dumb method doesn't test for the same element or not siblings etc.
   */
  $.fn.isBefore = function (sel) {
    return !this.isAfter(sel);
  };

  /**
   * Two tables with drag and drop rows to move rows between them.
   */
  Drupal.behaviors.swapTable = {
    attach: function (context, settings) {
      var base,
        constructSwapTable = function () {
          // Create the new swapTable instance. Save in the Drupal variable
          // to allow other scripts access to the object.
          Drupal.swapTable[this.id] = new Drupal.swapTable(this, settings.swapTable[this.id]);
        };
      for (base in settings.swapTable) {
        if (settings.swapTable.hasOwnProperty(base)) {
          $('#' + base, context).once('swaptable', constructSwapTable);
        }
      }
    }
  };

  /**
   * Constructor for the swapTable object.
   *
   * @param wrapper
   *   The root element in the template.
   * @param settings
   *   Settings for the swap tables.
   */
  Drupal.swapTable = function (table, settings) {
    var self = this, name = settings.name;

    // Required object variables.
    this.table = table;
    this.form = $(table).parents('form');
    this.order = $('input[name="' + name + '[order]"]', table)[0];
    this.modified = $('input[name="' + name + '[modified]"]', table)[0];
    this.load = $('input[name="op"]', table)[0];
    this.tables = $('table.swaptable', this.table);
    this.left = this.tables[0];
    this.right = this.tables[1];
    this.page = {
      left: $('input[name="' + name + '[page][left]"]', table).val(),
      right: $('input[name="' + name + '[page][right]"]', table).val()
    };
    this.limit = $('select[name="' + name + '[display]"]').val();
    this.settings = settings;

    // Change all pager links so they redirect to '#'.
    // There is a problem in that the overview display will hijack any link that
    // links to a admin section of the site so even if we prevent the default
    // event on click the overview panel will perform the request anyways.
    $('.pager a', this.table).each(function() {
      // Store the page parameters for later loading.
      var results = this.href.match('\\?.*page=([^&]*)');
      if ($.isArray(results) && results[1] !== undefined) {
        $.data(this, 'pages', results[1]);
      }
      this.href = "#";
    }).bind('click', function (e) {
      e.preventDefault();
      // Change the url to have the pager parameters.
      self.reload($.data(this, 'pages'));
    });

    // Before any ajax elements within this form serialize the form values
    // make sure we update the hidden value with the latest ordering and
    // modifications.
    $('.ajax-processed', this.form).each(function () {
      Drupal.ajax[this.id].beforeSerialize = function (element, options) {
        self.serializeOrderAndModifications();
        Drupal.ajax.prototype.beforeSerialize.call(this, element, options);
      };
    });

    // Always ensure the hidden fields has the latest ordering/modifications
    // before submitting.
    this.form.submit(function () {
      self.serializeOrderAndModifications();
      return true;
    });

    // Add the actual ID of the row to each row in left tables.
    // ID's are assumed to be unique and also assumed not to contain spaces.
    $('tbody > tr', this.left).each(function (index) {
      index = (self.limit * self.page.left) + index;
      $.data(this, 'id', settings.order[index]);
    });

    // Add the actual ID of the row to each row in right tables.
    // ID's are assumed to be unique and also assumed not to contain spaces.
    $('tbody > tr', this.right).each(function (index) {
      index = (self.limit * self.page.right) + index;
      $.data(this, 'id', settings.order[index]);
    });

    // Add a link before the table for users to show or hide weight columns.
    $('<a href="#" class="swaptable-toggle-original-ordering"></a>')
      .attr('title', Drupal.t('Show the original order of items.'))
      .click(function () {
        if ($.cookie('Drupal.swapTable.showOriginal') === 1) {
          self.hideColumns();
        } else {
          self.showColumns();
        }
        return false;
      })
      .wrap('<div class="swaptable-toggle-original-ordering-wrapper"></div>')
      .parent()
      .insertBefore(this.table);

    // Initialize the specified columns.
    self.initColumns();

    // Create placeholders to model empty spaces when dragging the selected
    // rows above the tables.
    this.createPlaceholder(this.left);
    this.createPlaceholder(this.right);

    // Listen to click events to select rows in the left table.
    $('tbody > tr', this.left).bind("click", function (event) {
      self.selectRow(event, self.left, this);
    });

    // Listen to click events to select rows in the right table.
    $('tbody > tr', this.right).bind("click", function (event) {
      self.selectRow(event, self.right, this);
    });

    // Configure dragging for both tables, ignore placeholders as they can't be
    // dragged.
    $('tbody > tr:not(.ui-placeholder)', this.tables).draggable({
      appendTo: 'body',
      cursor: "move",
      revert: "invalid",
      cursorAt: { top: 0, left: 0 },
      refreshPositions: true,
      containment: this.table,
      opacity: 0.7,
      helper: function (event) {
        var selected = $(event.currentTarget).siblings('.ui-selected').andSelf().addClass('ui-selected'),
          content = $(selected[0]).clone(false).removeAttr('id').removeClass('ui-droppable'),
          helper = $('<div class="ui-helper" style="position: absolute;"/>');
        $('.ordering', content).hide();
        $('.original-ordering', content).text('');
        if (selected.length > 1) {
          $('.original-ordering', content).text(Drupal.t('Multiple Selected: #') + selected.length);
          $('.original-ordering', content).removeAttr('class').show();
        }
        $('td:not(.ordering, .original-ordering)', content).each(function () {
          var style = { 'display': 'inline-block', 'vertical-align': 'middle' },
            span = $('<span/>')
              .addClass($(this).attr('class'))
              .html($(this).html())
              .css(style);
          helper.append(span);
        });
        helper.appendTo('body');
         // Center the cursor on the object.
        $(this).draggable("option", "cursorAt", {
          left: Math.floor(helper.width() / 2),
          top: Math.floor(helper.height() / 2)
        });
        return helper;
      },
      start: function (event, ui) {
        var table = $(this).parents('table')[0],
          otherTable = (table === self.left) ? self.right : self.left,
          selected = $(this).siblings('.ui-selected').andSelf();
        // Cancel selections in other table now that dragging has begun.
        $('tr.ui-selected', otherTable).removeClass('ui-selected');
        self.selectElements(this, selected);
        // Move the place holder to the selected row.
        $(this).before($('.ui-placeholder', table).show());
        // Hide the current selection until it's dragged outside of this table.
        selected.hide();
        // Update the ordering to account for the selected elements.
        self.updateOrdering(table);
        self.updateOrdering(otherTable);
      },
      stop: function (event, ui) {
        // Hide all place holders.
        $('.ui-placeholder', self.table).hide();
        // Show all rows.
        $('tr:hidden:not(.ui-placeholder)', self.tables).show();
        // Deselect everything.
        $('tr.ui-selected', self.tables).removeClass('ui-selected');
        // Forget what was selected.
        self.select(this, null);
        // Correct ordering for both tables.
        self.updateOrdering(self.left);
        self.updateOrdering(self.right);
      }
    });
    // Configure drop locations, ignore placeholder elements as they need to
    // ignore the over events.
    $('tbody > tr:not(.ui-placeholder)', this.tables).droppable({
      tolerance: 'intersect',
      over: function (event, ui) {
        var table = $(this).parents('table')[0],
          selected = self.selected(ui.draggable[0]);
        self.overTable(table, selected);
        self.movePlaceholder(table, this);
        self.updateOrdering(table, selected);
      },
      drop: function (event, ui) {
        var table = $(this).parents('table')[0],
          selected = self.selected(ui.draggable[0]);
        self.drop(table, this, selected);
      }
    });
  };

  /**
   * Triggers and hidden submit button that uses ajax to rebuild the page.
   */
  Drupal.swapTable.prototype.reload = function (pages) {
    var id = $(this.load).attr('id');
    pages = pages || this.page.left + ',' + this.page.right;
    Drupal.ajax[id].options.url += '?page=' + pages;
    $(this.load).trigger('mousedown');
  };

  /**
   * Serializes the current order of the rows as well as any modifications made.
   */
  Drupal.swapTable.prototype.serializeOrderAndModifications = function () {
    $(this.order).val(this.settings.order.join(' '));
    $(this.modified).val(this.settings.modified.join(' '));
  };

  /**
   * Marks the given ids as selected for the given draggable element.
   */
  Drupal.swapTable.prototype.select = function (draggable, ids) {
    $.data(draggable, 'selected', ids);
  };

  /**
   * Gets the selected elements.
   */
  Drupal.swapTable.prototype.selected = function (draggable) {
    return $.data(draggable, 'selected').get();
  };

  /**
   * Meant to be called in the context of the element.
   */
  Drupal.swapTable.prototype.getID = function () {
    return $.data(this, 'id');
  };

  /**
   * Selects the given elements and saves them with the given draggable element.
   */
  Drupal.swapTable.prototype.selectElements = function (draggable, selected) {
    this.select(draggable, selected.map(this.getID));
  };

  /**
   * Gets all the given elements within the given table.
   */
  Drupal.swapTable.prototype.getSelectedElements = function (table, selected) {
    return $('tr', table).filter(function () {
      return $.inArray($.data(this, 'id'), selected) !== -1;
    });
  };

  /**
   * Selection is "stored" by attaching the class 'ui-selected' to rows.
   *
   * @todo Store list and forget conditionally when doing one multi-select
   * immediately after the other, just like on OSX.
   */
  Drupal.swapTable.prototype.selectRow = function (event, table, row) {
    // Store the last selected row for building the multi-select list.
    var last = $.data(table, 'last'),
      id = '#' + row.id,
      list;
    // Shift indicates an intention to select multiple.
    if (event.shiftKey && !event.metaKey && last) {
      list = $(row).isAfter($(last)) ? $(last).nextUntil(id) : $(last).prevUntil(id);
      list.add(row).addClass('ui-selected');
    } else if (event.metaKey) {
      // Select/Deselect only the current selection.
      $(row).toggleClass('ui-selected');
      $.data(table, 'last', row);
    } else {
      // Select only the given row.
      $(row).addClass('ui-selected').siblings().removeClass('ui-selected');
      $.data(table, 'last', row);
    }
  };

  /**
   * Drop callback for droppable elements.
   */
  Drupal.swapTable.prototype.drop = function (table, row, selected) {
    var self = this,
      page = (table === this.left) ? this.page.left : this.page.right,
      row_index = $(row).parent().children(':visible').index(row),
      index = (this.limit * page) + row_index,
      args = [index, 0].concat(selected),
      order;
    // Remove the selected items before reinserting them.
    order = $.grep(this.settings.order, function (element, index) {
      return $.inArray(element, selected) === -1;
    });
    Array.prototype.splice.apply(order, args);
    this.settings.order = order;
    this.settings.modified = this.settings.modified.concat(selected);
    this.settings.modified = $.grep(this.settings.modified, function (v, k) {
      return $.inArray(v, self.settings.modified) === k;
    });
    this.reload();
  };

  /**
   * Callback for when a droppable element is hovering over the given table.
   */
  Drupal.swapTable.prototype.overTable = function (table, selected) {
    var otherTable = (table === this.left) ? this.right : this.left;
    // Show the currently selected rows only if they belong to the other table.
    this.getSelectedElements(table, selected).hide();
    $('tr:hidden', otherTable).show();
    // Show the place holder on only the current table
    $('tr.ui-placeholder', table).show();
    $('tr.ui-placeholder', otherTable).hide();
  };

  /**
   * Creates a place holder element for the given table.
   */
  Drupal.swapTable.prototype.createPlaceholder = function (table) {
    var self = this;
    $('<td class="empty"/>')
      .attr('colspan', $('th', table).length - 2)
      .wrap('<tr class="ui-placeholder">')
      .parent()
      .prepend('<td class="ordering">-1</td><td class="original-ordering"/>')
      .hide()
      .droppable({
        drop: function (event, ui) {
          var table = $(this).parents('table')[0],
            selected = self.selected(ui.draggable[0]);
          self.drop(table, this, selected);
        }
      }).prependTo($('tbody', table));
  };

  /**
   * Move the place holder to before or after the given row.
   */
  Drupal.swapTable.prototype.movePlaceholder = function (table, row) {
    // Move placeholder to current drop point.
    var placeholder = $('.ui-placeholder', table),
      list = $('tbody > tr:visible', table);
    if (list.index(row) > list.index(placeholder)) {
      $(row).after(placeholder);
    } else {
      $(row).before(placeholder);
    }
  };

  /**
   * Update the ordering and classes for the given table.
   */
  Drupal.swapTable.prototype.updateOrdering = function (table, selected) {
    this.updateTableOrdering(this.left, selected);
    this.updateTableOrdering(this.right, selected);
  };

  /**
   * Update the ordering and classes for the given table.
   */
  Drupal.swapTable.prototype.updateTableOrdering = function (table, selected) {
    var page = (table === this.left) ? this.page.left : this.page.right,
      index = (this.limit * page),
      count = index + 1;
    selected = selected || [];
    $('tbody > tr:visible', table).each(function (index) {
      // Restripe the table.
      $(this).removeClass('odd even').addClass(index % 2 ? 'even' : 'odd');
      // Reorder the displayed values to always be in order.
      $('td.ordering', this).text(count);
      count += $(this).hasClass('ui-placeholder') ? selected.length : 1;
    });
  };

  /**
   * Initialize columns containing form elements to be hidden by default.
   *
   * Identify and mark each cell with a CSS class so we can easily toggle
   * show/hide it. Finally, hide columns if user does not have a
   * 'Drupal.swapTable.showOriginal' cookie.
   */
  Drupal.swapTable.prototype.initColumns = function () {
    // Now hide cells and reduce colspans unless cookie indicates previous choice.
    // Set a cookie if it is not already present.
    if ($.cookie('Drupal.swapTable.showOriginal') === null) {
      $.cookie('Drupal.swapTable.showOriginal', 0, {
        path: Drupal.settings.basePath,
        // The cookie expires in one year.
        expires: 365
      });
      this.hideColumns();
    } else {
      // Check cookie value and show/hide weight columns accordingly.
      if ($.cookie('Drupal.swapTable.showOriginal') === 1) {
        this.showColumns();
      } else {
        this.hideColumns();
      }
    }
  };

  /**
   * Hide the columns containing weight/parent form elements.
   * Undo showColumns().
   */
  Drupal.swapTable.prototype.hideColumns = function () {
    // Hide weight/parent cells and headers.
    $('.swaptable-hide', '.swaptable-processed').hide();
    // Change link text.
    $('.swaptable-toggle-original-ordering').text(Drupal.t('Show original order'));
    // Change cookie.
    $.cookie('Drupal.swapTable.showOriginal', 0, {
      path: Drupal.settings.basePath,
      // The cookie expires in one year.
      expires: 365
    });
  };

  /**
   * Show the columns containing weight/parent form elements
   * Undo hideColumns().
   */
  Drupal.swapTable.prototype.showColumns = function () {
    // Show weight/parent cells and headers.
    $('.swaptable-hide', '.swaptable-processed').show();
    // Change link text.
    $('.swaptable-toggle-original-ordering').text(Drupal.t('Hide original order'));
    // Change cookie.
    $.cookie('Drupal.swapTable.showOriginal', 1, {
      path: Drupal.settings.basePath,
      // The cookie expires in one year.
      expires: 365
    });
  };
}(jQuery));
