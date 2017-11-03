/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * A grid that can be used to enter information in a form. 
 * Requires some extra logic to be populated and submitted.
 */
Ext.define('Form.Grid', {
  extend: 'Ext.grid.Panel',
  alias: 'widget.formgrid',
  initComponent: function() {
    var me = this;
    Ext.apply(me, {
      height: 200,
      collapsible: true,
      iconCls: 'icon-grid',
      selType: 'rowmodel',
      plugins:[ Ext.create('Ext.grid.plugin.RowEditing', {
        clicksToEdit: 2
      }) ],
      viewConfig: {
        plugins: {
          ptype: 'gridviewdragdrop',
          dragText: Drupal.t('Drag and drop to reorganize')
        }
      },
      dockedItems: [{
        xtype: 'toolbar',
        items: [{
          iconCls: 'icon-add',
          text: Drupal.t('Add'),
          scope: this,
          handler: function() {
            var rec = Ext.ModelManager.create(me.modelInitTmpl, me.store.model.modelName);
            var plugin = me.getPlugin();
            plugin.cancelEdit();
            this.store.insert(0, rec);
            plugin.startEdit(rec, me.columns[0]);
          }
        }, {
          iconCls: 'icon-delete',
          text: Drupal.t('Delete'),
          disabled: true,
          itemId: 'delete',
          scope: this,
          handler: function() {
            var selection = me.getView().getSelectionModel().getSelection()[0];
            if (selection) {
              this.store.remove(selection);
            }
          }
        }]
      }],
      listeners: {
        selectionchange: function(selModel, selections) {
          this.down('#delete').setDisabled(selections.length === 0);
        }
      }
    });
    me.callParent();
  }
});
