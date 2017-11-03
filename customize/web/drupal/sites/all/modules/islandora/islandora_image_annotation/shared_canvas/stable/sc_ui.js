var top_reqs = 0;
var top_finished = 0;
var MAX_TXT_SIZE = 18;

function ping_progressBar(flag) {
  var pb = $('#loadprogress')
 
  var c = pb.progressbar('value')

  if (flag == 'manifest') {
    var val = 20;
    top_finished = 0;
  } else if (flag == 'manifestDone') {
    val = 30;
  } else if (flag == 'sequence') {
    var val = 40;
  } else if (flag == 'sequenceDone') {
    var val = 60;
  } else if (flag.substring(0,4) == "req:") {
    top_reqs += 1
    var	val = c + 5;
  } else if (flag.substring(0,5) == 'recv:') {
    top_reqs -= 1
    var val = c + 5;
  } else if (flag == 'finishReqs') {
    top_finished = 1;
    var val = c + 5;
  }

  if (top_finished && top_reqs == 0) {
    val = 0;
    pb.hide();
  }
  var c = pb.progressbar('value', val);

}

function populate_jumpMenu(qry) {
  // Push 10-12 evenly spaced links to Jump menu
  var l = topinfo['sequence'];
  var ranges = topinfo['ranges'];

  var jmp = $('#jump_body');
  jmp.empty();

  // Maybe add in bonus links
  try {
    maybe_add_jump_links();
  } catch (e) {} // doesn't exist

  // Ranges
  if (ranges.length) {
    var idxs = {}
    var idxl = []
    for (var x=0,item;item=ranges[x];x++) {
      if (item[0] != '') {
        var idx = l.indexOf(item[1])
        idxs[idx] = item[0];
        idxl.push(idx);
      }
    }
    idxl.sort(function(a,b){
      return a-b
    });
    for (var x=0,idx; idx=idxl[x];x++) {
      title = idxs[idx];
      jmp.append('<li><span onclick="jumpPage('+idx+')">' + title + '</span></li>')
    }
    jmp.append('<li><hr/></li>')
  }

  if (ranges.length > 2) {
    n = 8;
  } else {
    n = 12;
  }
  if (l.length > n) {
    var evry = Math.floor(l.length / n);
  } else {
    var evry = 1;
  }

  for (var x=0; x < l.length; x+= evry) {
    var canvas = l[x];
    var info = topinfo['sequenceInfo'][canvas];
    if (info == undefined) {
      info = extract_canvas_size(qry, canvas)
      topinfo['sequenceInfo'][canvas] = info
    }
    var ttl = info[2];
    $('#jump_body').append('<li><span onclick="jumpPage(' + x + ')">' + ttl + '</span></li>')
  }
  $("#jump_body li:even").addClass("alt");
}

// Sequence Selector box
function show_seqSel(seqs) {
  // uri, ?title, ?description
  var trail = new RegExp('^.+/([^/]+)$');
  var bl = $('#seqSel_block');
  bl.empty();
  for (var s=0,seq;seq=seqs[s];s++) {
    var uri = seq[0].toString();
    var ttl = seq[1];
    if (ttl == '') {
      var m = uri.match(trail);
      ttl = m[1];
    }
    var line = '<li><span onclick="clickThruSequence(\'' + uri + '\');">' + ttl + '</span>';
    if (s[2] != '' && s[2] != undefined) {
      line += '<br/>' + s[2];
    }
    line += '</li>';
    bl.append(line);
  }
  $('#seqSel').show();
}



// Painting Canvases

function showPages() {
  // Empty current display
  $('#annotations').empty();
  $('#audio_annotations').empty();
  $('#comment_annos').hide();
  $('#comment_annos_block').empty();
  $(".canvas").empty();
  $('#svg_wrapper').empty();
  $('#title').empty();
  $('#next').empty();
  $('#prev').empty();
  $('#imgSel_block').empty();
  $('#imgSel').hide();
  $('.canvasUri').remove();

  try {
    $('#create_annotation_box').hide();
  } catch(e) {};

  topinfo['canvasDivHash'] = {};
  topinfo['raphaels'] = {
    'image':{},
    'text':{},
    'audio':{},
    'zone':{},
    'comment':{}
  };
  topinfo['paintedAnnos'] = [];

  // Reset URI to correct #fragment info
  topinfo['uriParams']['s'] = topinfo['current'];
  var hsh = makeUriHash();
  $(location).attr('hash',hsh);

  // Initialize to first Canvas div
  topinfo['currentCanvas'] = 0;
  // Show canvases
  for (var x=0;x<topinfo['numCanvases'];x++) {
    topinfo['currentCanvas'] = x;
    showPage();
    topinfo['current'] = topinfo['current'] + 1;
  }

  // Current is seq index for *first* canvas
  topinfo['current'] = topinfo['current'] - topinfo['numCanvases'];
  repositionCanvases();

  ping_progressBar('finishReqs');

}

