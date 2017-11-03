

// XXX Need author info in annotations
// Plus other XXXs (!)

// Raphael overwrites CSS with defaults :(


var outsideStyle = islandora_getOutsideStyle()

var insideStyle = {
    fill: '#FFFFFF',
    opacity: 0.3,
    'stroke-width': 'none',
    stroke: 'black',
    'stroke-dasharray': '- '
};

function fetch_comment_annotations() {
    islandora_getList();
	
}

function maybe_config_create_annotation() {
    $('#create_annotation').click(startAnnotating);
    $('#cancelAnno').click(closeAndEndAnnotating);
    $('#saveAnno').click(saveAndEndAnnotating);

    $('.annoShape').click(function() {
        var typ = $(this).attr('id').substr(10,5);
        topinfo['svgAnnoShape'] = typ;
        $('.annoShape').css('border', '0px');
        $(this).css('border', '1px solid black');
    });
	
    var shp = $('.annoShape').filter(':first');
    shp.css('border', '1px solid black');
    if(shp != null) {
      topinfo['svgAnnoShape'] = shp.attr('id').substr(10,5);
    }
}

function startAnnotating() {
    $('#anno_color_activated').attr('value', '');
    if ($('#create_annotation').text() == 'Annotating') {
        return;
    }
    $('#saveAnno').html('<span class="ui-button-text">Save</span>');
    $('#create_annotation').css({
        color:'#808080'
    });
    $('#create_annotation').empty().append('Annotating');
    $('#create_annotation_box').dialog('open');
    $('.ui-widget-overlay').remove();
    $('#canvases .canvas').each(function() {
        var cnv = $(this).attr('canvas');
        initForCreate(cnv);
    });
}

function startEditting(title, annotation, annoType, urn) {
    $('#anno_color_activated').attr('value', '');
    if ($('#create_annotation').text() == 'Annotating') {
      return;
    }

    $('#create_annotation').css({
        color:'#808080'
    });
    $('#create_annotation').empty().append('Annotating');
    $('#create_annotation_box').dialog('open');
    $('.ui-widget-overlay').remove();

    $('#anno_title').val(title);
    $('#anno_text').val(annotation);
    $('#anno_classification').val(annoType);
    $('#saveAnno').html('<span class="ui-button-text">Update Annotation</span>');
    $('#saveAnno').attr('urn', urn);
    $('#canvases .canvas').each(function() {
        var cnv = $(this).attr('canvas');
        initForCreate(cnv);
    });
}
function saveAndEndAnnotating() {
    var okay = saveAnnotation();
    if (okay) {
        closeAndEndAnnotating();
        // After rebuild, resize.
        resizeCanvas();
    }
    return okay;
}

function closeAndEndAnnotating() {

    $('#create_annotation').empty().append('Annotate');
    $('#create_annotation').css({
        color:'#000000'
    });
    $('#canvases .canvas').each(function() {
        cnv = $(this).attr('canvas');
        destroyAll(cnv);
    });
    //destroy or hide all annos both render and block represtentations
    $('.comment_text').each(function(i, el) {                        
                        $(el).hide();                         
                    });
    $('.comment_title').each(function(i, el) {                        
                        $(el).toggleClass('annotation-opened');                         
                    });
    $('.comment_showhide').each(function(i, el) {  
                       $(el).text('+ ');                         
                    });  
    $('.mycolor').each(function(i, el) {                        
                        $(el).hide();                         
                    }); 
    $('#create_annotation_box').dialog('close');

    // empty fields
    $('#anno_title').val('');
    $('#anno_text').val('');
    $('#anno_aboutCanvas').prop('checked', false);
    $('#anno_isResource').prop('checked', false);
    var tabs = $('#tabs').tabs();
    tabs.tabs('select', 3);

    
}
	
//We do creation by trapping clicks in an invisible SVG box
//This way we have the dimensions without messing around 
//converting between page clicks and canvas clicks

