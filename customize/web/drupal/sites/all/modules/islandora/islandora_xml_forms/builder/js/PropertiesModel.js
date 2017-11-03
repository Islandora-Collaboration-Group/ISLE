/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
var types = Ext.data.Types; 

/**
 * Form Properties Model
 */
Ext.define('Properties', {
    extend: 'Ext.data.Model',
    proxy: {
        type: 'memory',
        reader: {
            type: 'json'
        }  
    },
    fields: [{
        name: 'localName',
        type: 'string'
    }, {
        name: 'prefix',
        type: 'string'
    }, {
        name: 'uri',
        type: 'string'  
    }, {
        name: 'namespaces',
        type: types.MAP
    }, {
        name: 'schema',
        type: 'string'
    }]
});

