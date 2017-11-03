/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

Ext.formbuilder.enableDisableXMLFields = function() {
  var actions = [
    'actions_create',
    'actions_read',
    'actions_update',
    'actions_delete'
  ];
  var not_supported = [
    'checkbox', 'checkboxes', 'date', 'file', 'managed_file',
    'password_confirm', 'radio', 'radios', 'tableselect',
    'vertical_tabs', 'weight', 'button', 'image_button', 'submit'
  ];
  var value = Ext.getCmp('type').getValue();
  if (Ext.Array.contains(not_supported, value) ||
      (value == 'select' && Ext.getCmp('multiple').getValue() == true)) {
    Ext.Array.each(actions, function(action)  {
      Ext.getCmp(action).cascade(function(c) {
        if (c.isFormField)
          c.disable();
      }).disable();
    });
  }
  else {
    Ext.Array.each(actions, function(action)  {
      Ext.getCmp(action).cascade(function(c) {
        if (c.isFormField)
          c.enable();
      }).enable();
    });
  }
};

/**
 * Create a Form for Manipulating Element data.
 */
Ext.formbuilder.createElementForm = function () {
  return Ext.create('Ext.form.Panel', {
    id: 'xml-form-builder-element-form',
    title: Drupal.t('Element Form'),
    region: 'center',
    frame: true,
    margin: '1 1 1 0',
    items: [{
      xtype: 'textfield',
      id: 'key',
      name: 'key',
      fieldLabel: Drupal.t('Identifier'),
      width: 640,
      allowBlank: false,
      regex: /^[a-zA-Z0-9_\-\x7f-\xff][a-zA-Z0-9_\-\x7f-\xff]*$/,
      regexText: "Invalid Identifier, it may only contain letters, numbers, hyphens or underscores",
      listeners: {
        render: function() {
          Ext.create('Ext.tip.ToolTip', {
            target: 'key',
            anchor: 'left',
            html: Drupal.t('Identifies this form element. It is used as the Drupal form array key for this element.')
          });
        }
      }
    }, {
      xtype: 'tabpanel',
      height: 640,
      id: 'xml-form-builder-element-form-tab-panel',
      plain: true,
      unstyled: true,
      defaults: {
        frame: true
      },
      items:[{
        title: Drupal.t('Common Form Controls'),
        collapsible: true,
        autoScroll: true,
        items: [{
          xtype: 'combobox',
          id: 'type',
          name: 'type',
          store: this.elementTypeStore,
          displayField: 'display',
          valueField: 'value',
          fieldLabel: Drupal.t('Type'),
          queryMode: 'local',
          editable: false,
          allowBlank: false,
          listeners: {
            select: function(combo, records, eOpts) {
              Ext.formbuilder.enableDisableXMLFields();
            },
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'type',
                anchor: 'left',
                html: Drupal.t('<h3><a name="type" id="type"></a>#type</h3>'+
                               '<p><strong>Used by</strong>: All</p>' +
                               '<p><strong>Description</strong>: Used to determine the type of form element.</p>')
              });
            }
          }
        }, {
          xtype: 'textfield',
          id: 'title',
          name: 'title',
          fieldLabel: Drupal.t('Title'),
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'title',
                anchor: 'left',
                html: Drupal.t('<h3><a name="title" id="title"></a>#title</h3>' +
                               '<p><strong>Used by</strong>: <a href="#checkbox">checkbox</a>, <a href="#checkboxes">checkboxes</a>, <a href="#date">date</a>, <a href="#fieldset">fieldset</a>, <a href="#file">file</a>, <a href="#item">item</a>, <a href="#password">password</a>, <a href="#password_confirm">password_confirm</a>, <a href="#radio">radio</a>, <a href="#radios">radios</a>, <a href="#select">select</a>, <a href="#textarea">textarea</a>, <a href="#textfield">textfield</a>, <a href="#weight">weight</a></p>' +
                               '<p><strong>Description</strong>: The title of the form element.</p>' +
                               '<p><strong>Values</strong>: Mixed</p>')
              });
            }
          }
        }, {
          xtype: 'textarea',
          id: 'description',
          name: 'description',
          fieldLabel: Drupal.t('Description'),
          width: 500,
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'description',
                anchor: 'left',
                html: Drupal.t('<h3><a name="description" id="description"></a>#description</h3>' +
                               '<p><strong>Used by</strong>: <a href="#checkbox">checkbox</a>, <a href="#checkboxes">checkboxes</a>, <a href="#date">date</a>, <a href="#fieldset">fieldset</a>, <a href="#file">file</a>, <a href="#item">item</a>, <a href="#password">password</a>, <a href="#password_confirm">password_confirm</a>, <a href="#radio">radio</a>, <a href="#radios">radios</a>, <a href="#select">select</a>, <a href="#textarea">textarea</a>, <a href="#textfield">textfield</a>, <a href="#weight">weight</a></p>' +
                               '<p><strong>Description</strong>: The description of the form element. Make sure to enclose inside the <a href="http://api.drupal.org/api/function/t">t</a>() function so that this property can be translated.</p>' +
                               '<p><strong>Values</strong>: Mixed</p>')
              });
            }
          }
        },  {
          xtype: 'textfield',
          id: 'default_value',
          name: 'default_value',
          fieldLabel: Drupal.t('Default Value'),
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'default_value',
                anchor: 'left',
                html: Drupal.t('<h3><a name="default_value" id="default_value"></a>#default_value</h3>' +
                               '<p><strong>Used by</strong>: <a href="#checkbox">checkbox</a>, <a href="#checkboxes">checkboxes</a>, <a href="#date">date</a>, <a href="#hidden">hidden</a>, <a href="#radio">radio</a>, <a href="#radios">radios</a>, <a href="#select">select</a>, <a href="#textarea">textarea</a>, <a href="#textfield">textfield</a>, <a href="#token">token</a>, <a href="#weight">weight</a></p>' +
                               '<p><strong>Description</strong>: The value of the form element that will be displayed or selected initially if the form has not been submitted yet. <strong>Should NOT be confused with</strong> <strong><a href="#value">#value</a></strong>, which is a hard-coded value that the user cannot change!</p>' +
                               '<p><strong>Values</strong>: Mixed</p>')
              });
            }
          }
        }, {
          xtype: 'checkbox',
          id: 'required',
          name: 'required',
          fieldLabel: Drupal.t('Required'),
          inputValue: true,
          uncheckedValue: false,
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'required',
                anchor: 'left',
                html: Drupal.t('<h3><a name="required" id="required"></a>#required</h3>' +
                               '<p><strong>Used by</strong>: <a href="#checkbox">checkbox</a>, <a href="#checkboxes">checkboxes</a>, <a href="#date">date</a>, <a href="#file">file</a>, <a href="#password">password</a>, <a href="#password_confirm">password_confirm</a>, <a href="#radio">radio</a>, <a href="#radios">radios</a>, <a href="#select">select</a>, <a href="#textarea">textarea</a>, <a href="#textfield">textfield</a>, <a href="#weight">weight</a></p>' +
                               '<p><strong>Description</strong>: Indicates whether or not the element is required. This automatically validates for empty fields and flags inputs as required. File fields are <strong>NOT</strong> allowed to be required.</p>' +
                               '<p><strong>Values</strong>: TRUE or FALSE</p>')
              });
            }
          }
        }, {
          xtype:'fieldset',
          checkboxToggle: true,
          collapsed: true,
          title: Drupal.t('Create'),
          id: 'actions_create',
          checkboxName: 'actions_create',
          items: [{
            xtype: 'combobox',
            fieldLabel: Drupal.t('Path Context'),
            displayField: 'display',
            valueField: 'value',
            editable: false,
            allowBlank: false,
            queryMode: 'local',
            value: 'document',
            id: 'actions-create-context',
            name: 'actions_create_context',
            store: new Ext.data.Store({
              storeId: 'ElementTypes',
              fields: ['display', 'value'],
              proxy: {
                type: 'memory',
                reader: {
                  type: 'json'
                }
              },
              data: [{
                display:'document',
                value:'document'
              },{
                display:'parent',
                value:'parent'
              }]
            }),
            listeners: {
              render: function() {
                Ext.create('Ext.tip.ToolTip', {
                  target: 'actions-create-context',
                  anchor: 'left',
                  html: Drupal.t('<h3>Create - Context</h3>' +
                                 '<p class="help">The context in which the path will be executed.</p>')
                });
              }
            }
          }, {
            xtype: 'textfield',
            fieldLabel: Drupal.t('Path'),
            name: 'actions_create_path',
            id: 'actions-create-path',
            width: 600,
            listeners: {
              render: function() {
                Ext.create('Ext.tip.ToolTip', {
                  target: 'actions-create-path',
                  anchor: 'left',
                  html: Drupal.t('<h3>Create - Path</h3>' +
                                 '<p class="help">An XPath expression to this element\'s parent. Used to detemine where this element will be inserted.</p>')
                });
              }
            }
          }, {
            xtype: 'textfield',
            fieldLabel: Drupal.t('Schema'),
            name: 'actions_create_schema',
            id: 'actions-create-schema',
            width: 600,
            listeners: {
              render: function() {
                Ext.create('Ext.tip.ToolTip', {
                  target: 'actions-create-schema',
                  anchor: 'left',
                  html: Drupal.t('<h3>Create - Schema</h3>' +
                                 '<p class="help">An XPath exporession to the definition of this element\'s parent. The XPath expression is executed in the schema defined in this form\'s properties. This is used to determine the insert order for this element.</p>')
                });
              }
            }
          }, {
            xtype: 'combobox',
            fieldLabel: Drupal.t('Type'),
            displayField: 'display',
            valueField: 'value',
            editable: false,
            allowBlank: false,
            queryMode: 'local',
            value: 'document',
            name: 'actions_create_type',
            id: 'actions-create-type',
            value: 'element',
            store: new Ext.data.Store({
              storeId: 'ElementTypes',
              fields: ['display', 'value'],
              proxy: {
                type: 'memory',
                reader: {
                  type: 'json'
                }
              },
              data: [{
                display:'element',
                value:'element'
              }, {
                display:'attribute',
                value:'attribute'
              }, {
                display:'xml',
                value:'xml'
              }]
            }),
            listeners: {
              render: function() {
                Ext.create('Ext.tip.ToolTip', {
                  target: 'actions-create-type',
                  anchor: 'left',
                  html: Drupal.t('<h3>Create - Type</h3>' +
                                 '<p class="help">The type of node that will be created. If XML is specified, an XML snippet is expected in the value field.</p>')
                });
              }
            }
          }, {
            xtype: 'textarea',
            fieldLabel: Drupal.t('Value'),
            name: 'actions_create_value',
            id: 'actions-create-value',
            width: 600,
            listeners: {
              render: function() {
                Ext.create('Ext.tip.ToolTip', {
                  target: 'actions-create-value',
                  anchor: 'left',
                  html: Drupal.t('<h3>Create - Value</h3>' +
                                 '<p class="help">If the type is either Element or Attribute, the name of the element or attribute is expected. If the type is XML, an XML snippet is expected. The value of the form field will be inserted wherever the string %value% is used in the XML snippet.</p>')
                });
              }
            }
          }]
        }, {
          xtype:'fieldset',
          checkboxToggle: true,
          collapsed: true,
          title: Drupal.t('Read'),
          id: 'actions_read',
          checkboxName: 'actions_read',
          items: [{
            xtype: 'combobox',
            fieldLabel: Drupal.t('Path Context'),
            displayField: 'display',
            valueField: 'value',
            editable: false,
            allowBlank: false,
            queryMode: 'local',
            value: 'document',
            name: 'actions_read_context',
            id: 'actions-read-context',
            store: new Ext.data.Store({
              storeId: 'ElementTypes',
              fields: ['display', 'value'],
              proxy: {
                type: 'memory',
                reader: {
                  type: 'json'
                }
              },
              data: [{
                display:'document',
                value:'document'
              },{
                display:'parent',
                value:'parent'
              }]
            }),
            listeners: {
              render: function() {
                Ext.create('Ext.tip.ToolTip', {
                  target: 'actions-read-context',
                  anchor: 'left',
                  html: Drupal.t('<h3>Read - Context</h3>' +
                                 '<p class="help">The context in which the path will be executed.</p>')
                });
              }
            }
          }, {
            xtype: 'textfield',
            fieldLabel: Drupal.t('Path'),
            name: 'actions_read_path',
            id: 'actions-read-path',
            width: 600,
            listeners: { //The context in which the path will be executed in.
              render: function() {
                Ext.create('Ext.tip.ToolTip', {
                  target: 'actions-read-path',
                  anchor: 'left',
                  html: Drupal.t('<h3>Read - Path</h3>' +
                                 '<p class="help">An XPath expression to the node this form field repersents. The node\'s value will be used to automatically populate this form field. The node selected by this XPath expression can be used as the self context for the \'update\' and \'delete\' actions.</p>')
                });
              }
            }
          }]
        }, {
          xtype:'fieldset',
          checkboxToggle: true,
          collapsed: true,
          title: Drupal.t('Update'),
          id: 'actions_update',
          checkboxName: 'actions_update',
          items: [{
            xtype: 'combobox',
            fieldLabel: Drupal.t('Path Context'),
            displayField: 'display',
            valueField: 'value',
            editable: false,
            allowBlank: false,
            queryMode: 'local',
            value: 'self',
            name: 'actions_update_context',
            id: 'actions-update-context',
            store: new Ext.data.Store({
              storeId: 'ElementTypes',
              fields: ['display', 'value'],
              proxy: {
                type: 'memory',
                reader: {
                  type: 'json'
                }
              },
              data: [{
                display:'document',
                value:'document'
              },{
                display:'parent',
                value:'parent'
              },{
                display:'self',
                value:'self'
              }]
            }),
            listeners: {
              render: function() {
                Ext.create('Ext.tip.ToolTip', {
                  target: 'actions-update-context',
                  anchor: 'left',
                  html: Drupal.t('<h3>Update - Context</h3>' +
                                 '<p class="help">The context in which the path will be executed.</p>')
                });
              }
            }
          }, {
            xtype: 'textfield',
            fieldLabel: Drupal.t('Path'),
            value: 'self::node()',
            name: 'actions_update_path',
            id: 'actions-update-path',
            width: 600,
            listeners: {
              render: function() {
                Ext.create('Ext.tip.ToolTip', {
                  target: 'actions-update-path',
                  anchor: 'left',
                  html: Drupal.t('<h3>Update - Path</h3>' +
                                 '<p class="help">An XPath expression used to select one or more existing nodes within the document to update. The selected nodes values will be replaced by the value in the this form field.</p>')
                });
              }
            }
          }, {
            xtype: 'textfield',
            fieldLabel: Drupal.t('Schema'),
            name: 'actions_update_schema',
            id: 'actions-update-schema',
            width: 600,
            listeners: {
              render: function() {
                Ext.create('Ext.tip.ToolTip', {
                  target: 'actions-update-schema',
                  anchor: 'left',
                  html: Drupal.t('<h3>Update - Schema</h3>' +
                                 '<p class="help">An XPath expression to the definition of this element. The XPath expression is executed in the schema defined in this form\'s properties. This is used to automatically validate submitted values for this form field.</p>')
                });
              }
            }
          }]
        }, {
          xtype:'fieldset',
          checkboxToggle: true,
          collapsed: true,
          title: Drupal.t('Delete'),
          id: 'actions_delete',
          checkboxName: 'actions_delete',
          items: [{
            xtype: 'combobox',
            fieldLabel: Drupal.t('Path Context'),
            displayField: 'display',
            valueField: 'value',
            editable: false,
            allowBlank: false,
            queryMode: 'local',
            value: 'self',
            name: 'actions_delete_context',
            id: 'actions-delete-context',
            store: new Ext.data.Store({
              storeId: 'ElementTypes',
              fields: ['display', 'value'],
              proxy: {
                type: 'memory',
                reader: {
                  type: 'json'
                }
              },
              data: [{
                display:'document',
                value:'document'
              },{
                display:'parent',
                value:'parent'
              },{
                display:'self',
                value:'self'
              }]
            }),
            listeners: {
              render: function() {
                Ext.create('Ext.tip.ToolTip', {
                  target: 'actions-delete-context',
                  anchor: 'left',
                  html: Drupal.t('<h3>Delete - Context</h3>' +
                                 '<p class="help">The context in which the path will be executed.</p>')
                });
              }
            }
          }, {
            xtype: 'textfield',
            fieldLabel: Drupal.t('Path'),
            value: 'self::node()',
            name: 'actions_delete_path',
            id: 'actions-delete-path',
            width: 600,
            listeners: {
              render: function() {
                Ext.create('Ext.tip.ToolTip', {
                  target: 'actions-delete-path',
                  anchor: 'left',
                  html: Drupal.t('<h3>Delete - Path</h3>' +
                                 '<p class="help">An XPath expression used to select one or more existing nodes within the document to delete. The selected nodes will be removed from the document.</p>')
                });
              }
            }
          }]
        }]
      }, {
        title: Drupal.t('Advanced Form Controls'),
        autoScroll: true,
        items: [{
          xtype: 'checkbox',
          id: 'access',
          name: 'access',
          fieldLabel: Drupal.t('Access'),
          checked: true,
          inputValue: true,
          uncheckedValue: false,
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'access',
                anchor: 'left',
                html:  Drupal.t('<h3><a name="access" id="access"></a>#access</h3>' +
                                '<p><strong>Used by</strong>: All elements and forms</p>' +
                                '<p><strong>Description</strong>: Defines whether the element is accessible or not. When FALSE, the element is not rendered and the user-submitted value is not taken into consideration.</p>' +
                                '<p><strong>Values</strong>: TRUE or FALSE.</p>')
              });
            }
          }
        }, {

          xtype: 'textfield',
          id: 'autocomplete_path',
          name: 'autocomplete_path',
          fieldLabel: Drupal.t('Autocomplete Path'),
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'autocomplete_path',
                anchor: 'left',
                html: Drupal.t('<h3><a name="autocomplete_path" id="autocomplete_path"></a>#autocomplete_path</h3>' +
                               '<p><strong>Used by</strong>: <a href="#textfield">textfield</a></p>' +
                               '<p><strong>Description</strong>: The path that the AJAX autocomplete script uses as the source for autocompletion.</p>')
              });
            }
          }
        }, {
          xtype: 'textfield',
          id: 'button_type',
          name: 'button_type',
          fieldLabel: Drupal.t('Button Type'),
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'button_type',
                anchor: 'left',
                html: Drupal.t('<h3><a name="button_type" id="button_type"></a>#button_type</h3>' +
                               '<p><strong>Used by</strong>: <a href="#button">button</a>, <a href="#image_button">image_button</a>, <a href="#submit">submit</a></p>' +
                               '<p><strong>Description</strong>: Adds a CSS class to the button, in the form <em>form-[button_type_value]</em>. Note that this does NOT set the HTML attribute <em>type</em> of the button.</p>' +
                               '<p class="help"><strong>Values</strong>: String </p>')
              });
            }
          }
        }, {
          xtype: 'numberfield',
          id: 'cols',
          name: 'cols',
          fieldLabel: Drupal.t('Cols'),
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'cols',
                anchor: 'left',
                html: Drupal.t('<h3><a name="cols" id="cols"></a>#cols</h3>' +
                               '<p><strong>Used by</strong>: <a href="#textarea">textarea</a></p>' +
                               '<p><strong>Description</strong>: Defines how many columns the textarea should contain (see also <a href="#rows">#rows</a>).</p>' +
                               '<p><strong>Values</strong>: A positive number</p>')
              });
            }
          }
        }, {
          xtype: 'checkbox',
          id: 'disabled',
          name: 'disabled',
          fieldLabel: Drupal.t('Disabled'),
          inputValue: true,
          uncheckedValue: false,
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'disabled',
                anchor: 'left',
                html: Drupal.t('<h3><a name="disabled" id="disabled"></a>#disabled</h3>' +
                               '<p><strong>Used by</strong>: <a href="#button">button</a>, <a href="#checkbox">checkbox</a>, <a href="#checkboxes">checkboxes</a>, <a href="#date">date</a>, <a href="#file">file</a>, <a href="#image_button">image_button</a>, <a href="#password">password</a>, <a href="#password_confirm">password_confirm</a>, <a href="#radio">radio</a>, <a href="#radios">radios</a>, <a href="#select">select</a>, <a href="#submit">submit</a>, <a href="#textarea">textarea</a>, <a href="#textfield">textfield</a>, <a href="#weight">weight</a></p>' +
                               '<p><strong>Description</strong>: Disables (greys out) a form input element. Note that disabling a form field doesn\'t necessarily prevent someone from submitting a value through DOM manipulation; it just tells the browser not to accept input.</p>' +
                               '<p><strong>Values</strong>: TRUE or FALSE</p>')
              });
            }
          }
        }, {
          xtype: 'numberfield',
          id: 'delta',
          name: 'delta',
          fieldLabel: Drupal.t('Delta'),
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'delta',
                anchor: 'left',
                html: Drupal.t('<h3><a name="delta" id="delta"></a>#delta</h3>' +
                               '<p><strong>Used by</strong>: <a href="#weight">weight</a></p>' +
                               '<p><strong>Description</strong>: The number of weights to have selectable. For example, with $delta =&gt; 10, the weight selection box would display numbers from -10 to 10.</p>' +
                               '<p><strong>Values</strong>: A positive number</p>')
              });
            }
          }
        },   {
          xtype: 'textfield',
          id: 'prefix',
          name: 'prefix',
          fieldLabel: Drupal.t('Prefix'),
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'prefix',
                anchor: 'left',
                html: Drupal.t('<h3><a name="prefix" id="prefix"></a>#prefix</h3>' +
                               '<p><strong>Used by</strong>: All elements and forms.</p>' +
                               '<p><strong>Description</strong>: Text or markup to include before the form element. Also see <a href="#suffix">#suffix</a>.</p>' +
                               '<p><strong>Values</strong>: Mixed</p>')
              });
            }
          }
        }, {
          xtype: 'textfield',
          id: 'suffix',
          name: 'suffix',
          fieldLabel: Drupal.t('Suffix'),
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'suffix',
                anchor: 'left',
                html: Drupal.t('<h3><a name="suffix" id="suffix"></a>#suffix</h3>' +
                               '<p><strong>Used by</strong>: All elements and forms</p>' +
                               '<p><strong>Description</strong>: Text or markup to include after the form element. Also see <a href="#prefix">#prefix</a>.</p>' +
                               '<p><strong>Values</strong>: Mixed</p>')
              });
            }
          }
        }, {
          xtype: 'textfield',
          id: 'theme',
          name: 'theme',
          fieldLabel: Drupal.t('Theme'),
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'theme',
                anchor: 'left',
                html: Drupal.t('<h3><a name="theme" id="theme"></a>#theme</h3>' +
                               '<p><strong>Used by</strong>: All elements and forms.</p>' +
                               '<p><strong>Description</strong>: Theme function to call for element.</p>' +
                               '<p><strong>Values</strong>: The name of a theme function, without the initial <em>theme_</em> prefix.</p>')
              });
            }
          }
        },  {
          xtype: 'numberfield',
          id: 'weight',
          name: 'weight',
          fieldLabel: Drupal.t('Weight'),
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'weight',
                anchor: 'left',
                html: Drupal.t('<h3><a name="weightval" id="weightval"></a>#weight</h3>' +
                               '<p><strong>Used by</strong>: All elements</p>' +
                               '<p><strong>Description</strong>: Used to sort the list of form elements before output; lower numbers appear before higher numbers.</p>' +
                               '<p><strong>Values</strong>: A positive or negative number (integer or decimal)</p>')
              });
            }
          }
        }, {
          xtype: 'checkbox',
          id: 'tree',
          name: 'tree',
          fieldLabel: Drupal.t('Tree'),
          checked: true,
          inputValue: true,
          uncheckedValue: false,
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'tree',
                anchor: 'left',
                html: Drupal.t('<h3><a name="tree" id="tree"></a>#tree</h3>' +
                               '<p><strong>Used by</strong>: All</p>' +
                               '<p><strong>Description</strong>: Used to allow collections of form elements. Normally applied to the "parent" element, as the #tree property cascades to sub-elements. Use where you previously used ][ in form_ calls. For more information, see <a href="http://drupal.org/node/48643">#tree and #parents</a> in the handbook.</p>' +
                               '<p><strong>Values</strong>: TRUE or FALSE</p>')
              });
            }
          }
        }, {
          xtype: 'textfield',
          id: 'field_prefix',
          name: 'field_prefix',
          fieldLabel: Drupal.t('Field Prefix'),
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'field_prefix',
                anchor: 'left',
                html: Drupal.t('<h3><a name="field_prefix" id="field_prefix"></a>#field_prefix</h3>' +
                               '<p><strong>Used by</strong>: <a href="#textfield">textfield</a></p>' +
                               '<p><strong>Description</strong>: Text or code that is placed directly in front of the textfield. This can be used to prefix a textfield with a constant string.</p>' +
                               '<p><strong>Values</strong>: Mixed</p>')
              });
            }
          }
        }, {
          xtype: 'textfield',
          id: 'field_suffix',
          name: 'field_suffix',
          fieldLabel: Drupal.t('Field Suffix'),
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'field_suffix',
                anchor: 'left',
                html: Drupal.t('<h3><a name="field_suffix" id="field_suffix"></a>#field_suffix</h3>' +
                               '<p><strong>Used by</strong>: <a href="#textfield">textfield</a></p>' +
                               '<p><strong>Description</strong>: Text or code that is placed directly after a textfield. This can be used to add a unit to a textfield.</p>' +
                               '<p><strong>Values</strong>: Mixed</p>')
              });
            }
          }
        }, {
          xtype: 'numberfield',
          id: 'maxlength',
          name: 'maxlength',
          fieldLabel: Drupal.t('Max Length'),
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'maxlength',
                anchor: 'left',
                html: Drupal.t('<h3><a name="maxlength" id="maxlength"></a>#maxlength</h3>' +
                               '<p><strong>Used by</strong>: <a href="#password">password</a>, <a href="#textfield">textfield</a></p>' +
                               '<p><strong>Description</strong>: The maximum amount of characters to accept as input.</p>' +
                               '<p><strong>Values</strong>: A positive number.</p>')
              });
            }
          }
        }, {
          xtype: 'combobox',
          id: 'method',
          name: 'method',
          fieldLabel: Drupal.t('Method'),
          displayField: 'display',
          valueField: 'value',
          editable: false,
          queryMode: 'local',
          store: new Ext.data.Store({
            fields: ['display', 'value'],
            data: [{
              display: Drupal.t('Post'),
              value: 'post'
            },{
              display: Drupal.t('Get'),
              value: 'get'
            }]
          }),
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'method',
                anchor: 'left',
                html: Drupal.t('<h3><a name="method" id="method"></a>#method</h3>' +
                               '<p><strong>Used by</strong>: <a href="#form">form</a></p>' +
                               '<p><strong>Description</strong>: The HTTP method that will be used to submit the form.</p>' +
                               '<p><strong>Values</strong>: GET or POST. Default is POST.</p>')
              });
            }
          }
        }, {
          xtype: 'textfield',
          id: 'name',
          name: 'name',
          fieldLabel: Drupal.t('Name'),
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'name',
                anchor: 'left',
                html: Drupal.t('<h3 class="help"><a name="name" id="name"></a>#name</h3>' +
                               '<p><strong>Used by</strong>: <a href="#button">button</a>, <a href="#submit">submit</a></p>' +
                               '<p><strong>Description</strong>: INTERNAL, except for buttons. All button and submit elements on a form should have the same name, which is set to \'op\' by default in Drupal. This does not apply to image buttons. For non-button elements, Drupal sets the name by using \'foo\' in $form[\'foo\'] as well as any parents of the element.</p>' +
                               '<p><strong>Values</strong>: String.</p>')
              });
            }
          }
        }, {
          xtype: 'textfield',
          id: 'value',
          name: 'value',
          fieldLabel: Drupal.t('Value'),
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'value',
                anchor: 'left',
                html: Drupal.t('<h3><a name="value" id="value"></a>#value</h3>' +
                               '<p><strong>Used by</strong>: <a href="#button">button</a>, <a href="#hidden">hidden</a>, <a href="#image_button">image_button</a>, <a href="#item">item</a>, <a href="#markup">markup</a>, <a href="#submit">submit</a>, <a href="#token">token</a>, <a href="#val">value</a></p>' +
                               '<p><strong>Description</strong>: Used to set values that cannot be edited by the user. <strong>Should NOT be confused with <a href="#default_value">#default_value</a></strong>, which is for form inputs where users can override the default value.</p>' +
                               '<p><strong>Values</strong>: Mixed (text or numbers)</p>')
              });
            }
          }
        }, {
          xtype: 'textfield',
          id: 'return_value',
          name: 'return_value',
          fieldLabel: Drupal.t('Return Value'),
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'return_value',
                anchor: 'left',
                html: Drupal.t('<h3><a name="return_value" id="return_value"></a>#return_value</h3>' +
                               '<p><strong>Used by</strong>: <a href="#checkbox">checkbox</a>, <a href="#image_button">image_button</a>, <a href="#radio">radio</a></p>' +
                               '<p><strong>Description</strong>: The value that the element should return when selected.</p>' +
                               '<p><strong>Values</strong>: Mixed</p>')
              });
            }
          }
        }, {
          xtype: 'numberfield',
          id: 'rows',
          name: 'rows',
          fieldLabel: Drupal.t('Rows'),
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'rows',
                anchor: 'left',
                html: Drupal.t('<h3><a name="rows" id="rows"></a>#rows</h3>' +
                   '<p><strong>Used by</strong>: <a href="#textarea">textarea</a></p>' +
                   '<p><strong>Description</strong>: Defines how many rows should be in the textarea (see also <a href="#cols">#cols</a>).</p>' +
                   '<p><strong>Values</strong>: A positive number</p>')
              });
            }
          }
        }, {
          xtype: 'numberfield',
          id: 'size',
          name: 'size',
          fieldLabel: Drupal.t('Size'),
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'size',
                anchor: 'left',
                html: Drupal.t('<h3><a name="size" id="size"></a>#size</h3>' +
                   '<p><strong>Used by</strong>:  <a href="#password">password</a>, <a href="#password_confirm">password_confirm</a>, <a href="#select">select</a>, <a href="#textfield">textfield</a></p>' +
                   '<p><strong>Description</strong>: Width of the textfield (in characters), or size of the multiselect box (in lines).</p>' +
                   '<p><strong>Values</strong>: A positive number.</p>')
              });
            }
          }
        }, {
          xtype: 'textfield',
          id: 'src',
          name: 'src',
          fieldLabel: Drupal.t('Src'),
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'src',
                anchor: 'left',
                html: Drupal.t('<h3><a name="src" id="src"></a>#src</h3>' +
                   '<p><strong>Used by</strong>: <a href="#image_button">image_button</a></p>' +
                   '<p><strong>Description</strong>: The button image\'s URL.</p>' +
                   '<p><strong>Values</strong>: An URL.</p>')
              });
            }
          }
        },  {
          xtype: 'checkbox',
          id: 'collapsed',
          name: 'collapsed',
          fieldLabel: Drupal.t('Collapsed'),
          inputValue: true,
          uncheckedValue: false,
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'collapsed',
                anchor: 'left',
                html: Drupal.t('<h3><a name="collapsed" id="collapsed"></a>#collapsed</h3>' +
                   '<p><strong>Used by</strong>: <a href="#fieldset">fieldset</a></p>' +
                   '<p><strong>Description</strong>: Indicates whether or not the fieldset is collapsed by default. See <a href="#collapsible">#collapsible</a> property.</p>' +
                   '<p><strong>Values</strong>: TRUE or FALSE</p>')
              });
            }
          }
        }, {
          xtype: 'checkbox',
          id: 'collapsible',
          name: 'collapsible',
          fieldLabel: Drupal.t('Collapsible'),
          inputValue: true,
          uncheckedValue: false,
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'collapsible',
                anchor: 'left',
                html: Drupal.t('<h3><a name="collapsible" id="collapsible"></a>#collapsible</h3>' +
                   '<p><strong>Used by</strong>: <a href="#fieldset">fieldset</a></p>' +
                   '<p><strong>Description</strong>: Indicates whether or not the fieldset can be collapsed with JavaScript. See <a href="#collapsed">#collapsed</a> property.</p>' +
                   '<p><strong>Values</strong>: TRUE or FALSE</p>')
              });
            }
          }
        }, {
          xtype: 'checkbox',
          id: 'executes_submit_callback',
          name: 'executes_submit_callback',
          fieldLabel: Drupal.t('Executes Submit Callback'),
          inputValue: true,
          uncheckedValue: false,
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'executes_submit_callback',
                anchor: 'left',
                html: Drupal.t('<h3><a name="executes_submit_callback" id="executes_submit_callback"></a>#executes_submit_callback</h3>' +
                   '<p><strong>Used by</strong>: <a href="#button">button</a>, <a href="#image_button">image_button</a>, <a href="#submit">submit</a></p>' +
                   '<p><strong>Description</strong>: Indicates whether or not the button should submit the form.</p>' +
                   '<p><strong>Values</strong>: TRUE or FALSE</p>')
              });
            }
          }
        }, {
          xtype: 'checkbox',
          id: 'multiple',
          name: 'multiple',
          fieldLabel: Drupal.t('Multiple'),
          inputValue: true,
          uncheckedValue: false,
          listeners: {
            change: function(combo, newValue, oldValue, eOpts) {
              Ext.formbuilder.enableDisableXMLFields();
            },
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'multiple',
                anchor: 'left',
                html: Drupal.t('<h3><a name="multiple" id="multiple"></a>#multiple</h3>' +
                   '<p><strong>Used by</strong>: <a href="#select">select</a></p>' +
                   '<p><strong>Description</strong>: Indicates whether the user may select more than one item.</p>' +
                   '<p><strong>Values</strong>: TRUE or FALSE</p>')
              });
            }
          }
        }, {
          xtype: 'checkbox',
          id: 'resizable',
          name: 'resizable',
          fieldLabel: Drupal.t('Resizable'),
          inputValue: true,
          uncheckedValue: false,
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'resizable',
                anchor: 'left',
                html: Drupal.t('<h3><a name="resizable" id="resizable"></a>#resizable</h3>' +
                   '<p><strong>Used by</strong>: <a href="#textarea">textarea</a></p>' +
                   '<p><strong>Description</strong>: Indicates whether users should be allowed to resize the text area</p>' +
                   '<p><strong>Values</strong>: TRUE or FALSE</p>')
              });
            }
          }
        }, {
          xtype:'fieldset',
          checkboxToggle: true,
          checkboxName: 'ajax',
          collapsed: true,
          title: Drupal.t('AJAX'),
          id: 'ajax',
          layout: 'anchor',
          defaults: {
            anchor: '100%'
          },
          items: [{
            xtype: 'textfield',
            id: 'ahah-effect',
            name: 'ahah_effect',
            fieldLabel: Drupal.t('Effect'),
            listeners: {
              render: function() {
                Ext.create('Ext.tip.ToolTip', {
                  target: 'ahah-effect',
                  anchor: 'left',
                  html:  Drupal.t('<h3><a name="ahah_effect" id="ahah_effect"></a>#ajax[\'effect\']</h3>' +
                  '<p><strong>Description</strong>: Specifies the effect used when adding content from an AHAH request. </p>' +
                  '<p><strong>Values</strong>: String. Possible values: \'none\' (default), \'fade\', \'slide\'. If the <a href="http://interface.eyecon.ro/">interface elements library</a> is installed, any effect with the name <em>effect</em>Toggle may also be used. </p>')
                });
              }
            }
          },{
            xtype: 'textfield',
            id: 'ahah-event',
            name: 'ahah_event',
            fieldLabel: Drupal.t('Event'),
            listeners: {
              render: function() {
                Ext.create('Ext.tip.ToolTip', {
                  target: 'ahah-event',
                  anchor: 'left',
                  html: Drupal.t('<h3><a name="ahah_event" id="ahah_event"></a>#ajax[\'event\']</h3>' +
                 '<p><strong>Description</strong>: When this event occurs in this element, Drupal will perform an HTTP request in the background via JavaScript.</p>' +
                 '<p><strong>Values</strong>: String. Possible values: Any valid <a href="http://docs.jquery.com/Events">jQuery event</a>, including \'mousedown\', \'blur\', and \'change\'.'+
                 'Note that #ajax[\'event\'] does not need to be explicitly specified. Although it can be manually set, usually the <a href="#element_default_values">default value </a> will be sufficient.</p>')
                });
              }
            }
          }, {
            xtype: 'textfield',
            id: 'ahah-method',
            name: 'ahah_method',
            fieldLabel: Drupal.t('Method'),
            listeners: {
              render: function() {
                Ext.create('Ext.tip.ToolTip', {
                  target: 'ahah-method',
                  anchor: 'left',
                  html: Drupal.t('<h3><a name="ahah_method" id="ahah_method"></a>#ajax[\'method\']</h3>' +
                 '<p><strong>Description</strong>: Modifies the behavior of the returned HTML from an AHAH request when inserting into the <a href="#ajax_wrapper">#ajax_wrapper</a>. If not set, the returned HTML will replace the contents of the wrapper element, but it\'s also possible to use any of the available <a href="http://docs.jquery.com/DOM/Manipulation">jQuery operations for DOM manipulation</a>. </p>' +
                 '<p><strong>Values</strong>: String. Possible values: \'replace\' (default), \'after\', \'append\', \'before\', \'prepend\'.</p>')
                });
              }
            }
          },{
            xtype: 'textfield',
            id: 'ahah-path',
            name: 'ahah_path',
            fieldLabel: Drupal.t('Path'),
            listeners: {
              render: function() {
                Ext.create('Ext.tip.ToolTip', {
                  target: 'ahah-path',
                  anchor: 'left',
                  html: Drupal.t('<h3><a name="ahah_path" id="ahah_path"></a>#ajax[\'path\']</h3>' +
                 '<p><strong>Description</strong>: If set, this property triggers AHAH behaviors on a form element. This is the Drupal menu path to a callback function which will generate HTML and return the HTML string to Drupal. The result will be placed in the <em>div</em> element specified in <a href="#ajax_wrapper">#ajax[\'wrapper\']</a>. </p>' +
                 '<p><strong>Values</strong>: String containing a Drupal menu path.</p>')
                });
              }
            }
          }, {
            xtype: 'textfield',
            id: 'ahah-wrapper',
            name: 'ahah_wrapper',
        fieldLabel: Drupal.t('Wrapper'),
            listeners: {
              render: function() {
                Ext.create('Ext.tip.ToolTip', {
                  target: 'ahah-wrapper',
                  anchor: 'left',
                  html: Drupal.t('<h3><a name="ahah_wrapper" id="ahah_wrapper"></a>#ajax[\'wrapper\']</h3>' +
                 '<p><strong>Description</strong>: Defines the HTML <em>id</em> attribute of an element on the page that will serve as the destination for HTML returned by an AHAH request. A <em>div</em> element is generally used as the wrapper since it provides the most flexibility for placement of elements before, after or inside of its HTML tags. This property is required for using AHAH requests in a form element.</p>' +
                 '<p><strong>Values</strong>: String containg a valid <em>id</em> attribute of an HTML element on the same page.</p>')
                });
              }
            }
          }, {
            xtype: 'checkbox',
            id: 'ahah-keypress',
            name: 'ahah_keypress',
            fieldLabel: Drupal.t('Keypress'),
            inputValue: true,
            listeners: {
              render: function() {
                Ext.create('Ext.tip.ToolTip', {
                  target: 'ahah-keypress',
                  anchor: 'left',
                  html: Drupal.t('<h3><a name="ahah_keypress" id="ahah_keypress"></a>#ajax[\'keypress\']</h3>' +
                 '<p><strong>Description</strong>: If set to TRUE, then the element\'s #ajax[\'event\'] will be triggered if the ENTER key is pressed while the element has focus.</p>')
                });
              }
            }
          }, {
            xtype:'fieldset',
            checkboxToggle: true,
            collapsed: true,
            checkboxName: 'ahah_progress',
            id: 'ahah_progress',
            title: Drupal.t('Progress'),
            items: [{
              xtype: 'textfield',
              id: 'ahah-progress-type',
              name: 'ahah_progress_type',
              fieldLabel: Drupal.t('Type'),
              listeners: {
                render: function() {
                  Ext.create('Ext.tip.ToolTip', {
                    target: 'ahah-progress-type',
                    anchor: 'top',
                    html: Drupal.t('<p>Possible values:</p>' +
                   '<ul><li><strong>#ajax[\'progress\'][\'type\']</strong> String. Possible values: \'throbber\' (default), \'bar\'.</li></ul>')
                  });
                }
              }
            }, {
              xtype: 'textfield',
              id: 'ahah-progress-message',
              name: 'ahah_progress_message',
              fieldLabel: Drupal.t('Message'),
              listeners: {
                render: function() {
                  Ext.create('Ext.tip.ToolTip', {
                    target: 'ahah-progress-message',
                    anchor: 'top',
                    html: Drupal.t('<p>Possible values:</p>' +
                   '<ul><li><strong>#ajax[\'progress\'][\'message\']</strong> String. An optional message to the user; should be wrapped with <a href="/api/drupal/includes--common.inc/function/t/6" title="Translate strings to the page language or a given language." class="local">t</a>().</li></ul>')
                  });
                }
              }
            }, {
              xtype: 'textfield',
              id: 'ahah-progress-url',
              name: 'ahah_progress_url',
              fieldLabel: Drupal.t('Url'),
              listeners: {
                render: function() {
                  Ext.create('Ext.tip.ToolTip', {
                    target: 'ahah-progress-url',
                    anchor: 'top',
                    html: Drupal.t('<p>Possible values:</p>' +
                   '<ul><li><strong>#ajax[\'progress\'][\'url\']</strong> String. The optional callback path to use to determine how full the progress bar is (as defined in progress.js). Only useable when \'type\' is \'bar\'.</li></ul>')
                  });
                }
              }
            }, {
              xtype: 'textfield',
              id: 'ahah-progress-interval',
              name: 'ahah_progress_interval',
              fieldLabel: Drupal.t('Interval'),
              listeners: {
                render: function() {
                  Ext.create('Ext.tip.ToolTip', {
                    target: 'ahah-progress-interval',
                    anchor: 'top',
                    html: Drupal.t('<p>Possible values:</p>' +
                   '<li><strong>#ajax[\'progress\'][\'interval\']</strong> String. The interval to be used in updating the progress bar (as defined in progress.js). Ony used if \'url\' is defined and \'type\' is \'bar\'.</li>')
                  });
                }
              }
            }],
            listeners: {
              render: function() {
                Ext.create('Ext.tip.ToolTip', {
                  target: 'ahah-progress',
                  anchor: 'bottom',
                  html: Drupal.t('<h3><a name="ahah_progress" id="ahah_progress"></a>#ajax[\'progress\']</h3>' +
                 '<p><strong>Description</strong>: Choose either a throbber or progress bar that is displayed while awaiting a response from the callback, and add an optional message.</p>' +
                 '<p><strong>Values</strong>: Array.</p>' +
                 '<p>Possible keys: \'type\', \'message\', \'url\', \'interval\'</p>')
                });
              }
            }
          }],
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'ahah',
                anchor: 'left',
                html: Drupal.t('<h3><a name="ajax" id="ajax"></a>#ajax</h3>' +
                   '<p><strong>Used by</strong>:' +
                   '<a href="#button">button</a>,' +
                   '<a href="#checkbox">checkbox</a>,' +
                   '<a href="#hidden">hidden</a>,' +
                   '<a href="#image_button">image button</a>,' +
                   '<a href="#password">password</a>,' +
                   '<a href="#radio">radio</a>,' +
                   '<a href="#select">select</a>,' +
                   '<a href="#submit">submit</a>,' +
                   '<a href="#textarea">textarea</a>,' +
                   '<a href="#textfield">textfield</a>' +
                   '</p>' +
                   '<p>An array of elements whose values control the behavior of the element with respect to Drupal AHAH JavaScript methods.</p>')
              });
            }
          }
        }]
      }, {
        title: Drupal.t('More Advanced Controls'),
        autoScroll: true,
        items: [{
          xtype: 'formgrid',
          id: 'attributes',
          name: 'attributes',
          title: Drupal.t('Attributes'),
          store: Ext.create('Ext.data.Store', {
            fields:['key', 'value'],
            proxy: {
              type: 'memory',
              reader: {
                type: 'json'
              }
            }
          }),
          modelInitTmpl: {
            key: '',
            value: ''
          },
          columns: [{
            xtype: 'gridcolumn',
            dataIndex: 'key',
            header: Drupal.t('Key'),
            sortable: true,
            width: 200,
            field: {
              type: 'textfield'
            }
          },{
            xtype: 'gridcolumn',
            dataIndex: 'value',
            header: Drupal.t('Value'),
            sortable: true,
            flex: 1,
            field: {
              type: 'textfield'
            }
          }],
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'attributes',
                anchor: 'left',
                html: Drupal.t('<h3><a name="attributes" id="attributes"></a>#attributes</h3>' +
                   '<p><strong>Used by</strong>: <a href="#button">button</a>, <a href="#checkbox">checkbox</a>, <a href="#checkboxes">checkboxes</a>, <a href="#date">date</a>, <a href="#fieldset">fieldset</a>, <a href="#file">file</a>, <a href="#form">form</a>, <a href="#image_button">image_button</a>, <a href="#password">password</a>, <a href="#radio">radio</a>, <a href="#radios">radios</a>, <a href="#select">select</a>, <a href="#submit">submit</a>, <a href="#textarea">textarea</a>, <a href="#textfield">textfield</a>, <a href="#weight">weight</a></p>' +
                   '<p><strong>Description</strong>: A mechanism allowing for setting of additional HTML attributes such as \'class\'.</p>' +
                   '<p><strong>Values</strong>: Any HTML attribute not covered by other properties, e.g. <strong>class</strong> (for control types), <strong>enctype</strong> (for forms).</p>')
              });
            }
          }
        }, {
          xtype: 'formgrid',
          id: 'element_validate',
          name: 'element_validate',
          title: Drupal.t('Element Validate'),
          store: Ext.create('Ext.data.Store', {
            fields:['value'],
            proxy: {
              type: 'memory',
              reader: {
                type: 'json'
              }
            }
          }),
          modelInitTmpl: {
            value: ''
          },
          columns: [{
            xtype: 'gridcolumn',
            dataIndex: 'value',
            header: Drupal.t('Function'),
            sortable: true,
            flex: 1,
            field: {
              type: 'textfield'
            }
          }],
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'element_validate',
                anchor: 'left',
                html: Drupal.t('<h3><a name="element_validate" id="element_validate"></a>#element_validate</h3>' +
                   '<p class="help"><strong>Used by</strong>: any element</p>' +
                   '<p><strong>Description</strong>: A list of custom validation functions that need to be passed. The functions must use <a href="/api/drupal/includes--form.inc/function/form_error/6" title="Flag an element as having an error." class="local">form_error</a>() or <a href="/api/drupal/includes--form.inc/function/form_set_error/6" title="File an error against a form element." class="local">form_set_error</a>() to set an error if the validation fails.</p>' +
                   '<p><strong>Values</strong>: an array of function names to be called to validate this element (and/or its children).</p>')
              });
            }
          }
        },  {
          xtype: 'formgrid',
          title: Drupal.t('Process'),
          id: 'process',
          name: 'process',
          store: Ext.create('Ext.data.Store', {
            fields:['value'],
            proxy: {
              type: 'memory',
              reader: {
                type: 'json'
              }
            }
          }),
          modelInitTmpl: {
            value: ''
          },
          columns: [{
            xtype: 'gridcolumn',
            dataIndex: 'value',
            header: Drupal.t('Functions'),
            sortable: true,
            flex: 1,
            field: {
              type: 'textfield'
            }
          }],
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'process',
                anchor: 'left',
                html: Drupal.t('<h3><a name="process"></a>#process</h3>' +
                   '<p><strong>Description</strong>: An array of functions that are called when an element is processed. Using this callback, modules can "register" further actions - for example, the "radios" form type is expanded to multiple radio buttons using a processing function.</p>' +
                   '<p><strong>Values</strong>: Array of function names (strings)</p>')
              });
            }
          }
        }, {
          xtype: 'formgrid',
          title: Drupal.t('Pre Render'),
          id: 'pre_render',
          name: 'pre_render',
          store: Ext.create('Ext.data.Store', {
            fields:['value'],
            proxy: {
              type: 'memory',
              reader: {
                type: 'json'
              }
            }
          }),
          modelInitTmpl: {
            value: ''
          },
          columns: [{
            xtype: 'gridcolumn',
            dataIndex: 'value',
            header: Drupal.t('Functions'),
            sortable: true,
            flex: 1,
            field: {
              type: 'textfield'
            }
          }],
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'pre_render',
                anchor: 'left',
                html: Drupal.t('<h3><a name="pre_render" id="pre_render"></a>#pre_render</h3>' +
                   '<p><strong>Used by</strong>: All elements and forms.</p>' +
                   '<p><strong>Description</strong>:' +
                   'Function(s) to call <strong>before</strong>' +
                   'rendering in </a><a href="http://api.drupal.org/api/function/drupal_render/" class="local">drupal_render</a>()' +
                   'has occured. The function(s) provided in #pre_render receive the element as an argument and ' +
                   'must return the altered element.</p>' +
                   '<p><strong>Values</strong>: An array of function names to call.</p>')
              });
            }
          }
        }, {
          xtype: 'formgrid',
          title: Drupal.t('Post Render'),
          id: 'post_render',
          name: 'post_render',
          store: Ext.create('Ext.data.Store', {
            fields:['value'],
            proxy: {
              type: 'memory',
              reader: {
                type: 'json'
              }
            }
          }),
          modelInitTmpl: {
            value: ''
          },
          columns: [{
            xtype: 'gridcolumn',
            dataIndex: 'value',
            header: Drupal.t('Functions'),
            sortable: true,
            flex: 1,
            field: {
              type: 'textfield'
            }
          }],
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'post_render',
                anchor: 'left',
                html: Drupal.t('<h3><a name="post_render" id="post_render"></a>#post_render</h3>' +
                   '<p><strong>Used by</strong>: All elements and forms</p>' +
                   '<p><strong>Description</strong>:' +
                   'Function(s) to call <strong>after</strong>' +
                   'rendering in </a><a href="http://api.drupal.org/api/function/drupal_render/" class="local">drupal_render</a>()' +
                   'has occured. The named function is called with two arguments - the rendered element and its children. It returns the (potentially)' +
                   'altered element content.</p>' +
                   '<p><strong>Values</strong>: An array of function names to call.</p>')
              });
            }
          }
        }, {
          xtype: 'formgrid',
          title: Drupal.t('After Build'),
          id: 'after_build',
          name: 'after_build',
          store: Ext.create('Ext.data.Store', {
            fields:['value'],
            proxy: {
              type: 'memory',
              reader: {
                type: 'json'
              }
            }
          }),
          modelInitTmpl: {
            value: ''
          },
          columns: [{
            xtype: 'gridcolumn',
            dataIndex: 'value',
            header: Drupal.t('Function'),
            sortable: true,
            flex: 1,
            field: {
              type: 'textfield'
            }
          }],
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'after_build',
                anchor: 'left',
                html: Drupal.t('<h3><a name="after_build" id="after_build"></a>#after_build</h3>'+
                   '<p><strong>Used by</strong>: All elements and forms</p>' +
                   '<p><strong>Description</strong>: An array of function names which will be called after the form or element is built.</p>')
              });
            }
          }
        }, {
          xtype: 'formgrid',
          id: 'options',
          name: 'options',
          title: Drupal.t('Options'),
          store: Ext.create('Ext.data.Store', {
            fields:['key', 'value'],
            proxy: {
              type: 'memory',
              reader: {
                type: 'json'
              }
            }
          }),
          modelInitTmpl: {
            key: '',
            value: ''
          },
          columns: [{
            xtype: 'gridcolumn',
            dataIndex: 'key',
            header: Drupal.t('Value'),
            sortable: true,
            width: 100,
            field: {
              type: 'textfield'
            }
          },{
            xtype: 'gridcolumn',
            dataIndex: 'value',
            header: Drupal.t('Label'),
            sortable: true,
            flex: 1,
            field: {
              type: 'textfield'
            }
          }],
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'options',
                anchor: 'left',
                html: Drupal.t('<h3><a name="options" id="options"></a>#options</h3>' +
                   '<p><strong>Used by</strong>: <a href="#checkboxes">checkboxes</a>, <a href="#radios">radios</a>, <a href="#select">select</a></p>' +
                   '<p><strong>Description</strong>: Selectable options for a form element that allow multiple choices.</p>')
              });
            }
          }
        },  {
          xtype: 'formgrid',
          title: Drupal.t('User Data'),
          id: 'user_data',
          name: 'user_data',
          store: Ext.create('Ext.data.Store', {
            fields:['key', 'value'],
            proxy: {
              type: 'memory',
              reader: {
                type: 'json'
              }
            }
          }),
          modelInitTmpl: {
            key: '',
            value: ''
          },
          columns: [{
            xtype: 'gridcolumn',
            dataIndex: 'key',
            header: Drupal.t('Key'),
            sortable: true,
            width: 150,
            field: {
              type: 'textfield'
            }
          },{
            xtype: 'gridcolumn',
            dataIndex: 'value',
            header: Drupal.t('Value'),
            sortable: true,
            flex: 1,
            field: {
              type: 'textfield'
            }
          }],
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'user_data',
                anchor: 'left',
                html: Drupal.t('<h3><a name="weightval" id="weightval"></a>#user_data</h3>' +
                   ' <p><strong>Used by</strong>: Custom elements</p>' +
                   '<p><strong>Description</strong>: Used by custom form elements such as tabpanels. Consult the documentation for more information on what values can be specified here.</p>')
              });
            }
          }
        }, {
          xtype: 'formgrid',
          title: Drupal.t('Submit'),
          id: 'submit',
          name: 'submit',
          store: Ext.create('Ext.data.Store', {
            fields:['value'],
            proxy: {
              type: 'memory',
              reader: {
                type: 'json'
              }
            }
          }),
          modelInitTmpl: {
            value: ''
          },
          columns: [{
            xtype: 'gridcolumn',
            dataIndex: 'value',
            header: Drupal.t('Functions'),
            sortable: true,
            flex: 1,
            field: {
              type: 'textfield'
            }
          }],
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'submit',
                anchor: 'left',
                html: Drupal.t('<h3><a name="submit-prop" id="submit-prop"></a>#submit</h3>' +
              '<p><strong>Used by</strong>: <a href="#button">button</a>, <a href="#form">form</a>, <a href="#image_button">image_button</a>, <a href="#submit">submit</a></p>' +
              '<p><strong>Description</strong>: Contains a list of submit callbacks to be excuted on the form, or only when a specific button is clicked.</p>' +
              '<p><strong>Values</strong>: An array of function names.</p>')
              });
            }
          }
        }, {
          xtype: 'formgrid',
          title: Drupal.t('Validate'),
          id: 'validate',
          name: 'validate',
          store: Ext.create('Ext.data.Store', {
            fields:['value'],
            proxy: {
              type: 'memory',
              reader: {
                type: 'json'
              }
            }
          }),
          modelInitTmpl: {
            value: ''
          },
          columns: [{
            xtype: 'gridcolumn',
            dataIndex: 'value',
            header: Drupal.t('Functions'),
            sortable: true,
            flex: 1,
            field: {
              type: 'textfield'
            }
          }],
          listeners: {
            render: function() {
              Ext.create('Ext.tip.ToolTip', {
                target: 'validate',
                anchor: 'left',
                html: Drupal.t('<h3><a name="validate" id="validate"></a>#validate</h3>' +
              '<p class="help"><strong>Used by</strong>: <a href="#button">button</a>, <a href="#image_button">image_button</a>, <a href="#form">form</a>, <a href="#submit">submit</a></p>' +
              '<p><strong>Description</strong>: A list of custom validation functions that need to be passed. This is usually used to add additional validation functions to a form, or to use an alternate function, as opposed to the default form validation function (the form ID with <em>_validate</em> appended to it).</p>' +
              '<p><strong>Values</strong>: An array of function names.</p>')
              });
            }
          }
        }]
      }]
    }],
    buttons: [{
      text: Drupal.t('Clear'),
      handler: function() {
        this.up('form').getForm().reset();
      }
    }],
    listeners: {
      hide: function() {
        Ext.formbuilder.saveElementForm();
      }
    }
  });
};