function initForCreate(canvas) {
    var r = mk_raphael('comment', canvas, topinfo['canvasDivHash'][canvas]);
    var invScale = 1.0 / r.newScale;
    var ch = Math.floor(r.height * invScale);
    var cw = Math.floor(r.width * invScale);
    var prt = r.wrapperElem;
    // Ensure we're above all painting annos
    $(prt).css('z-index', 1001);
    var bg = r.rect(0,0,cw,ch);
    bg.attr({
        'fill': 'white',
        'opacity': 0.15
    });
    bg.creating = null;
    bg.invScale = invScale;
    bg.myPaper = r;
    bg.myShapes = [];
    r.annotateRect = bg;
    bg.drag(function(dx,dy) {
        this.creating.resizeFn(dx, dy);
    }, switchDown, switchUp);
}

function destroyAll(canvas) {
    if ( topinfo['raphaels']['comment'][canvas]){
        var r = topinfo['raphaels']['comment'][canvas];
        var bg = r.annotateRect;
        if(bg){
            for (var x in bg.myShapes) {
                var sh = bg.myShapes[x];
                if (sh.set != undefined) {
                    sh.set.remove();
                } else {
                    sh.remove();
                }
            };
            bg.remove();
        }
        $(r.wrapperElem).remove();
        $(r).remove();
    }
    topinfo['raphaels']['comment'][canvas] = undefined;
}

function saveAnnotation() {
    // Basic Sanity Check
    var title = $('#anno_title').val();
    if($("#anno_title").get(0).tagName == 'SELECT'){
        //if title is a select box we want the title not the pid
        title = $('#anno_title option:selected').text();
    }
    var content = $('#anno_text').val();
    var annoType = $('#anno_classification').val();
    var color = '';

    //check to see if color box has been activated
    color = $('#anno_color').attr('value');
    if($('#hidden_annotation_type').attr('anno_type') == 'Update Annotation'){
        urn = $('#hidden_annotation_type').attr('urn');
        islandora_updateAnno(urn, title, annoType, content, color);
        return;
    }
    if (!content || (!title && typ == 'comment')) {
        alert('An annotation needs both title and content');
        return 0;
    } 
	
    // Create
    var rinfo = create_rdfAnno();
    var rdfa = rinfo[0];
    var tgt = rinfo[1];
    if (tgt == null) {
        alert('You must draw a shape around the target.');
        return 0;
    }

    // Save
    var data = $(rdfa).rdf().databank.dump({
        format:'text/turtle',
        serialize:true
    });
    var type = $('#anno_classification').val();
    // add category to annoblock before saving annotation.  Fixes concurrency errors
    if(type == '') {
      type = 'unclassified';
    }
    var fixed_cat = type.replace(/[^\w]/g,'');

    var type_class = "annoType_" + fixed_cat;
    var blockId = 'islandora_annoType_'+ fixed_cat;
    var contentId = 'islandora_annoType_content_'+ fixed_cat;
    
    var dt = data.split("<urn:uuid:");
    var split_dt = dt[1].split('>');
    var idSelector = '#' + blockId;
    
    if($(idSelector).length == 0){
        header =  '<div class = "islandora_comment_type" id = "'+ blockId + '">';
        header += '<div class = "islandora_comment_type_title">' + type + '</div>';
        header += '<div class = "islandora_comment_type_content" style = "display:none" id = "'+ contentId + '"></div>';
        header += '</div>';
        $('#comment_annos_block').append(header);
    }
    // add new categories to typeahead if necessary
    if($.inArray(type, islandora_canvas_params.categories) == -1){
        islandora_canvas_params.categories.push(type);
        $( "#anno_classification" ).autocomplete({
            source: islandora_canvas_params.categories
        });
    }

    // Updated backport fix. Add's click handler to new annotations,
    // Making them available.
    islandora_postData(tgt, rdfa, type, color);
    $(".islandora_comment_type_title").off();
    $(".islandora_comment_type_title").ready().on("click", function(){
      $(this).siblings('.islandora_comment_type_content').toggle();
    });
    
    var data = {};
	data.Target = tgt;
	data.Type = type;
	data.Colour = color;
	data.uuid = split_dt[0];
    return data;
}

