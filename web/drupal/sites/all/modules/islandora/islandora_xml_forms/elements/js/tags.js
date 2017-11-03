/**
 * @file
 * Tags controls.
 */

Drupal.behaviors.xmlFormElementTags = function(context) {
  $('.xml-form-elements-tags .tag-editor:not(.processed)').keypress(function(e) {
    if (e.keyCode == '13') {
      e.preventDefault();
      var id = this.id;
      $('#' + id + '-add').trigger("mousedown");
      $('.xml-form-elements-tags .tag-editor').addClass('processed');
    }
  });
}


