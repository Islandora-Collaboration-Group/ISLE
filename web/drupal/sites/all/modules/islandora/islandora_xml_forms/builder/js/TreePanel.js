/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

Ext.formbuilder.createTreePanel = function() {
  return Ext.create('Ext.tree.Panel', {
    viewConfig: {
      plugins: {
        ptype: 'treeviewdragdrop'
      },
      style: {
        overflow: 'auto'
      }
    },
    title: Drupal.t('Elements'),
    store: this.elementStore,
    region: 'west',
    scroll: false,
    width: 230,
    height: 820,
    margin: '1 0 1 1',
    rootVisible: false,
    split: true,
    tbar: {
      xtype: 'toolbar',
      items: [{
        xtype: 'button',
        text: Drupal.t('Add'),
        handler: function() {
          var tree = Ext.formbuilder.treePanel;
          var selectionModel = tree.getSelectionModel();
          var selection = selectionModel.getSelection();
          if(selection.length > 0) {
            var element = new Element({
              children: []
            });
            var selected = selection[0];
            var node = selected.createNode(element);
            selected.appendChild(node);
            selected.expand();
            selectionModel.select(node);
          }
        }
      }, {
        xtype: 'button',
        text: Drupal.t('Copy'),
        handler: function() {
          var tree = Ext.formbuilder.treePanel;
          var selectionModel = tree.getSelectionModel();
          var selection = selectionModel.getSelection();
          if(selection.length > 0) {
            Ext.formbuilder.treePanel.clipboard = selection[0];
          }
        }
      }, {
        xtype: 'button',
        text: Drupal.t('Paste'),
        handler: function() {
          var tree = Ext.formbuilder.treePanel;
          var selectionModel = tree.getSelectionModel();
          var selection = selectionModel.getSelection();
          var source = Ext.formbuilder.treePanel.clipboard;
          if(selection.length > 0 && source) {
            var selected = selection[0];
            var node = source.copy();
            Ext.data.Model.id(node);
            selected.appendChild(node);
            selected.expand();
            selectionModel.select(node);
          }
        }
      }, {
        xtype: 'button',
        text: Drupal.t('Delete'),
        handler: function() {
          var tree = Ext.formbuilder.treePanel;
          var selectionModel = tree.getSelectionModel();
          var selection = selectionModel.getSelection();
          if(selection.length > 0) {
            var selected = selection[0];
            selected.remove(true);
          }
        }
      }]
    },
    listeners: {
      itemmousedown: function() {
        Ext.formbuilder.showElementForm();
      },
      selectionchange: function(view, selections) {
        Ext.formbuilder.saveElementForm();
        if(selections.length > 0) {
          var record = selections[0];
          var form = Ext.formbuilder.elementForm.getForm();
          //var data = Ext.clone(record.data);
          var data = record.data;
          form.reset();
          form.loadRecord(record);
          //attributes
          var form_grids = [ 'attributes', 'element_validate', 'process', 'pre_render', 'post_render', 'after_build', 'options', 'user_data', 'submit', 'validate'];
          Ext.Array.each(form_grids, function(name) {
            var converted = [];
            if(data[name] instanceof Object) {
              jQuery.each(data[name], function(i, n) {
                converted.push({
                  key: i,
                  value: n
                });
              });
            }
            else if(data[name] instanceof Array) {
              jQuery.each(data[name], function(i, n) {
                converted.push({
                  value: n
                });
              });
            }
            Ext.getCmp(name).store.loadData(converted, false);
          });
          /* Ahah */
          var ahah = data.ahah;
          if(ahah !== undefined && ahah != "") {
            var values = {
              ahah: "on",
              ahah_effect: ahah.effect,
              ahah_event: ahah.event,
              ahah_method: ahah.method,
              ahah_path: ahah.path,
              ahah_wrapper: ahah.wrapper,
              ahah_keypress: ahah.keypress
            };
            if(ahah.progress !== undefined && ahah.progress != "") {
              var progress = ahah.progress;
              values.ahah_progress = "on";
              values.ahah_progress_type = progress.type;
              values.ahah_progress_message = progress.message;
              values.ahah_progress_url = progress.url;
              values.ahah_progress_interval = progress.interval;
            }
            else {
              Ext.getCmp('ahah_progress').collapse();
            }
            form.setValues(values);
          }
          else {
            Ext.getCmp('ajax').collapse();
            Ext.getCmp('ahah_progress').collapse();
          }
          var actions = data.actions;
          if(actions !== undefined && actions != "") {
            if(actions.create !== undefined && actions.create != "" && actions.create !== null) {
              var create = actions.create;
              var values = {
                actions_create: "on",
                actions_create_context: create.context,
                actions_create_path: create.path,
                actions_create_schema: create.schema,
                actions_create_type: create.type,
                actions_create_value: create.value
              };
              form.setValues(values);
            }
            else {
              Ext.getCmp('actions_create').collapse();
            }
            if(actions.read !== undefined && actions.read != "" && actions.read !== null) {
              var read = actions.read;
              var values = {
                actions_read: "on",
                actions_read_context: read.context,
                actions_read_path: read.path
              };
              form.setValues(values);
            }
            else {
              Ext.getCmp('actions_read').collapse();
            }
            if(actions.update !== undefined && actions.update != "" && actions.update !== null) {
              var update = actions.update;
              var values = {
                actions_update: "on",
                actions_update_context: update.context,
                actions_update_path: update.path,
                actions_update_schema: update.schema
              };
              form.setValues(values);
            }
            else {
              Ext.getCmp('actions_update').collapse();
            }
            if(actions['delete'] !== undefined && actions['delete'] != "" && actions['delete'] !== null) {
              var remove = actions['delete'];
              var values = {
                actions_delete: "on",
                actions_delete_context: remove.context,
                actions_delete_path: remove.path
              };
              form.setValues(values);
            }
            else {
              Ext.getCmp('actions_delete').collapse();
            }
          }
          else {
            Ext.getCmp('actions_create').collapse();
            Ext.getCmp('actions_read').collapse();
            Ext.getCmp('actions_update').collapse();
            Ext.getCmp('actions_delete').collapse();
          }

        }
        // Load by name...
        Ext.formbuilder.enableDisableXMLFields();
        Ext.formbuilder.showElementForm();
      }
    }
  });
}