function nodeToXml(what) {
    // MUST use 's as in attribute on span
    var xml = '<svg:' + what.nodeName + " xmlns:svg='" + SVG_NS +"' ";
    for (a in what.attributes) {
        var attr = what.attributes[a];
        if (attr.nodeName != undefined) {
            xml += (attr.nodeName + "='"+attr.nodeValue+"' ");
        }
    }
    xml += ("></svg:" + what.nodeName+'>');
    return xml;
}

function create_rdfAnno() {

    var nss = opts.namespaces;
    var typ = null;
    $('#anno_type :selected').each(function() {
        typ = this.value;

    });

    var clss = 'oac:Annotation';
    var fullclss = nss['oa'] +'Annotation';
    var bodyClass = null;
    var bodyFullClass = null;

    var clss = 'oac:Annotation';
    var fullclss = nss['oa'] +'Annotation';
    var now = isodate(new Date());
    // Generate namespaces from RDF options
    var xmlns = '';
    for (x in nss) {
        xmlns += ('xmlns:'+x+'="'+nss[x]+'" ')
    }

    var rdfa = '<div '+xmlns+'>'; // Start wrapper for XMLNS

    // Build Annotation
    var annoUU = new UUID();
    rdfa += '<div about="urn:uuid:' + annoUU + '"> '; // Start Anno
    rdfa += ('<a rel="rdf:type" href="'+fullclss+'"></a>');
    rdfa += '<span property="dcterms:created" content="' + now + '"></span> ';
    var title = $('#anno_title').val();
    if($("#anno_title").get(0).tagName == 'SELECT'){
        //if title is a select box we want the title not the pid
        title = $('#anno_title option:selected').text();
    }
    var color = $('#anno_color').attr('value');
    
    // Check to be sure that entity linking is enabled.
    if(Drupal.settings.islandora_image_annotation.allow_entity_linking) {
      // Parse out the entity pid from the inner anchor.
      var image_entity = $('#hidden_entity').data('entity');
      if(image_entity) {
        var wrapper= document.createElement('a');
        wrapper.innerHTML= image_entity.data;
        var div= wrapper.firstChild;
        var image_entity_id = div.innerHTML;
          if(image_entity_id != null) {
          rdfa += '<span property="dcterms:relation" content="' + image_entity_id + '"></span>';
          rdfa += '<span property="dcterms:hasPart" content="' + image_entity.label + '"></span>';
        }
      }
    }
    if (title != '') {
        rdfa += '<span property="dc:title" content="' + title + '"></span>';
    }
    
    var type = $('#anno_classification').val();
    if(type == ''){
        type = 'unclassified'
    }
    if (title != '') {
        rdfa += '<span property="dc:type" content="' + type + '"></span>';
    }
   
//    try {
//        // XXX Gdata specific, but can send to other services
//        var which = $('#create_body input[name="blog_radio"]:radio:checked').attr('id');
//        var bFO = topinfo['blogs'][which][2];
//        var authors = bFO.author;
//        for (var a=0, auth; auth=authors[a]; a++) {
//            var email = auth.email.getValue(); // check for noreply@blogger.com
//            var name = auth.name.getValue();
//            var uri = auth.uri.getValue();
//            rdfa += '<a rel="dcterms:creator" href="' + uri + '"></a> ';
//            rdfa += '<div about="' + uri + '"> ';
//            rdfa += '<a rel="rdf:type" href="http://xmlns.com/foaf/0.1/Agent"></a>';
//            rdfa += '<span property="foaf:name" content="' + name + '"></span> ';
//            if (email != 'noreply@blogger.com') {
//                rdfa += '<span property="foaf:mbox" content="' + email + '"></span> ';
//            }
//            rdfa += '</div> '; // Close Creator
//        }
//	
//        var rights = bFO.getRights();
//        if (rights != undefined) {
//            var rtxt = rights.getText();
//            var ruri = rights.getUri();
//            if (rtxt) {
//                rdfa += '<span property="dc:rights" content="' + rtxt + '"></span> ';
//            } else {
//                rdfa += '<a rel="dcterms:rights" href="' + ruri + '"></a> ';
//            }
//        }
//    } catch (e) {};
    
    // Build Body
    var isResc = $('#anno_isResource').prop('checked');
    var tgtsCanvas = $('#anno_aboutCanvas').prop('checked');
    var content = $('#anno_text').val();
    
    if (isResc == true) {
        // XXX Could be constrained resource, eg part of an XML or image
        // So would need to build constraint ... too hard!
        rdfa += '<b>See:</b> <a rel="oa:hasBody" href="' + content + '">' + content + '</a>';
    } else {
        var contUU = new UUID();
        rdfa += '<a rel="oa:hasBody" href="urn:uuid:' + contUU +'"></a> ';
        rdfa += '<div about="urn:uuid:' + contUU + '"> ';
        rdfa += '<a rel="rdf:type" href="http://www.w3.org/2008/content#ContentAsText"></a> ';
        if (bodyFullClass != null) {
            rdfa += '<a rel="rdf:type" href="'+bodyFullClass+'"></a>';
        }
        rdfa += '<span property="cnt:chars">' + content + '</span> ';
        rdfa += '<span property="cnt:characterEncoding" content="utf-8"></span>';
        rdfa += '</div> ';  // Close Body
    }
    
    // Build Target(s)
    // For each Canvas, for each SVG, build a ConstrainedTarget and SvgConstraint
    // XXX Bad assumption that about ALL canvases if tgtsCanvas
    // XXX Should allow selection of other Annotations (eg Texts)
    
    var target = null;
    $('#canvases .canvas').each(function() {
        var cnv = $(this).attr('canvas');
        if(cnv){
            if (tgtsCanvas == true) {
                target = cnv;
                rdfa += '<a rel="oa:hasTarget" href="' + cnv +'"></a>';
            } else {
                var r = topinfo['raphaels']['comment'][cnv];
                var bg = r.annotateRect;
                var stuff = bg.myShapes;
                for (s in stuff) {
                    target = cnv;
                    var svgxml = nodeToXml(stuff[s].node);
                    svgxml = svgxml.replace("stroke='#000000'" , "stroke='" + color +  "'")
                    svgxml = svgxml.replace('<', '&lt;');
                    svgxml = svgxml.replace('<', '&lt;');
                    svgxml = svgxml.replace('>', '&gt;');
                    svgxml = svgxml.replace('>', '&gt;');
                    var ctuu = new UUID();
                    rdfa += '<a rel="oa:hasTarget" href="urn:uuid:' + ctuu +'"></a>';
                    rdfa += '<div about="urn:uuid:' + ctuu +'">';
                    rdfa += '<a rel="rdf:type" href="http://www.w3.org/ns/openannotation/core/ConstrainedTarget"></a>';
                    rdfa += '<a rel="oa:constrains" href="' + cnv + '"></a>';
                    var svguu = new UUID();
                    rdfa += '<a rel="oa:constrainedBy" href="urn:uuid:' + svguu + '"></a>'
                    rdfa += '<div about="urn:uuid:' + svguu + '">';
                    rdfa += '<a rel="rdf:type" href="http://www.w3.org/ns/openannotation/core/SvgConstraint"></a>';
                    rdfa += '<a rel="rdf:type" href="http://www.w3.org/2008/content#ContentAsText"></a>';
                    rdfa += '<span property="cnt:chars" content="' + svgxml + '"></span>';
                    rdfa += '<span property="cnt:characterEncoding" content="utf-8"></span>';
                    rdfa += "</div>"; // Close Constraint
                    rdfa += "</div>"; // Close Constrained Target
                }
            }
        }
    });
    rdfa += "</div>"; // Close Annotation
    rdfa += "</div>"; // Close wrapper
    return [rdfa, target, color];
}

