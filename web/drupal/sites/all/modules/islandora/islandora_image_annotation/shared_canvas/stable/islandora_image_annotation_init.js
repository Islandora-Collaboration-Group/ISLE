// Toggle full window shared canvas.
(function ($) {
  Drupal.behaviors.islandorAnnoFullWindow = {
    attach: function (context, settings) {
      $('#full-window-button').click(function() {
        window_change_view_size();
      });
    }
  };
  $(document).keyup(function(e) {
    if (e.keyCode == 27) {
      // Only preform the 'ESC' functionality if in full screen.
      if ($('#full-window-button').html() != Drupal.t('Full Window')) {
        window_change_view_size();
      }
    }
  });
  // Add the 'ESC' key press exit full screen.
  function window_change_view_size() {
    $('.islandora-anno-wrapper').toggleClass('islandora-anno-fullwindow');
    resizeCanvas();
    if ($('#full-window-button').html() == Drupal.t('Full Window')) {
      $('#full-window-button').html(Drupal.t('Exit Full Window'));
      $('#islandora_shared_canvas_header').css('height', '0');
      
      $('.islandora-anno-wrapper').css('top', $('#admin-menu-wrapper').height());
      
      // Add a message to notify user of the 'ESC' button.
      $messageCont = $('<div class="message_cont">');
      $message = $('<div>Press ESC to exit full screen</div>').hide();
      $messageCont.append($message);
      $('#colright').prepend($messageCont);
      $message.fadeIn(400, function() {
          setTimeout(function(){
              $messageCont.fadeOut(400, function(){
                $('.message_cont').remove();
              });
          }, 3000);
      })
    }
    else {
      $('#full-window-button').html(Drupal.t('Full Window'));
      $('#islandora_shared_canvas_header').css('height', '0');
      if ($('.message_cont').length > 0) {
        $('.message_cont').remove();
      }
      
    }
  }
  
})(jQuery);

// Adapted from sc_init of the shared canvas project.
var startDate = 0;

var topinfo = {
    'canvasWidth' : 0,    // scaled width of canvas in pixels
    'numCanvases' : 0,    // number of canvases to display
    'current' : 0,        // current idx in sequence
    'done': [],           // URLs already processed
    'query' : null,       // top level databank
    'sequence' : [],      // Sequence list
    'sequenceInfo' : {},  // uri to [h,w,title]

    'annotations' : {
        'image' : {},
        'text' : {},
        'audio' : {},
        'zone' : {},
        'comment' : {}
    },
    'lists' : {
        'image' : {},
        'text' : {},
        'audio' : {},
        'zone' : {},
        'comment' : {}
    },
    'raphaels' : {
        'image' : {},
        'text' : {},
        'audio' : {},
        'zone' : {},
        'comment' : {}
    },

    'zOrders' : {
        'image' : 1,
        'detailImage' : 1000,
        'text' : 2000,
        'audio' : 3000,
        'zone' : 4000,
        'comment' : 5000
    },
    'canvasDivHash' : {},
    'builtAnnos' : [],
    'paintedAnnos' : [],
    'audioAnno' : null,
    'waitingXHR' : 0
};


var SVG_NS = "http://www.w3.org/2000/svg";
var XLINK_NS = "http://www.w3.org/1999/xlink";

var opts = {
    base : 'http://localhost/EmicShared/impl/',
    namespaces : {
        dc : 'http://purl.org/dc/elements/1.1/',
        dcterms : 'http://purl.org/dc/terms/',
        dctype : 'http://purl.org/dc/dcmitype/',
        oa : 'http://www.w3.org/ns/openannotation/core/',
        cnt : 'http://www.w3.org/2008/content#',
        dms : 'http://dms.stanford.edu/ns/',
        rdf : 'http://www.w3.org/1999/02/22-rdf-syntax-ns#',
        ore : 'http://www.openarchives.org/ore/terms/',
        exif : 'http://www.w3.org/2003/12/exif/ns#'
    }
};

function initCanvas(nCanvas) {
    var w = $('#canvas-body').width();
    var h = $('#canvas-body').height();
    
    $('#top_menu_bar').width(w - 5);

    for (var x = 0; x < nCanvas; x++) {
        $('#canvases').append('<div id="canvas_' + x + '" class="canvas"></div>')
        $('#canvas_' + x).width(w);
        $('#canvas_' + x).height(h);
    }
    topinfo['canvasWidth'] = w;
    topinfo['numCanvases'] = nCanvas;

    if (nCanvas > 2) {
        // Default text off if lots of canvases
        $('#check_show_text').attr('checked',false);
    }
};


