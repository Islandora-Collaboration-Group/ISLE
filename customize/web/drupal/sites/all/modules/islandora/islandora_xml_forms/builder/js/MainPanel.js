/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Create the Application
 */
Ext.formbuilder.save = function(showPreview) {
    Ext.formbuilder.savePropertiesForm();
    Ext.formbuilder.saveElementForm();
    var url = window.location.pathname + '/save';
    var properties_record = Ext.formbuilder.propertiesStore.getAt(0);
    var data = {
        properties: properties_record.data, 
        elements: []
    };
    var root = Ext.formbuilder.elementStore.getRootNode();//.getChildAt(0);
    root.eachChild(function(child) {
        var elements = this;
        elements.push(child.data);
        var last = elements.length-1;
        this[last].elements = [];
        child.eachChild(arguments.callee, elements[last].elements);
    }, data.elements);
    Ext.Ajax.request({
        url: url,
        params: {
            data: Ext.encode(data)
        },
        success: function(response) {
            if(showPreview) {
                Ext.formbuilder.showPreview();
            }
        }
    });
};

/**
 * Create
 */
Ext.formbuilder.createMainPanel = function(children){
    return Ext.create('Ext.panel.Panel', {
        width: 960,
        height: 820,
        title: Drupal.t('Form Editor'),
        layout: 'border',
        renderTo: 'xml-form-builder-editor',
        items: children,
        defaults: {
            margin: '1 0 1 0',
            frame: true
        },
        tbar: {
            xtype: 'toolbar',
            items: [{
                xtype: 'button',
                text: Drupal.t('Form Properties'),
                handler: function() {
                    Ext.formbuilder.showPropertiesForm();
                }
            },{
                xtype: 'tbfill'
            },{
                xtype: 'button',
                text: Drupal.t('Save & Preview'),
                handler: function() {
                    Ext.formbuilder.save(true);
                }
            },{
                xtype: 'tbseparator'
            },{
                xtype: 'button',
                text: Drupal.t('Save'),
                handler: function() {
                    Ext.formbuilder.save(false);
                }
            }]
        }
    });
};