function switchDown(x,y) {
    var fixedxy = fixXY(this,x,y);
    var x = fixedxy[0];
    var y = fixedxy[1];
    var which = topinfo['svgAnnoShape'];
	
    if (which == 'circ') {
        this.creating = mkCircle(this,x,y);
        this.myShapes.push(this.creating);
    } else if (which == 'rect') {
        this.creating = mkRect(this, x,y);
        this.myShapes.push(this.creating);
    } else {
        if (this.creating == null) {
            this.creating = mkPoly(this,x,y);
            this.myShapes.push(this.creating);
        } else {
            this.creating.addPoint(this,x,y);
        }
    }
}

function switchUp(x, y) {
    var which = topinfo['svgAnnoShape'];
    if (which == 'circ') {
        this.creating.start=[];
        this.creating = null;
    } else if (which == 'rect') {
        this.creating.set.start=[];
        this.creating = null;
    }
}

function fixXY(what, x, y) {
    // modify for x,y of wrapper
    var r = what.myPaper;
    var wrap = r.wrapperElem;

    // This is location of canvas
    var offsetLeft = $(wrap).offset().left;
    var offsetTop = $(wrap).offset().top;
    y-= offsetTop;
    x -= offsetLeft;
	
    // Change made to support embeding shared canvas.
    var pageOffsetTop = $(window).scrollTop();
    var pageOffsetLeft = $(window).scrollLeft();
    y += pageOffsetTop;
    x += pageOffsetLeft;
	
    // And now scale for Canvas resizing
    x = Math.floor(x * what.invScale);
    y = Math.floor(y * what.invScale);
    return [x,y]
}



