/**
 * Produces a jQuery dialog box, used for
 * creating an image annotations in the
 * image annotation module and the critical
 * edition module.
 * @returns DialogBox(jQuery)
 */
function annotation_dialog() {
  var img_base_path = Drupal.settings.islandora_image_annotation.images_path;
  var html_text = ''+
 '<div id="create_annotation_box">'+
 '<div id="hidden_annotation_type" type="hidden"></div>'+
    '<div style="display:inline; margin-top: 3px; padding-left: 5px;">'+
      '<img id="annoShape_rect" class="annoShape" src="' + img_base_path + '/draw_rect.png" style="padding-left: 2px; padding-top: 1px;"/>'+
      '<img id="annoShape_circ" class="annoShape" src="' + img_base_path + '/draw_circ.png" style="padding-left: 1px;"/>'+
      '<img id="annoShape_poly" class="annoShape" src="' + img_base_path + '/draw_poly.png" style="padding: 2px;"/>'+
      '<hr style="margin: 0px; padding: 0px; height: 1px;"/>'+
    '</div>'+
    '<div id="create_annos_block" class="dragBlock">'+
      '<div class="element-wrap">'+
        '<label for="anno_title">Title:</label>'+
        '<input id="anno_title" type="text" size="28"></input>'+
      '</div>'+
      '<div id ="islandora_classification" class="element-wrap">'+
        '<div id="type_wrapper" class="element-wrap">'+
          '<label for="anno_type">Type:</label>'+
          '<input id="anno_classification" type="text" size="28"></input>'+
        '</div>'+
      '</div>'+
      '<div id ="color-picker-wrapper" class="element-wrap">'+
        '<label for="anno_color">Color:</label>'+
        '<input id ="anno_color" type="hidden" name="color4" value="#91843c" class="color-picker" size="7" />'+
        '<input id ="anno_color_activated" type="hidden" value ="" size="7" />'+
      '</div>'+
      '<div id ="stroke-width-wrapper" class="element-wrap">'+
        '<label for="stroke_width">Stroke Width:</label>'+
        '<input id="stroke_width" type="text" size="5" value=".3"></input>'+
      '</div>'+
      '<div class="element-wrap">'+
        '<label for="anno_text">Annotation:</label>'+
        '<textarea id="anno_text" cols="40" rows="5"></textarea>'+
      '</div>'+
      '<span style="width:200px;margin:0px;padding:0px;float:left">'+
        '<ul id="create_body" style="width: 200px; list-style:none;font-size:10pt;margin:0;padding:0;">'+
        '</ul>'+
      '</span>'+
    '</div>'+
  '</div>';
  var entity_combobox = ''+
  '<div id="entity_wrapper" class="element-wrap">'+
    '<label for="cboAddEntity">Entity</label>'+
    '<select id="cboAddEntity" class="easyui-combobox" name="Add Entity" style="width:100px;">'+
      '<option>Tag Person</option>'+
      '<option>Tag Event</option>'+
      '<option>Tag Place</option>'+
      '<option>Tag Organization</option>'+
    '</select>'+
    '<input id="cboEntityLookup"></input>'+
    '<div id="hidden_entity" type="hidden" data-entity=""></div>'
  '</div>';

  $(document.body).append(html_text);
  $('#islandora_classification').addClass("dialog-entity-group");
  
  // Optionally allow/disallow users from tagging entities.
  if (Drupal.settings.islandora_image_annotation.allow_entity_linking) {
    $('#islandora_classification').append(entity_combobox);
    $('#cboEntityLookup').autocomplete({
      source: function(request,response){
        // lookup_entity is shared by image annotation and critical editions,
        // so it has been exposed to both via 'lookup_entity() function in
        // entity_search.js.
        var result = lookup_entity($('#cboAddEntity').find('option:selected').text() + '/' + '?entities_query=' + request.term);
        response($.map(result, function(item){
          return {
            label: item.identifier,
            value: item.identifier,
            data: item.Object,
          };
        }));
      },
      select: function(event, ui) {
        // Add the selected entity data to the hidden
        // 'hidden_entity' field.
        $('#hidden_entity').data('entity',ui.item);
      },
    });
  }
  var annotation_dialog = $('#create_annotation_box');
  return annotation_dialog.dialog({
    modal: true,
    open: function(event,ui) {
      // Fix to paint random colors.
      if(Drupal.settings.islandora_image_annotation.can_choose == 1) {
        var rcol = get_random_color();
        $('#anno_color').attr('value',rcol);
        $('.miniColors-trigger').css('background-color',rcol);
        $('#color-picker-wrapper').hide();
      }
    },
    title: 'Annotate',
    resizable: false,
    closeOnEscape: false,
    height: 470,
    width: 380,
    buttons: {
    'Save': function() {
      var ret = saveAndEndAnnotating();
      if (ret != 0) {
        closeAndEndAnnotating();
        // Reset hidden data for the next time this 
        // dialog is used.
        if ($('#hidden_entity')) {
          $('#hidden_entity').data('entity','');
        }
        $('#hidden_annotation_type').attr('anno_type','');
        $('#hidden_annotation_type').attr('urn','');
        annotation_dialog.dialog('close');
      }
    },
    'Cancel': function() {
      closeAndEndAnnotating();
      annotation_dialog.dialog('close');
      $('#hidden_annotation_type').text('');
    }
  }
  });
};