function showPage() {

  // Index in Sequence of Canvases
  var which = topinfo['current'];
  // Index in set of divs
  var wCanvas = topinfo['currentCanvas'];
  // id for current div
  var canvasId = 'canvas_' + wCanvas;
  // List of Canvases
  var seq = topinfo['sequence'];
  // Current Canvas
  var canvas = seq[which];

  // Build Nav links
  showNavLinks(canvas, which, wCanvas, seq)
  // And start to paint canvas

  topinfo['canvasDivHash'][canvas] = canvasId;
  $("#"+canvasId).attr('canvas', canvas);
  showCanvas(canvas, canvasId)
}

function showNavLinks(canvas, which, wCanvas, seq) {

  var qry = topinfo['query']

  // Info for current canvas
  var info = topinfo['sequenceInfo'][canvas];
  if (info == undefined) {
    info = extract_canvas_size(qry, canvas)
    topinfo['sequenceInfo'][canvas] = info
  }
  //
  //      Removed for EMiC
  //
  //	var title = info[2];
  //	var ttl = $('#title');
  //	var curr = ttl.text()
  //	if (!curr) {
  //		$('#title').append(title)
  //	} else {
  //		var cl = curr.split(' ')
  //		var nl = title.split(' ')
  //		while (cl && nl && cl[0] == nl[0]) {
  //			cl.shift();
  //			nl.shift();
  //		}
  //		title = nl.join(' ');
  //		ttl.append(', ' + title)
  //	}

  // Info for next canvas, if there is one
  $('#next').empty();
  if (wCanvas < seq.length-1)
  {
    $('#next').append('&nbsp; <button class="menu_head">next &rarr;</button>')
  }

  // Info for previous canvas
  if (wCanvas == 0 && which > 0) {
    $('#prev').append('<button class="menu_head">&larr; prev</button> &nbsp;')
  }
}

function showCanvas(canvas, canvasId) {
  var info = topinfo['sequenceInfo'][canvas];
  var cvsh = info[0];
  var cvsw = info[1];
  var scale = topinfo['canvasWidth'] / cvsw;
  var sh = cvsh * scale;
  $('#' + canvasId).height(sh)


  // Maybe show Canvas URI
  if (1) {
    var mw = (cvsw * scale)/2;
    var cvsuri = '<span style="font-size: 10pt; z-index: 15000; padding-top:6px; padding-bottom: 6px;" id="canvasUri_'+
    canvasId+'" class="canvasUri"><input size="40" id="canvasUriInput_'+canvasId+'" style="width:'+
    (mw-30)+' border: 1px solid black;" "type="text" value="'+canvas+'"/></span>';
    $('#dialogs').append(cvsuri);

    var cud = $('#canvasUri_'+canvasId);
    cud.position({
      'of':'#' + canvasId,
      'my':'left top',
      'at':'left top',
      'collision':'none',
      'offset':'20 -10'
    });
    var cudi = $('#canvasUriInput_'+canvasId);
    cudi.click(function() {
      $(this).select();
    });

    if (!$('#check_view_uri').is(':checked')) {
      cud.hide();
    }
  }

  // Start sucking down annotations
  fetch_comment_annotations();
  fetch_annotations('zone', canvas);
  fetch_annotations('image', canvas);
  fetch_annotations('text', canvas);
  fetch_annotations('audio', canvas);
  fetch_annotations('comment', canvas);
  fetch_comment_annotations();
}