function mkGrabber(what,poly,x,y,idx) {
    var myr = Math.floor(10*what.invScale);
    var r = what.myPaper;
    var c = r.circle(x,y,myr);
    c.attr(insideStyle);
    c.pointIdx = idx;
    c.poly = poly;
    poly.set.push(c);
    c.start = [x,y];
	
    var mdf = function() {
        this.moved = 0;
        this.start = [this.attr("cx"), this.attr("cy")]
    };

    var muf = function() {
        if (what.creating == this.poly && this.pointIdx == 0) {
            what.creating = null;
            this.poly.attr('path', this.poly.attr('path') + 'Z');
        } else if (!this.moved) {
            // delete point
            pth = Raphael.parsePathString(this.poly.attr("path"));
            pth.splice(this.pointIdx, 1);
            this.poly.attr("path", pth);
            // Now shuffle down all subsequent points
            for (var i = this.pointIdx, pt; pt = this.poly.set[i+1]; i++) {
                pt.pointIdx -= 1;
            }
            this.remove();
        }
        this.start = undefined;
    };

    var move = function (dx, dy) {
        dx = Math.floor(dx * what.invScale);
        dy = Math.floor(dy * what.invScale);
		
        this.attr({
            cx: this.start[0] + dx,
            cy: this.start[1] + dy
        })
        var pathsplit = Raphael.parsePathString(this.poly.attr("path"))
        pathsplit[this.pointIdx][1] = Math.floor(this.start[0]+dx);
        pathsplit[this.pointIdx][2] = Math.floor(this.start[1]+dy);

        this.poly.attr('path', pathsplit);
        this.moved = 1;
    };
    c.moveFn = move;
    c.drag(move, mdf, muf);
    return c;
}

function mkPoly(what, x,y) {

    addPointFn = function(what, x,y) {
        this.attr('path', this.attr('path') + ('L'+x+','+y) );
        c = mkGrabber(what, this, x,y,this.attr('path').length-1);
    };

    var mdf = function() {
        this.set.tmp = [0,0];
    };
    var muf = function() {
        this.set.tmp = undefined;
    };
    var move = function(dx,dy) {
        dx=Math.floor(dx * what.invScale);
        dy=Math.floor(dy * what.invScale);
        this.set.translate(dx-this.set.tmp[0], dy-this.set.tmp[1]);
        this.set.tmp = [dx,dy];
    };
    var resizefn = function(dx,dy) {
        dx=Math.floor(dx * what.invScale);
        dy=Math.floor(dy * what.invScale);
        c = this.set[this.set.length-1];
        c.moveFn(dx,dy);
    };
	
    var r = what.myPaper;
    var s = r.set();
    var outer = r.path("M" +x + ',' + y);
    var outsideStyle = islandora_getOutsideStyle();
    outer.attr(outsideStyle);
    outer.attr()
    outer.addPoint = addPointFn;
    s.push(outer)
    outer.set = s;
	
    c = mkGrabber(what,outer,x,y,0);
    outer.drag(move, mdf, muf);
    outer.resizeFn = resizefn;
    outer.dblclick(function() {
        this.set.remove();
    });
    return outer;
}


