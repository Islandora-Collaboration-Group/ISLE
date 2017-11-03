
function getRemForAggr(uri, qry) {
  var rem = '';
  if (uri.toString != undefined) {
    var uri = uri.toString();
  };
	
  // Check for turtle first, as faster (?)
  qry.reset();
  qry
  .where('<'+uri+'> ore:isDescribedBy ?rem')
  .where('?rem dc:format "application/rdf+xml"')
  // .where('?rem dc:format "text/turtle"')
  .each(function() {
    rem=this.rem.value.toString()
  });

  // Then n3. XML parser seems faster now?!
  if (rem == '') {
    qry.reset();
    qry
    .where('<'+uri+'> ore:isDescribedBy ?rem')
    .where('?rem dc:format "application/rdf+xml"')
    //.where('?rem dc:format "text/turtle"')
    .each(function() {
      rem=this.rem.value.toString()
    });
  }
	
  if (rem == '') {
    // Check if # URI for aggregation
    var hshidx = uri.indexOf('#');
    if (hshidx > -1) {
      return uri.substr(0, hshidx)
    } else {
      // Fallback. Assume .xxx convention, based on first in done
      // eg Manifest.xml -> (aggr).xml
      var m = topinfo['done'][0].match(/\.(xml|n3|json)$/)
      if (m) {
        var ext = m[0];
      } else {
        var ext = '.xml';
      }
      return uri + ext;
    }
  } else {
    return rem;
  }
}

function extract_canvas_size(qry, uri) {
  var h = 0;
  var w = 0;
  var t = ''
  qry.where('<' + uri + '> exif:height ?height')
  .where('<' + uri + '> exif:width ?width')
  .optional('<' + uri + '> dc:title ?title')
  .each(function() {
    h = this.height.value;
    w = this.width.value;

    if (this.title != undefined) {
      t = this.title.value;
    }
  });
  
  return [h, w, t]
}

// Top level Manifest of other Aggregations of resources
function cb_process_manifest(qry, uri) {
  //ping_progressBar('manifest');

  // Find Sequences
  var seqs = [];
  // And find all the other types
		
  var textAnnos = {
    '*':[]
  };
  var audioAnnos = {
    '*':[]
  };
  var imgAnnos = {
    '*':[]
  };
  var commentAnnos = {
    '*':[]
  };
  var zoneAnnos = {
    '*':[]
  };
  qry
  .where('<' + uri +'> ore:describes ?aggr')
  .where('?aggr ore:aggregates ?seq')
  .where('?seq rdf:type dms:Sequence')
  .each(function() {
    seqs.push(this.seq.value);
  });
  qry.reset();
  
  qry
  .where('<' + uri +'> ore:describes ?aggr')
  .where('?aggr ore:aggregates ?seq')
  .optional('?seq dms:forCanvas ?canv')
  .where('?seq rdf:type dms:ImageAnnotationList')
  .each(function() {
    
    c = (this.canv == undefined) ? '*' : this.canv.value;
    what = this.seq.value;
    if (c == '*') {
      imgAnnos[c].push(what);
    } else if (imgAnnos[c] == undefined) {
      imgAnnos[c] = [what];
    } else {
      imgAnnos[c].push(what);
    }
  })
      
  .end()
  .where('?seq rdf:type dms:TextAnnotationList')
  .each(function() {
    c = (this.canv == undefined) ? '*' : this.canv.value;
    what = this.seq.value;
    if (c == '*') {
      textAnnos[c].push(what)
    } else if (textAnnos[c] == undefined) {
      textAnnos[c] = [what]
    } else {
      textAnnos[c].push(what)
    }
  })
  .end()
  .where('?seq rdf:type dms:AudioAnnotationList')
  .each(function() {
    c = (this.canv == undefined) ? '*' : this.canv.value;
    what = this.seq.value;
    if (c == '*') {
      audioAnnos[c].push(what)
    } else if (audioAnnos[c] == undefined) {
      audioAnnos[c] = [what]
    } else {
      audioAnnos[c].push(what)
    }
  })
  .end()
  .where('?seq rdf:type dms:ZoneAnnotationList')
  .each(function() {
    c = (this.canv == undefined) ? '*' : this.canv.value;
    what = this.seq.value;
    if (c == '*') {
      zoneAnnos[c].push(what)
    } else if (zoneAnnos[c] == undefined) {
      zoneAnnos[c] = [what]
    } else {
      zoneAnnos[c].push(what)
    }
  })
  .end()
  .where('?seq rdf:type dms:CommentAnnotationList')
  .each(function() {
    c = (this.canv == undefined) ? '*' : this.canv.value;
    what = this.seq.value;
    if (c == '*') {
      commentAnnos[c].push(what)
    } else if (commentAnnos[c] == undefined) {
      commentAnnos[c] = [what]
    } else {
      commentAnnos[c].push(what)
    }
  })
  qry.reset();

  var ranges = []
  qry.where('?seq dcterms:hasPart ?rng')
  .where('?rng a dms:Range')
  .where('?rng dc:title ?title')
  .where('?rng rdf:first ?f')
  .each(function() {
    ranges.push([this.title.value, this.f.value])
  });
  topinfo['ranges'] = ranges
  topinfo['lists']['text'] = textAnnos;
  topinfo['lists']['image'] = imgAnnos;
  topinfo['lists']['audio'] = audioAnnos;
  topinfo['lists']['comment'] = commentAnnos;
  topinfo['lists']['zone'] = zoneAnnos;
  if (seqs.length == 1) {
    var uri2 = getRemForAggr(seqs[0], qry)
    ping_progressBar('manifestDone')
    fetchTriples(uri2, qry, cb_process_sequence);
  } else {
    // Present list of possible sequences to user
    ping_progressBar('manifestDone')
    var st = []
    qry.where('?seq a dms:Sequence')
    .optional('?seq dc:title ?title')
    .optional('?seq dc:description ?desc')
    .each(function() {
      var ttl = this.title != undefined ? this.title.value : '';
      var desc = this.desc != undefined ? this.desc.value : '';
      st.push([this.seq.value, ttl, desc])
    });
    show_seqSel(st);
  }
        
}