function paint_annos() {
  // Step through all displayed canvases and paint all finished, unpainted annos
  // Do it this way as can't predict when annos will be available due to
  // different AJAX speeds
  for (canvas in topinfo['canvasDivHash']) {
    var div = topinfo['canvasDivHash'][canvas];
    for (typ in topinfo['annotations']) {
      var annos = topinfo['annotations'][typ][canvas];
      for (var a in annos) {
        var anno = annos[a];
        if (anno.finished && topinfo['paintedAnnos'].indexOf(anno.id) == -1) {
          topinfo['paintedAnnos'].push(anno.id);
          paint_anno(typ, anno, div);
        }
      }
    }
  }
}

function mk_raphael(typ, canvas, canvasId) {
  if (topinfo['raphaels'][typ][canvas] != undefined) {
    return topinfo['raphaels'][typ][canvas];
    } else {
    var info = topinfo['sequenceInfo'][canvas];
    if (info == undefined) {
      info = extract_canvas_size(topinfo['query'], canvas)
      topinfo['sequenceInfo'][canvas] = info
    }
    var cvsw = info[1];
    var cvsh = info[0];
    var scale = topinfo['canvasWidth'] / cvsw;
    var sh = cvsh * scale;
    var sw = cvsw * scale;
    var svgWrap = $('#svg_wrapper');
    svgWrap.append('<div class="svg_canvas_wrapper" id="svg_annos_' + typ + '_' + canvasId + '"></div>');
    var svg = $('#svg_annos_' + typ + '_' + canvasId);
    svg.height(sh);
    svg.width(sw);
    z = topinfo['zOrders'][typ] + 1; // Allow a base image at 1
    svg.css('z-index', z)
    svg.css('position', 'absolute')
    svg.position({
      'of':'#'+canvasId,
      'my':'left top',
      'at':'left top',
      'collision':'none'
    });

    var svgcanvas = ScaleRaphael('svg_annos_'+typ + '_' + canvasId, cvsw, cvsh);
    svgcanvas.changeSize(sw, sh, false, false);
    if ($.browser.webkit) {
      svgcanvas.safari();
    }
    topinfo['raphaels'][typ][canvas] = svgcanvas;
    return svgcanvas;
  }
}

function paint_anno(typ, anno, div) {

  try {

    if (typ == 'image') {
      paint_imageAnno(anno, div);
    } else if (typ == 'text') {
      paint_textAnno(anno, div);
    } else if (typ == 'audio') {
      paint_audioAnno(anno, div);
    } else if (typ == 'comment') {
      paint_commentAnno(anno, div);
    }

  } catch(e) {
    alert('paint error: ' + e)
  }
}


function swapImage(radio, canvasId, annoId, optId) {
  var canvas = $('#' + canvasId).attr('canvas');
  var annos = topinfo['annotations']['image'][canvas];
  var anno = null;
  for (var i=0;anno=annos[i];i++) {
    if (anno.id == annoId) {
      for (var b=0,bodo;bodo=anno.body.options[b];b++) {
        if (bodo.id == optId) {
          anno.body.defaultOpt = bodo;
          break;
        }
      }
      break;
    }
  }
  if (anno != null) {
    var myid = 'imganno_' + anno.id.substring(9, 100)
    $('#'+myid).remove();
    $('#imgSel_set_' + canvasId).remove();
    paint_imageAnno(anno, canvasId)
  }
}

function show_zpr(what) {
  // Stupid rewrite as no access to African

  $('body').append('<div id="zpr-wrap"></div><span style="border: 1px solid black;position:absolute; top:5;right:5;z-index:20001; background: white; padding: 2px;" id="zpr-close"><b>close</b></span><div id="zpr-fullscreen"></div>');
  $('#zpr-close').click(function() {
    $('#zpr-wrap').remove();
    $('#zpr-fullscreen').remove();
    $('#zpr-close').remove();
  });

  $('#zpr-wrap').width($(window).width());
  $('#zpr-wrap').height($(window).height());
  $('#zpr-fullscreen').width($(window).width() - 50);
  $('#zpr-fullscreen').height($(window).height() - 50);
  var basefile = "file:///home/azaroth/jp2/W165/";
  var st = what.indexOf('W165_0');
  var fn = what.substr(st, 100);
  var rez = '600';
  var w = 3815;
  var h = 3997;

  // 003 = w3633 h3997
  // 004 = w3591 h3997

  if (fn.indexOf('001_') > -1 || fn.indexOf('030_') > -1 ||fn.indexOf('031_') > -1 || fn.indexOf('035_')> -1) {
    rez = '1200';
    w = 6998;
    h = 7975;
  }
  fn = fn.replace('sap', rez);
  fn = fn.replace('jpg', 'jp2');
  fn = basefile + fn;
  // alert(fn);
  var z = new zpr('zpr-fullscreen', {
    'jp2URL': fn,
    'width': w ,
    'height': h,
    'marqueeImgSize': 150
  });

}

