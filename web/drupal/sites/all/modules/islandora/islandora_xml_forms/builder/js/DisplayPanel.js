/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */


/**
 * 
 */
Ext.formbuilder.createDisplayPanel = function (children) {
    return Ext.create('Ext.panel.Panel', {
        id: 'xml-form-builder-display',
        region: 'center',
        layout: 'card',
        margin: '1 1 1 0',
        activeItem: 2,
        unstyled: true,
        defaults: {
            bodyStyle: 'padding:15px'
        },
        items: children
    });
}