function clickThruSequence(seq) {
	
  var qry = topinfo['query'];
  var uri2 = getRemForAggr(seq, qry)
  $('#seqSel').hide();
  fetchTriples(uri2, qry, cb_process_sequence);
	
}


// Given a Sequence, extract list of Canvases
function cb_process_sequence(qry, uri) {
  var top = null;
  var l = [];

  ping_progressBar('sequence');
	
  try {
    qry.where('?seq rdf:type dms:Sequence')
    .where('?seq ore:isDescribedBy <'+uri+'>')
    .each(function() {
      top = this.seq.value;
    });
	
    if (top == null) {
      // Just pick one!
      qry.where('?seq rdf:type dms:Sequence')
      .each(function() {
        top = this.seq.value;
      });
    }
  } catch (e) {
    alert(e)
  }
	
  if (top != null) {
    l = rdf_to_list(qry, top);
    topinfo['sequence'] = l;
  } else {
    // broken
    alert('Could not find the sequence :(')
    return;
  }
	
  populate_jumpMenu(qry);
  // Initialize based on URI params or first folio of sequence
  if (topinfo['uriParams']['s'] != undefined) {
    topinfo['current'] = topinfo['uriParams']['s'];
  } else {
    topinfo['current'] = 0;
  }
  ping_progressBar('sequenceDone');
  showPages();
}


function fetch_annotations(which, canvas) {
  var tal = topinfo['lists'][which][canvas];
  if (tal == undefined) {
    tal = topinfo['lists'][which]['*'];
  }
  if (tal != undefined && tal.length > 0) {
    // New databank per file, and delete after extracting annos
    var q = $.rdf(opts);
    var topQuery = topinfo['query'];
    for (u in tal) {
      var uri = tal[u];
      var uri2 = getRemForAggr(uri, topQuery);
      for (var x=0,u2;u2=topinfo['done'][x];x++) {
        if (uri2 == u2) {
          paint_annos(which, canvas);
          return;
        }
      }
      ping_progressBar('req:'+uri2);
      fetchTriples(uri2, q, cb_process_annoList);
    }
  }
}