function paint_imageAnno(anno, canvasId) {
  var canvas = $('#' + canvasId).attr('canvas');
  var info = topinfo['sequenceInfo'][canvas];
  var cvsw = info[1];
  var cvsh = info[0];
  var scale = topinfo['canvasWidth'] / cvsw;
  var sh = scale * cvsh;
  var sw = scale * cvsw;

  if (anno.id.substring(0,9) == 'urn:uuid:') {
    var myid = 'imganno_' + anno.id.substring(9,1000);
  } else {
    aid = anno.id.substring(8,1000)
    aid = aid.replace(/\./g, '_');
    aid = aid.replace(/\//g, '_');
    aid = aid.replace(/-/g, '_');
    var myid = 'imganno_' + aid;

  }

  // First find which image to use!
  var img = null;
  if (anno.body.options.length > 0) {

    // Display choice with best default
    anno.body.options.sort(function(a,b){
      return a.id > b.id ? 1 : -1
    });
    if (img == null) {
      if (anno.body.defaultOpt != null) {
        img = anno.body.defaultOpt;
      } else {
        img = anno.body.options[0];
      }
    }

    html = '<div id="imgSel_set_' + canvasId + '">';
    html += '<span class="imgSel_canvas">' + info[2] + ':</span><ul class="imgSelul">'
    for (var o = 0, iopt; iopt=anno.body.options[o];o++) {
      var ittl = iopt.title == '' ? "Image " + o : iopt.title;
      if (iopt.id == img.id) {
        // Default is a different object to options
        var chkd = " checked";
        ittl = '<b>' + ittl + '</b>';
      } else {
        var chkd = "";
      }
      html += '<li>'+ittl+'<span style="float:right"><input type="radio" name="imgSelRadioItem" onclick="swapImage(this, \''+canvasId+'\',\''+ anno.id + '\',\''+iopt.id+'\')" class="imgSelRadio"'+chkd+'></span></li>'
    }
    html += "</ul></div>"
    $('#imgSel_block').append(html);
    $(".imgSelul li:even").addClass("alt");
    $('.imgSelRadio').click(function() {
    
      })
    if ($('#check_view_imgSel').is(':checked')) {
      alert("Its checked")
      $('#imgSel').show();
    }
  } else {
    alert('abnno body')
    img = anno.body;
  }

  // And Display it
  var xywh = getRect(img);
  if (xywh != null) {
    var x = xywh[0], y = xywh[1], w = xywh[2], h = xywh[3];
  }


  if (img.rotation == 0 && anno.targets[0].partOf == null && img.partOf == null) {
    // Full Image annotates Full Canvas
    var imgid = 'img_' + myid
    var imguri = img.id;

    $("#annotations").append('<div class="base_img" id="'+myid+'"><img src="'+imguri+'" id="'+imgid+'"></div>');
    var div = $('#' + myid)
    div.position({
      'of':'#'+canvasId,
      'my':'left top',
      'at':'left top',
      'collision':'none',
      'offset': '0 0'
    });
    
    // William Panting: changed imgid selector because of possible characters needing escaping in jquery.
    //var imge = $('#'+imgid)
    var imge = $('.base_img').children(":first");
    
    imge.width(sw)
    imge.height(sh)
    //div.width(sw)
    div.height(sh);
    div.css('z-index', topinfo['zOrders']['image']);

    //    if (!$('#check_show_baseImg').is(':checked')) {
    //      div.hide();
    //    }

    var zpr = '<div id="zpr_'+canvasId+'" style="position:absolute;"><button id="zprb_'+canvasId+'">zoom</button></div>';
    $('#annotations').append(zpr);
    var zprs = $('#zpr_'+canvasId);
    zprs.css('z-index',9000);
    zprs.position({
      'of':'#' + canvasId,
      'my':'left top',
      'at':'left top',
      'collision':'none',
      'offset':'2 2'
    });
    var zprb = $('#zprb_'+canvasId);
    zprb.click(function() {
      show_zpr(imguri)
    });


    if (!$('#check_view_zpr').is(':checked')) {
      zprs.hide();
    }

  } else if (img.rotation == 0 && (xywh != null || !img.partOf)) {

    // Full Image/Rect Segment annotates Rect Segment of Canvas
    // Can get by with just regular HTML and CSS

    if (img.partOf == null) {
      var imguri = anno.body.id;
      clip = '';

    } else {
      var imguri = img.partOf.id;
      // top, right, bottom, left offsets from edges == STUPID!
      // Clip needs to be scaled against size of full image

      var ttlh = 1088;
      var ttlw = 816;

      var clipScale = sw / w;
      var imgh = ttlh * clipScale;
      var imhw = ttlw * clipScale;

      var clip = 'rect('+(y*clipScale)+'px,'+((x+w)*clipScale)+'px,'+((y+h)*clipScale)+'px,'+(x*clipScale)+'px)'
      var zIdx = topinfo['zOrders']['image'] + anno.zOrder;
      var divh = sh;
      var divw = sw;
      var offset = "0 0"
      var imgoffleft = (x*clipScale);
      var imgofftop = (y*clipScale);
    }

    var xywh = getRect(anno.targets[0]);
    if (xywh != null) {
      var tx = xywh[0], ty = xywh[1], tw = xywh[2], th = xywh[3];
      var offset = "" + Math.floor(tx*scale) + ' ' + Math.floor(ty*scale)
      var zIdx = topinfo['zOrders']['detailImage'] + anno.zOrder;
      var imgh = th * scale;
      var imgw = tw * scale;
      var divw = imgw;
      var divh = imgh;
    }

    var body = '<img src="'+ imguri +'" id="img_'+ myid +'"/>';
    $("#annotations").append('<div class="img_anno" id="' + myid + '" >'
      + body + '</div>');
    var div = $('#' + myid);
    div.css('z-index', zIdx);
    div.height(divh);
   // div.width(divw);

    img = $('#img_' + myid);

    if (clip) {
      img.css('position', 'absolute');
      img.offset({
        top:div.position().top - imgofftop,
        left:div.position().left-imgoffleft
      });
      img.css('clip', clip);
    }
    img.height(imgh);
    img.width(imgw);

    div.position({
      'of':'#'+canvasId,
      'my':'left top',
      'at':'left top',
      'collision':'none',
      'offset': offset
    });
    if (!$('#check_show_detailImg').is(':checked')) {
      div.hide();
    }

  } else if (img.rotation && anno.targets[0].partOf == null) {
    // rotated image targets entire canvas
    // No need to clip, just translate top corner to correct place
    // and then scale to correct size. Non visible stuff will be outside SVG
    // viewport and thus not display

    var fullImage = img.partOf;
    var svgcanvas = mk_raphael('image', canvas, canvasId)
    var r = img.fragmentInfo;
    var x = svgcanvas.image(fullImage.id, 0, 0, fullImage.width, fullImage.height)
    x.node.setAttribute('preserveAspectRatio', 'none')

    x.rotate(img.rotation,r[0],r[1])
    if (img.rotation == 90) {
      x.translate(r[3]-r[0], 0-r[1])
    } else {
      x.translate(0-r[0],r[2]-r[1])

    }



  } else {
    // Non Rect Segment of Image annotates (Canvas / Segment of Canvas)
    // Use SVG clip, either from SVG or media fragment
    // OR full image that needs rotation
    // XXX This cannot be unshown, and re-orders all together as just one SVG canvas

    var fullImage = img.partOf;
    var svgcanvas = mk_raphael('image', canvas, canvasId)

    // reset z-index to detail
    $(svgcanvas.wrapperElem).css('z-index', topinfo['zOrders']['detailImage'])
    var target = anno.targets[0];
    if (img.constraint != null) {
      var svg = img.constraint.value;
    } else if (img.fragmentType == 'rect') {
      // Construct SVG rect from fragmentInfo
      var r = img.fragmentInfo;
      var svg = "<rect xmlns='"+SVG_NS+"' x='"+r[0]+"' y='"+r[1]+"' width='"+r[2]+"' height='"+r[3]+"'/>";
    }

    var pth = $.parseXML(svg)
    var doc = $(pth)
    var pthelm = doc.children()[0];
    // Duplicating into the DOM for webkit
    var npth = document.createElementNS(SVG_NS, pthelm.nodeName)
    for (var a in pthelm.attributes) {
      var attr = pthelm.attributes[a];
      npth.setAttribute(attr.nodeName, attr.nodeValue);
    }
    pthelm = npth;

    // Now determine if target is full canvas or fragment

    if (target.fragmentType == 'rect') {
      var th = target.fragmentInfo[3];
      var tw = target.fragmentInfo[2];
      var tx = target.fragmentInfo[0];
      var ty = target.fragmentInfo[1]
    } else {
      var tw = cvsw;
      var th = cvsh;
      var tx = 0;
      var ty = 0;
    }

    var sc = th / fullImage.height;

    // And build SVG clippath
    var cpe = document.createElementNS(SVG_NS, 'clipPath')
    cpe.setAttribute('id', 'svgclippath' + canvasId);
    // Need to translate and scale the path as well as the image
    pthelm.setAttribute('transform', 'translate(' + tx + ' ' + ty + ') scale(' + sc + ')')

    cpe.appendChild(pthelm)
    svgcanvas.canvas.appendChild(cpe)

    var x = svgcanvas.image(fullImage.id, 0, 0, fullImage.width, fullImage.height)
    x.node.setAttribute('preserveAspectRatio', 'none')
    x.node.setAttribute('clip-path', 'url(#svgclippath' + canvasId + ')')
    x.scale(sc, sc, 0, 0)
    x.translate(tx, ty)
  }
}

function process_bodyValue(anno) {
  var xml = anno.body.value;
  $(xml).find('abbr').remove();
  $(xml).find('sic').remove();

  var words = $(xml).find('seg[type="word"]');
  for (var i=0,w;w=words[i];i++) {
    txt = $(w).text();
    txt = txt.replace(/\s/g, '')
    $(w).empty();
    $(w).append(txt);
  }

  txt = xml.text();
  txt = txt.replace(/\s+/g, ' ')
  anno.body.value = txt;
}


function paint_textAnno(anno, canvasId) {
  nss = opts.namespaces;
  var canvas = $('#' + canvasId).attr('canvas');
  var myid = anno.id.substring(9, 1000);

  // Check if body.value needs processing further
  if (typeof anno.body.value != "string") {
    // Manipulate in place
    process_bodyValue(anno);
  }

  var xywh = getRect(anno.targets[0]);
  if (xywh != null) {
    var tx = xywh[0], ty = xywh[1], tw = xywh[2], th = xywh[3];

    // Fetch scale
    var cvsw = topinfo['sequenceInfo'][canvas][1];
    var scale = topinfo['canvasWidth'] / cvsw;
    var offset = "" + Math.floor(tx*scale) + ' ' + Math.floor(ty*scale);
    // Configurable per instance
    var txtpx = ((tw * scale) / anno.body.value.length) * 2.1;
    var txtpx = Math.min(txtpx, MAX_TXT_SIZE) // at most MTS
    var txtpx = Math.max(txtpx, 10) // at least 8 pt

    $("#annotations").append('<div class="text_anno" id="text_' + myid + '" >' + anno.body.value + '</div>');
    var danno = $('#text_' + myid);
    danno.css('z-index', topinfo['zOrders']['text'] + anno.zOrder)
    danno.height(Math.floor(th * scale));
    danno.width(Math.floor(tw * scale));

    danno.position({
      'of':'#' + canvasId,
      'my':'left top',
      'at':'left top',
      'collision':'none',
      'offset': offset
    });
    danno.css('font-size', '' + txtpx + 'px')

    if (anno.body.types.indexOf(nss['dms']+'RubricBody') > -1) {
      danno.addClass('text_anno_rubric');
    }
    if (anno.body.types.indexOf(nss['dms']+'InitialBody') > -1) {
      danno.addClass('text_anno_initial');
      // Try to guess better font-size from width
      danno.css('font-size', '' + (th*scale/1.2) + 'px')
    }

  } else if (anno.targets[0].constraint != null) {
    // SVG text path. Not supported by Raphael
    // http://www.w3.org/TR/SVG/text.html#TextPathElement

    var svgcanvas = mk_raphael('text', canvas, canvasId);

  // XXX
  // <defs><path id="IDFORPATH" d="path commands here"/></defs>
  // <text> <textPath xlink:href="#IDFORPATH">text</textPath> </text>

  } else {
  // text associated with canvas, rather than segment
  // not sure how to render intuitively?

  }

  try {
    MathJax.Hub.Typeset()
    danno.css('font-size', '18px');
  } catch (e) {};

  if (!$('#check_show_text').is(':checked')) {
    danno.hide();
  }

}




// UI for Comment Annotations

// paint title to a floating list in #comment_annos
// when selected, paint targets (if any)

function paint_commentAnno(anno, canvasId) {
  var title = anno.title;
  var annoType = anno.annoType;
  // remove illegal characters
  var fixed_annotype = annoType.replace(/[^\w]/g,'');
  var txt = anno.body.value;
  var myid = anno.id.substring(9, 100);
  var tgttxt = '';
  for (var x=0,tgt;tgt=anno.targets[x];x++) {
    if (tgt.partOf != null) {
      tgt = tgt.partOf.id;
      var tgtttl = topinfo['sequenceInfo'][tgt][2];
      tgttxt = 'part of ' + tgtttl;
    } else {
      tgt = tgt.id;
      var tgtttl = topinfo['sequenceInfo'][tgt][2];
      tgttxt = 'all of ' + tgtttl;
    }
  }
  txt = txt.replace(/\n/g, '<br/>');
  
  var entity_link = '';

  if(anno.relation) {
    entity_link = '<div class="comment_entity">' + '<a href="' + Drupal.settings.islandora_image_annotation.entity_link_path +
      anno.relation +'">' + 
      anno.hasPart + '</a>' + '</div>';
  }
  //block contains complete annotation
  block = '<div class = "canvas_annotation" ' + 'urn ="' + myid + '" '+ ' >';
  block += '<div class="comment_title" id="anno_' + myid + '"><span class="comment_showhide">+ </span>' + title + '</div>';
  block += '<div class="comment_text">' + '<div class="comment_type">' + annoType + '</div><div class="comment_content">' + txt + '</div>' + entity_link + '</div>';
  block += '</div>';
  
  selectBlock = "#islandora_annoType_content_" + fixed_annotype;
  if($(selectBlock).append(block)) {
  }
  $('#anno_' + myid).attr('canvas', canvasId);

  $('#delete_anno_'+myid).click(function(e){
    if (confirm("Permananently Delete This Annotation?")) {
      islandora_deleteAnno(myid);
    }
    e.preventDefault();
  });

  $('#anno_' + myid).click(function() {
    $(this).toggleClass('annotation-opened').next().toggle();
    var pm = $(this).find('.comment_showhide');
    if (pm.text() == '+ ') {
      pm.empty().append('- ');
      var id = $(this).attr('id').substring(5,100);
      var canvas = $(this).attr('canvas');
      paint_commentAnnoTargets(this, canvas, id, annoType);
    } else {
      $('.svg_' + myid).remove();
      var c = $(this).find('.mycolor');
      svgAreaColors.push(c.attr('color'));
      c.remove();
      pm.empty().append('+ ');
    }
    return false;
  }).next().hide();

  $('#comment_annos').show();

}


var svgAreaColors = ['#FF0000', '#FF6600', '#FF9400', '#FEC500', '#FFFF00', '#8CC700', '#0FAD00', '#00A3C7', '#0064B5', '#0010A5', '#6300A5', '#C5007C']
function get_random_color() {
  return svgAreaColors[Math.floor(Math.random()*svgAreaColors.length)];
}

function paint_commentAnnoTargets(ttldiv, canvasId, annoId, annoType) {
  var col;
  var canvas = $('#' + canvasId).attr('canvas');
  var annos = topinfo['annotations']['comment'][canvas];
  for (var a = 0, anno; anno = annos[a]; a++) {
    if (anno.id == 'urn:uuid:' + annoId) {
      // Paint it up
      if (!svgAreaColors) {
        col = '#a0f060';
      } else {
        col = get_random_color();
      }
      if(islandora_canvas_params.islandora_anno_stroke_width != null) {
        strokeWidth = islandora_canvas_params.islandora_anno_stroke_width;
      } else if(islandora_canvas_params.strokeWidth['urn:uuid:' + annoId] != ''){
        strokeWidth = islandora_canvas_params.strokeWidth[['urn:uuid:' + annoId]];
      } else {
        strokeWidth = $('#stroke_width').val();
      }
      if(Drupal.settings.islandora_image_annotation.can_choose == 1) {
        col = get_random_color();
      }
      if(typeof islandora_canvas_params.mappings['urn:uuid:' + annoId] != 'undefined') {
        if(islandora_canvas_params.mappings['urn:uuid:' + annoId] != ''){
          col = islandora_canvas_params.mappings[['urn:uuid:' + annoId]];
        }
      }
      // Fix for google chrome not getting the color.
      if(typeof col == 'undefined'){
        col = $('#anno_color').attr('value');
      }
      for (var t = 0, tgt; tgt = anno.targets[t]; t++) {
        if (tgt.partOf != null) {
          if (tgt.constraint != null) {
            var str = $(anno.targets[t].constraint.value);
            // New, grabbing the colour for each annotation, not just the
            // first one.
            if(ttldiv != null || ttldiv != "") {
              $(ttldiv).append('<span color="' + str.attr('stroke') + '" class="mycolor" style="margin-top: 2px; background: '+str.attr('stroke')+';float:right;width:15px;height:15px;">&nbsp;</span>');
            }
            svgc = mk_raphael('comment', canvas, canvasId);
            // paint SVG
            paint_svgArea(svgc, anno.id.substring(9, 100), str.attr('stroke'), tgt.constraint.value, str.attr('stroke-width'));
          } else if (tgt.fragmentInfo == 'rect') {
            // paint html box
          }
        }
      }
      break;
    }
  }
}

function paint_svgArea(svgc, annoId, col, svg, strokeWidth) {
  var pth = $.parseXML(svg)
  var doc = $(pth)
  var pthelm = doc.children()[0];
  // Duplicating into the DOM for webkit
  var npth = document.createElementNS(SVG_NS, pthelm.nodeName)
  for (var a in pthelm.attributes) {
    var attr = pthelm.attributes[a];
    npth.setAttribute(attr.nodeName, attr.nodeValue);
  }
  pthelm = npth;
  //changed by UPEI we override the style so after editing we don't have to 
  //reload the anno from fedora.  this should be addressed when saving the anno
  pthelm.setAttribute('style', 'fill:none;opacity:none;stroke:'+col+
      ';stroke-width:'+strokeWidth+'%');
  //pthelm.setAttribute('style', 'fill:none;opacity:none;stroke:'+col);
  pthelm.setAttribute('class', 'svg_' + annoId);
  svgc.canvas.appendChild(pthelm);
}

// UI Callbacks:  Navigation

function reorder_layers(e) {
  // re style the items
  $(".menu_body li:odd").removeClass("alt");
  $(".menu_body li:even").addClass("alt");

  ids = [];
  $('#show_body').children().each(function() {
    ids.push($(this).attr('id'))
  });
  // This is the overall order in which annotations should be ordered
  // li_audio, li_comment, ...  first is topmost in list

  z = 1
  for (var x=ids.length, id; id=ids[x-1];x--) {
    type = id.substring(3,100);
    if (type == 'detailImg') {
      type = 'detailImage'
    } else if (type == 'baseImg') {
      type = 'image'
    }
    topinfo['zOrders'][type] = z;
    z += 1000;
  }
  // Redo display
  showPages();
}

// UI handlers
function nextPage(evt) {
  idx = topinfo['current'] + topinfo['numCanvases'];
  jumpPage(idx);
}

function prevPage(evt) {
  idx = topinfo['current'] - topinfo['numCanvases'];
  jumpPage(idx);
}

function jumpPage(n) {
  topinfo['current'] = n;
  showPages();
}

function repositionCanvases() {
  nCanvas = topinfo['numCanvases']
  rows = Math.floor(Math.sqrt(nCanvas));
  perrow = Math.ceil(nCanvas/rows);

  for (var x=0;x<nCanvas;x++) {
    if (x != 0) {
      if (x % perrow == 0) {
        // below previous first in row
        $('#canvas_'+x).position({
          'of':'#canvas_' + (x-perrow),
          'my':'left top',
          'at':'left bottom',
          'collision':'none',
          'offset': '0 10'
        });
      } else {
        $('#canvas_' +x).position({
          'of':'#canvas_' + (x-1),
          'my':'left top',
          'at':'right top',
          'collision':'none',
          'offset': '10 0'
        });
      }
    }
  }
}
