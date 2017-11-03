/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * 
 */
Ext.formbuilder.getPreviewURL = function(url) {
    var view_url = url.replace(/\/edit/i, '/view');
    return "<iframe src='" + view_url + "' width='100%' height='100%'><p>" + Drupal.t('Your browser does not support iframes.') + "</p></iframe>";
}
/**
 * 
 */
Ext.formbuilder.createPreviewPanel =  function(url) { // Use an iframe...
    var preview = Ext.formbuilder.getPreviewURL(url);
    return Ext.create('Ext.form.Panel', {
        title: Drupal.t('Preview'),
        html: preview
    });
};
/**
 * 
 */
Ext.formbuilder.refreshPreviewPanel =  function(url) { // Use an iframe...
    var display = this.displayPanel.layout;
    var preview = display.setActiveItem(0);
    if(preview) {
        preview.update(Ext.formbuilder.getPreviewURL(url));
    }
    else {
        preview = display.getActiveItem();
        preview.update(Ext.formbuilder.getPreviewURL(url));
    }
};