var ancCnt = 0;
function cb_process_annoList(qry, uri) {
  ping_progressBar('recv:'+uri);
  var nss = opts.namespaces;
  var allAnnos = topinfo['annotations'];
  var zorder = {};
  // Walk through aggregation/list of annos
  if (uri != undefined) {
    qry.reset();
    var top = null;
    qry.where('<' + uri + '> ore:describes ?seq')
    .each(function() {
      top = this.seq.value.toString();
    });
    if (top != null) {
      var l = rdf_to_list(qry, top);
      zorder = {};
      for (var x in l) {
        zorder[l[x]] = parseInt(x)+1;
      }
    }
  } else {
	  console.log("uri is not defined");
  }

  var xmlfiles = {};

  try {
    var annos = buildAllAnnos(qry);
  } catch (e) {
    alert('error building annos: ' + e)
  }
	
  for (var a=0,anno;anno=annos[a];a++) {
    // figure out what sort of anno
    var mtyp = '';
    for (var t=0,typ;typ=anno.types[t];t++) {
      // check type
      if (typ == nss['dms']+'ImageAnnotation') {
        mtyp = 'image';
        break;
      } else if (typ == nss['dms']+'TextAnnotation') {
        mtyp = 'text';
        break;
      } else if (typ == nss['dms']+'AudioAnnotation') {
        mtyp = 'audio';
        break;
      } else if (typ == nss['dms']+'CommentAnnotation') {
        mtyp = 'comment';
        break;
      } else if (typ == nss['dms']+'ZoneAnnotation') {
        mtyp = 'zone';
        break;
      } else if (typ == nss['oa']+'Annotation') {
        mtyp = 'comment'; // don't break in case multi typed
      }
    }
    // Check body type
    if (!mtyp) {
      for (var t=0,typ;typ=anno.body.types[t];t++) {
        if (typ == nss['dctype']+'Image') {
          mtyp = 'image';
          break;
        } else if (typ == nss['dctype']+'Text') {
          mtyp = 'text';
          break;
        } else if (typ == nss['dms']+'ImageBody') {
          mtyp = 'image';
          break
        } else if (typ == nss['dms']+'TextBody') {
          mtyp = 'text';
          break;
        } else if (typ == nss['dms']+'ImageSegment') {
          mtyp = 'text';
          break;
        }
      }
    }
    // Check body fragment type
    if (!mtyp) {
      mtyp = anno.body.fragmentType;
    }
		
    if (!mtyp) {
      continue;
    }
		
    try {
		
      if (zorder && zorder[anno.id] != undefined) {
        anno.zOrder = zorder[anno.id];
      }
		
      // Figure out which canvas
      if (anno.targets[0].partOf != null) {
        var cnv = anno.targets[0].partOf.id;
      } else {
        var cnv = anno.targets[0].id;
      }
		
      // type specific features
      // 1) fetch XML docs (eg TEI transcription)
      if (anno.body.fragmentType == 'xml') {
        var pid = anno.body.partOf.id;
        if (xmlfiles[pid]== undefined) {
          xmlfiles[pid] = [];
        }
        // need to fetch XML before painting
        xmlfiles[pid].push([anno, anno.body]);
        anno.finished -= 1;
      }
      // 2) fetch SVG doc if external
      if (anno.body.constraint != null && !anno.body.constraint.value) {
        var pid = anno.body.constraint.id;
        if (xmlfiles[pid]== undefined) {
          xmlfiles[pid] = [];
        }
        // need to fetch XML before painting
        xmlfiles[pid].push([anno, anno.body.constraint]);
        anno.finished -= 1;
      }
				
      if (allAnnos[mtyp][cnv] == undefined) {
        allAnnos[mtyp][cnv] = [];
      }
      allAnnos[mtyp][cnv].push(anno);
		
    } catch(e) {
      alert(e)
    };
  }
  topinfo['annotations'] = allAnnos;
  // Try to force GC on the query
  delete qry.databank;
  qry = null;

  paint_annos();
	
  // And launch AJAX queries for any external XML docs
  for (var uri in xmlfiles) {
    ping_progressBar('req:'+uri)
		
    $.ajax(
    {
      url: uri,
      dataType: "xml",
      success: function(data, status, xhr) {
        ping_progressBar('recv:'+uri)
        try {
          // We have the XML now, so walk through all annos for it
          var remotes = xmlfiles[uri];
          for (var i=0,inf; inf=remotes[i]; i++) {
            var anno = inf[0];
            var what = inf[1];
            if (what.fragmentType == 'xml') {
              var sel = what.fragmentInfo[0];
              var txtsel = what.fragmentInfo[1];
              if (txtsel) {
                var btxt = $(data).find(sel).text().substring(txtsel[0], txtsel[1]);
              } else {
                var btxt = $(data).find(sel); // leave it up to Paint to deal with.
              }
            } else {
              var btxt = data;
            }
            what.value = btxt;
            anno.finished += 1;
          }
          paint_annos()
        } catch(e) {
          alert('Broken XML in ' + anno.id +  ':' + e)
        }
      },
      error:  function(XMLHttpRequest, status, errorThrown) {
        alert('Can not fetch data from ' + uri);
      }
    }
    );
  }
}


function load_commentAnno(data) {
  // RDFA
  var lqry = $(data).rdf();
  if (lqry.databank.size() == 0) {
    // Turtle or RDF/XML
    try {
      lqry = $.rdf(opts).load(data);
    } catch (e) {
     //alert('broken comment annotation: ' + data)
    }
  }
  cb_process_annoList(lqry, '');
}

