/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * 
 */
Ext.formbuilder.createPropertiesForm = function() {
    return Ext.create('Ext.form.Panel', {
        title: Drupal.t('Properties Form'),
        region: 'center',
        margin: '1 1 1 0',
        frame: true,
        items:  [{
            xtype: 'fieldset',
            title: Drupal.t('Root Element'),
            items: [{
                xtype: 'textfield',
                id: 'localName',
                name: 'localName',
                fieldLabel: Drupal.t('Root Element Name'),
                allowBlank: false,
                anchor: '100%'
            },{
                xtype: 'textfield',
                id: 'uri',
                name: 'uri',
                fieldLabel: Drupal.t('Namespace URI'),
                anchor: '100%'
            }]
        },{
            xtype: 'fieldset',
            title: Drupal.t('Schema'),
            items: [{
                xtype: 'textfield',
                id: 'schema',
                name: 'schema',
                fieldLabel: Drupal.t('Name'),
                anchor: '100%'
            }]
        },{
            xtype: 'formgrid',
            title: Drupal.t('Namespaces'),
            id: 'namespaces',
            name: 'namespaces',
            height: 300,
            store: this.createMapStore(),
            modelInitTmpl: {
                key: '', 
                value: ''
            },
            columns: [{
                xtype: 'gridcolumn',
                dataIndex: 'key',
                header: Drupal.t('Prefix'),
                sortable: true,
                width: 150,
                field: {
                    type: 'textfield'
                }
            },{
                xtype: 'gridcolumn',
                dataIndex: 'value',
                header: Drupal.t('URI'),
                sortable: true,
                flex: 1,
                field: {
                    type: 'textfield'
                }
            }]
        }],
        listeners: {
            hide: function() {
                Ext.formbuilder.savePropertiesForm();
            }     
        }
    });
};