function init_ui() {

  var anno_d = annotation_dialog();
  anno_d.dialog('close');

    $('.dragBox').draggable().resizable();
    $('.dragBox').hide();

    $('.dragShade').click(function() {
        var sh = $(this);
        if (sh.text() == '[-]') {
            sh.empty().append('[+]');
            var p = $(this).parent(); // header
            var h = p.height();
            var pp = p.parent(); // box
            var nh = pp.height();
            sh.attr('ph', nh);
            p.next().hide();
            pp.height(h + 6);

        } else {
            var n = sh.parent().next();
            var nh = sh.attr('ph');
            var p = sh.parent().parent();
            p.height(nh);
            sh.empty().append('[-]');
            n.show();
        }
    });

    $('#loadprogress').progressbar({
        value : 2
    }).css({
        height : 15,
        width : 300,
        opacity : 1.0,
        'z-index' : 10000
    });
    $('#loadprogress').position({
        of : '#create_annotation',
        my : 'left top',
        at : 'right top',
        collision : 'none',
        offset : '10 0'
    })

    $(".menu_body li:even").addClass("alt");

    // Link to menus
    $('.menu_head').click(function () {
        // First find our id, and then use to find our menu
        var id = $(this).attr('id');
        var menubody = $('#' + id + '_body')
        menubody.slideToggle('medium');
        menubody.position({
            'of' : '#' + id,
            'my' : 'top left',
            'at' : 'bottom left',
            'collision' : 'fit',
            'offset' : '0 8'
        })
    });

    try {
        // May not want to allow annotation
        maybe_config_create_annotation();
    } catch (e) {
    // XXX Remove annotation button and shape menu

    }
    $('#color-picker-wrapper').click(function(){
        $('#anno_color_activated').attr('value', 'active');
      });
      $('.color-picker').miniColors();
    // Refresh Canvas if browser is resized
    // We're called as per move... so need wait till finished resizing
    $(window).resize(function() {
        resizeCanvas();
    });
}

var timeout = false;
var delta = 300;
function resizeCanvas() {
  var w = $('#canvas-body').width();
  if(timeout === false) {
    timeout = true;
    closeAndEndAnnotating();
    window.setTimeout(maybeResize, delta);
  }
}

function maybeResize() {
    timeout = false;
    var w = $('#canvas-body').width();
    var image_element = $('.base_img').children(":first");
    
    initCanvas(topinfo['numCanvases']);
    image_element.width(w);
    image_element.css("height", "auto");
    $('.base_img').css("height", image_element.height());
    $('#canvas_0').css("width", w);
    
    // Set the top to zero.
    $('.islandora-anno-wrapper').css('top', 0);
    
    // Resize incase we have the admin menu available.
    if($('#admin-menu-wrapper').length > 0) {
      $('.islandora-anno-wrapper').css('top', $('#admin-menu-wrapper').height());
    }
}