function mkCircle(what, x,y) {

    innerSize = Math.floor(10 * what.invScale);
	            	
    mdf = function() {
        this.start = [this.attr("cx"), this.attr("cy"), this.attr('r')]
    };
	
    muf = function() {
        this.start = undefined;
    };
	
    move = function (dx, dy) {
        this.set.attr(
        {
            cx: this.start[0] + Math.floor(dx * what.invScale),
            cy: this.start[1] + Math.floor(dy * what.invScale)
        });
    };
	
    resize = function(dx, dy) {
        this.attr('r', this.start[2] + Math.floor(dx * what.invScale));
        this.inner.attr('r', this.start[2] + (Math.floor(dx * what.invScale) - innerSize));
    };
	
    var r = what.myPaper;
    var st = r.set();
    var outer = r.circle(x,y,innerSize);
    var outsideStyle = islandora_getOutsideStyle();
    outer.attr(outsideStyle);
    outer.start = [x,y,innerSize];
    outer.set = st;
	
    var inner = r.circle(x, y, 0);
    inner.attr(insideStyle);
    inner.toFront();
    inner.set = st;
	
    st.push(outer);
    st.push(inner);
    outer.inner = inner;
	
    inner.drag(move, mdf, muf);
    outer.drag(resize, mdf, muf);
    outer.resizeFn = resize;
    inner.dblclick(function() {
        this.set.remove();
    });
    return outer;
}


function mkRect(what, x,y) {
	            
    innerSize = Math.floor(14 * what.invScale);
	
    mdf = function() {
        this.set.start = [
        Math.floor(this.set.outer.attr('x') ),
        Math.floor(this.set.outer.attr('y') ),
        Math.floor(this.set.outer.attr('height') ),
        Math.floor(this.set.outer.attr('width') )
        ];
    };
	
    muf = function() {
        this.set.start = undefined;
    };
	
    move = function(dx, dy) {
        this.set.outer.attr(
        {
            'x': this.set.start[0] + Math.floor(dx * what.invScale),
            'y' : this.set.start[1] + Math.floor(dy * what.invScale)
        });
        this.set.inner.attr(
        {
            'x': this.set.start[0] + this.set.start[3] + Math.floor(dx * what.invScale) - innerSize,
            'y' : this.set.start[1] + this.set.start[2] + Math.floor(dy * what.invScale) - innerSize
        });
    };
							 
    resize = function(dx, dy) {
        this.set.outer.attr(
        {
            'height' : this.set.start[2] + Math.floor(dy * what.invScale),
            'width' : this.set.start[3] + Math.floor(dx * what.invScale)
        });
        this.set.inner.attr(
        {
            'x' : this.set.start[0] + this.set.start[3] + Math.floor(dx * what.invScale) - innerSize,
            'y': this.set.start[1] + this.set.start[2] + Math.floor(dy * what.invScale) - innerSize
        });
    };
	
    var r = what.myPaper;
    var st = r.set();
    st.start = [x, y, innerSize,innerSize];

    var outer = r.rect(x,y, innerSize,innerSize);
    var outsideStyle = islandora_getOutsideStyle();
    outer.attr(outsideStyle);
    outer.set = st;
    var inner = r.rect(x,y, innerSize+1,innerSize+1);
    inner.attr(insideStyle);
    inner.toFront();
    inner.set = st;

    st.push(outer);
    st.push(inner);
    st.outer = outer;
    st.inner = inner;
	
    inner.drag(resize, mdf, muf);
    outer.drag(move, mdf, muf);
    outer.resizeFn = resize;
    outer.dblclick(function() {
        this.set.remove();
    });
    return outer;
}

function islandora_getOutsideStyle(){
    var outsideStyle = {
        fill: 'none',
        opacity: 'none',
        'stroke-width': $('#stroke_width').val() + '%' ,
        stroke: $('#anno_color').attr('value')
    };
    return outsideStyle;
}
