/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 * Create Namespace for this application.
 */
Ext.ns('Ext.formbuilder');

/**
 * Application Object.
 */
Ext.formbuilder = (function() {
  var url = window.location.pathname; // Private Variable.
  var that = {
    /**
         * Create all the components required to render this application.
         */
    create: function() {
      /* Create Display Panel: Card Layout */
      this.propertiesForm = this.createPropertiesForm(); // Defined in PropertiesForm.js
      this.elementForm = this.createElementForm(); // Defined in ElementForm.js
      this.previewPanel = this.createPreviewPanel(url); // Defined in PreviewPanel.js
      this.displayPanel = this.createDisplayPanel([this.previewPanel, this.elementForm, this.propertiesForm]); // Defined in DisplayPanel.js
      this.displayPanel.activeItem = 0;// TODO: remove.
      /* Create Tree Panel */
      this.treePanel = this.createTreePanel(); // Defined in TreePanel.js
      this.treePanel.expandAll();
      /* Create Main Panel */
      this.mainPanel = this.createMainPanel([this.treePanel, this.displayPanel]); // Defined in MainPanel.js
    //this.createToolTips(); // Defined in Tooltips.js
    },
    /**
         * Creates an array based store.
         */
    createArrayStore: function () {
      return new Ext.data.Store({
        model: 'ArrayModel'
      });
    },
    /**
         * Creates a map based store.
         */
    createMapStore: function () {
      return new Ext.data.Store({
        model: 'MapModel'
      });
    },
    /**
     *
     */
    showPreview: function () {
      this.refreshPreviewPanel(url);
    },
    /**
     *
     */
    showElementForm: function () {
      var display = this.displayPanel.layout;
      display.setActiveItem(1);
      var element = Ext.getCmp('xml-form-builder-element-form-tab-panel');
      element.setActiveTab(0);
    },
    /**
     *
     */
    showPropertiesForm: function () {
      var display = this.displayPanel.layout;
      display.setActiveItem(2);
      var record = Ext.formbuilder.propertiesStore.getAt(0);
      var form = this.propertiesForm.getForm();
      form.loadRecord(record);
      var namespaces = [];
      if(record.data['namespaces'] instanceof Object) {
        jQuery.each(record.data['namespaces'], function(i, n) {
          namespaces.push({
            key: i,
            value: n
          });
        });
      }
      Ext.getCmp('namespaces').store.loadData(namespaces, false);
    },
    saveElementForm: function() {
      var form = this.elementForm.getForm();
      if (form.isValid()) {
        var record = form.getRecord();
        record.beginEdit();
        if(record.store === undefined) { // Store is undefined when the element is removed.
          return;
        }
        // Normal Form Fields
        var values = form.getValues();
        for(var i in values) {
          if(record.data[i] !== undefined) {
            record.set(i, values[i]);
          }
        }
        if(values['required'] === undefined) { // Hack
          record.set('required', false);
        }
        /* Form Array Grids */
        var form_array_grids = [ 'element_validate', 'process', 'pre_render', 'post_render', 'after_build', 'submit', 'validate' ];
        var toArray = function(store) {
          var output = [];
          store.each(function(item){
            item = item.data;
            output.push(item.value);
          });
          return output;
        }
        Ext.Array.each(form_array_grids, function(name) {
          var store = Ext.getCmp(name).store;
          record.set(name, toArray(store));
        });
        /* Form Map Grids */
        var form_map_grids = [ 'attributes', 'options', 'user_data' ];
        var toObject = function(store) {
          var output = {};
          store.each(function(item){
            item = item.data;
            output[item.key] = item.value;
          });
          return output;
        }
        Ext.Array.each(form_map_grids, function(name) {
          var store = Ext.getCmp(name).store;
          record.set(name, toObject(store));
        });
        /* Ahah */
        if(values['ahah'] == "on") {
          var ahah = {
            effect: values['ahah_effect'],
            event: values['ahah_event'],
            method: values['ahah_method'],
            path: values['ahah_path'],
            wrapper: values['ahah_wrapper'],
            keypress: values['ahah_keypress']
          };
          if(values['ahah_progress'] == "on") {
            ahah.progress = {
              type: values['ahah_progress_type'],
              message: values['ahah_progress_message'],
              url: values['ahah_progress_url'],
              interval: values['ahah_progress_interval']
            };
          }
          record.set('ahah', ahah);
        }
        var actions = {};
        var has_actions = false;
        if(values['actions_create'] == "on" && !form.findField('actions_create').disabled) {
          has_actions = true;
          actions.create = {
            context: values['actions_create_context'],
            path: values['actions_create_path'],
            schema: values['actions_create_schema'],
            type: values['actions_create_type'],
            value: values['actions_create_value']
          };
        }
        if(values['actions_read'] == "on" && !form.findField('actions_create').disabled) {
          has_actions = true;
          actions.read = {
            context: values['actions_read_context'],
            path: values['actions_read_path']
          };
        }
        if(values['actions_update'] == "on" && !form.findField('actions_create').disabled) {
          has_actions = true;
          actions.update = {
            context: values['actions_update_context'],
            path: values['actions_update_path'],
            schema: values['actions_update_schema']
          };
        }
        if(values['actions_delete'] == "on" && !form.findField('actions_create').disabled) {
          has_actions = true;
          actions['delete'] = {
            context: values['actions_delete_context'],
            path: values['actions_delete_path']
          };
        }
        if(has_actions) {
          record.set('actions', actions);
        }
        else {
          record.set('actions', undefined);
        }
        record.set('text', values.key + ' (' + values.type + ')');
        record.endEdit();
        record.commit();
        record.store.sync();
        Ext.formbuilder.elementStore.sync();
      }
    },
    savePropertiesForm: function() {
      var form = this.propertiesForm.getForm();
      if (form.isValid()) {
        var record = form.getRecord();
        // Start
        record.beginEdit();
        // Normal form fields
        var values = form.getValues();
        for(var i in values) {
          record.set(i, values[i]);
        }
        // Grids
        var toObject = function(store) {
          var output = {};
          store.each(function(item){
            item = item.data;
            output[item.key] = item.value;
          });
          return output;
        }
        var store = Ext.getCmp('namespaces').store;
        record.set('namespaces', toObject(store));
        // End
        record.endEdit();
      }
    }
  };
  return that;
})();

/**
* Run the application.
*/
Ext.onReady(function() {
  Ext.formbuilder.create();
});