// Let's start it up!
$(document).ready(function(){
    // gets setup information from Islandora
    $.ajax({
        url: Drupal.settings.basePath + 'islandora/anno/setup/'
          + Drupal.settings.islandora_image_annotation.PID,
        async:false,
        success: function(data, status, xhr) {
            islandora_canvas_params = data;
        },
        error: function(data, status, xhd) {
            alert("Please Login to site");
        },
        dataType: 'json'
    });

    //establish color-picker if allowed
    if(islandora_canvas_params.can_choose){
        $('#color-picker-wrapper').click(function(){
            $('#anno_color_activated').attr('value', 'active');
        });
        $('.color-picker').miniColors();
    }
    else{
        $('#color-picker-wrapper').empty();
    }

    if(islandora_canvas_params.no_edit == true){
        $('#create_annotation').hide();
    }
    else{
        $(function(){
            $.contextMenu({
                selector : '.comment_title',
                callback : function(key, options) {
                    var urn = $(this).attr('id');
                    urn = urn.substring(5,100);
                    var title = $(this).text().substring(2,100);
                    title = title.trim();

                    var comment_text = $(this).next('.comment_text');
                    var anno_type = comment_text.find('.comment_type').text();

                    if(key == 'delete'){
                        if (confirm("Permananently Delete Annotation '" + title + "'")) {
                            islandora_deleteAnno(urn);
                        }
                    }

                    if(key == 'edit'){
                        $(this).addClass('annotation-opened').next().show();
                        var annotation = comment_text.find('.comment_content').text();
                        var pm = $(this).find('.comment_showhide');
                        if (pm.text() == '+ ') {
                            pm.empty().append('- ');
                            var id = $(this).attr('id').substring(5,100);
                            var canvas = $(this).attr('canvas');
                            paint_commentAnnoTargets(this, canvas, id);
                        }
                        $('#hidden_annotation_type').attr('anno_type','Update Annotation');
                        $('#hidden_annotation_type').attr('urn',urn);
                        startEditting(title, annotation, anno_type, urn)
                    }
                },
                items: {
                    "edit": {
                        name : "Edit",
                        icon : "edit",
                        accesskey : "e"
                    },
                    "delete" : {
                        name : "Delete annotation",
                        icon : "delete"
                    }
                }
            });
        });
    }
    opts.base = islandora_canvas_params.object_base;

    if(islandora_canvas_params.use_dropdown == 1){
        $('#islandora_classification').empty();
        $('<label for="anno_classification">Type:</label>').appendTo('#islandora_classification');
        var sel = $('<select  id="anno_classification">').appendTo('#islandora_classification');
    
        $(islandora_canvas_params.categories).each(function() {
            value = this.toString();
            sel.append($("<option>").attr('value',value).text(value));
        });
    }
    else{
        $( "#anno_classification" ).autocomplete({
            source : islandora_canvas_params.categories
        });
    }
    if(islandora_canvas_params.islandora_anno_use_title_vocab == 1){
        $('#islandora_titles').empty();
        $('<label for="anno_title">Title:</label>').appendTo('#islandora_titles');
        var titles = $('<select  id="anno_title">').appendTo('#islandora_titles');
        titles.append($("<option>").attr('value','--Choose a type--').text('--Choose a type above to populate--'));
    }

    $("#anno_classification").change(function()
    {
        if(islandora_canvas_params.islandora_anno_use_title_vocab == 1){
            var id=$(this).val();
            var base_url = islandora_canvas_params.islandora_base_url + 'islandora/anno/solr/title/terms/';
            $.getJSON(base_url + id,{
                id : $(this).val(),
                ajax : 'true'
            }, function(j){
                var options = '<option value="nothing">--Select from ' + id + '--</option>';
                for (var i = 0; i < j.length; i++){
                    var fieldName, objectPid;
                    $.each(j[i], function (key, val){
                        if(key =='PID'){
                            objectPid = val;
                        } else {
                            fieldName = val;
                        }
                    });
                    options += '<option value="' + objectPid + '">' + fieldName + '</option>';
                }
                if(j.length == 0){
                    $('#islandora_titles').empty();
                    $('#islandora_titles').append('<label for"anno_title">Title:</label>');
                    $('#islandora_titles').append('<input id="anno_title" type="text" size="28"/>');
                }
                else {
                    $('#islandora_titles').empty();
                    $('<label for="anno_title">Title:</label>').appendTo('#islandora_titles');
                    var titles = $('<select  id="anno_title">').appendTo('#islandora_titles');
                    titles.append($("<option>").attr('value','--Choose a type--').text('--Choose a type above to populate--'));
                    $('#anno_title').html(options);
                    $("#anno_title").change(function()
                    {
                        var id = $(this).val();
                        var mads_url = islandora_canvas_params.islandora_base_url + 'islandora/anno/mads/';
                        $.getJSON(mads_url+id,{
                            id : $(this).val(),
                            ajax : 'true'
                        }, function(mads){
                            var mads_text = "";
                            $.each(mads, function(i, val) {
                                mads_text += i +': ' +val + '\n\n';
                            });
                            $('#anno_text').val(mads_text);
                        });
                    });
                }
            });
        }
    });
    
    var stroke_widths = islandora_canvas_params.islandora_anno_stroke_widths.split(" ");
    var s_options = "";
    for (var i = 0; i < stroke_widths.length; i++) {
      s_options += '<option value="'+ stroke_widths[i] + '">' + stroke_widths[i] + '</option>';
    }
    $('#stroke-width-wrapper').empty();
    $('#stroke-width-wrapper').append('<label for"stroke_width">Stroke Width:</label>');
    $('#stroke-width-wrapper').append('<select id="stroke_width" />');
    $('#stroke_width').append(s_options);
    
    // RDF Initializationc
    var rdfbase = $.rdf(opts);
    topinfo['query'] = rdfbase;

    var l = $(location).attr('hash');
    var uriparams = {};
    var nCanvas = 1;
    var start = 0;
    if (l[0] == '#' && l[1] == '!') {
        // Process initialization
        var params = l.substr(2,l.length).split('&');
        for (var p=0,prm;prm=params[p];p++) {
            var tup = prm.split('=');
            var key = tup[0];
            var val = tup[1];
            if (key == 's') {
                start = parseInt(val);
                uriparams['s'] = start;
            }
            else if (key == 'n') {
                nCanvas = parseInt(val);
                uriparams['n'] = nCanvas;
            }
        }
    }
    topinfo['uriParams'] = uriparams
    // Initialize UI
    init_ui();
    // Setup a basic Canvas with explicit width to scale to from browser width
    initCanvas(nCanvas);

    // Manifest initialization.
    fetchTriples(islandora_canvas_params.manifest_url,
                 rdfbase,
                 cb_process_manifest);
    resizeCanvas();
});
