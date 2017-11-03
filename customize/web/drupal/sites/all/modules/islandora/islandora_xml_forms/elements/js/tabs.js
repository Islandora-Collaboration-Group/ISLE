/**
 * @file
 * Tabs controls.
 */

Drupal.behaviors.xmlFormElementTabs = {
  tabs: {
    tool_tip: null,
    tabs: null, // Collection of all tabpanels.
    collapsibleTabs: null,
    nonCollapsibleTabs: null,
    loadPanels: function(collapse, context) {
      var load = '.xml-form-elements-tabs:not(.processed)';
      var collapsible = '.xml-form-elements-tabs-collapsible';
      var collapsed = '.xml-form-elements-tabs-collapsed';
      this.tabs = jQuery(load);
      this.collapsibleTabs = this.tabs.filter(collapsible);
      this.nonCollapsibleTabs = this.tabs.not(collapsible);
      var expandedTabs = this.collapsibleTabs.not(collapsed);
      var collapsedTabs = this.collapsibleTabs.filter(collapsed);
      if (collapsedTabs.length > 0) {
        collapsedTabs.tabs({
          collapsible: true,
          selected: collapse ? -1 : undefined,
          select: this.setCollapsibleIconOnSelect,
          create: this.setCollapsibleIconOnCreate
        });
      }
      if (expandedTabs.length > 0) {
        expandedTabs.tabs({
          collapsible: true,
          select: this.setCollapsibleIconOnSelect,
          create: this.setCollapsibleIconOnCreate
        });
      }
      if (this.nonCollapsibleTabs.length > 0) {
        this.nonCollapsibleTabs.tabs({});
      }
      this.tabs.each(function() {
        jQuery(this).tabs({
          selected: jQuery(this).find('li').length - 1
        });
      });
    },
    setCollapsibleIconOnSelect: function(event, ui) {
      var icon = jQuery('span.expand-tabpanel-icon:first', this);
      if (jQuery(ui.panel).hasClass('ui-tabs-hide')) {
        icon.removeClass('ui-icon-circle-triangle-e');
        icon.addClass('ui-icon-circle-triangle-s');
      }
      else {
        icon.removeClass('ui-icon-circle-triangle-s');
        icon.addClass('ui-icon-circle-triangle-e');
      }
    },
    setCollapsibleIconOnCreate: function(event, ui) {
      var icon = jQuery('span.expand-tabpanel-icon:first', this);
      if (jQuery('div.ui-tabs-panel:not(.ui-tabs-hide)', this).length > 0) {
        icon.removeClass('ui-icon-circle-triangle-e');
        icon.addClass('ui-icon-circle-triangle-s');
      }
      else {
        icon.removeClass('ui-icon-circle-triangle-s');
        icon.addClass('ui-icon-circle-triangle-e');
      }
    },
    attachToolTips: function() {
      jQuery('.tool_tip_trigger').once(function() {
        jQuery(this).hover(function(e) {
          var html = '';// + i + '<br/>';
          var id = jQuery(this).children('a[href]').attr('href');
          jQuery(id + ' div.form-item').each(function() {
            var item = jQuery(this);
            jQuery('> input[type~="text"]', item).each(function(i, text) {
              var id = jQuery(text).attr('id');
              var label = jQuery('label[for="' + id + '"]', item);
              if (label.length > 0) {
                label = label.text().trim();
                var textOut = jQuery(text).val();
                jQuery('input[class~="form-tag"]', jQuery(text).parent()).each(function() {
                  var tag = jQuery(this);
                  textOut += ' ' + tag.val();
                });
                textOut = jQuery.trim(textOut);
                if (textOut.length > 0) {
                  html += label + ': ' + textOut + '<br/>';
                }
              }
            });

            jQuery('> select', item).each(function(index, select) {
              var id = jQuery(select).attr('id');
              var label = jQuery('label[for=' + id + ']');
              if (label.length > 0) {
                label = label.text().trim();
                html += label + ': ';
              }
              jQuery('option:selected', select).each(function(idx, selected) {
                html += jQuery(selected).text().trim() + '<br/>';
              });
            });
          });
          html = jQuery.trim(html);
          if (html == "") {
            html = Drupal.t("Empty");
          }

          if (Drupal.behaviors.xmlFormElementTabs.tabs.tool_tip != null) {
            Drupal.behaviors.xmlFormElementTabs.tabs.tool_tip.remove();
          }
          else {
            Drupal.behaviors.xmlFormElementTabs.tabs.tool_tip = jQuery(document.createElement('span')).addClass('tool_tip');
          }

          Drupal.behaviors.xmlFormElementTabs.tabs.tool_tip.html(html);

          var x = e.pageX + 20,
              y = e.pageY + 20,
              w = Drupal.behaviors.xmlFormElementTabs.tabs.tool_tip.width(),
              h = Drupal.behaviors.xmlFormElementTabs.tabs.tool_tip.height(),
              dx = jQuery(window).width() - (x + w),
              dy = jQuery(window).height() - (y + h);
          if (dx < 20)
            x = e.pageX - w - 20;
          if (dy < 20)
            y = e.pageY - h - 20;
          Drupal.behaviors.xmlFormElementTabs.tabs.tool_tip.css({
            'left': x,
            'top': y
          });

          Drupal.behaviors.xmlFormElementTabs.tabs.tool_tip.appendTo('body');
        },
            function() {
              if (Drupal.behaviors.xmlFormElementTabs.tabs.tool_tip != null) {
                Drupal.behaviors.xmlFormElementTabs.tabs.tool_tip.remove();
              }
            });
      });
    },
    enableActions: function() {
      var icons = jQuery(".ui-icon-close:not(.processed)");
      icons.click(function() {
        jQuery("#" + jQuery(this).text()).trigger("mousedown");
      });
      icons.addClass('processed');
    }
  },
  attach: function(context, settings) {
    this.tabs.loadPanels(true, context);
    this.tabs.attachToolTips();
    this.tabs.enableActions();

  }
}
