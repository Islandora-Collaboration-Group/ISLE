

function buildAllAnnos(query, type) {
  query.reset();

  var dump = query.databank.dump();
  if (type != undefined) {
    var typres = query.where('?anno a ' + type);
  }
  var annos = {};
  var result = query.where('?anno oa:hasBody ?body')
  .each(function() {
    annos[this.anno.value.toString()]=1;
  });
  query.reset();
  return queryToJson(annos, dump);
}


function uniqueValueList(list) {
  var hash = {};
  for (var i=0,item;item=list[i];i++) {
    hash[item.value] = 1;
  }
  var res = [];
  for (j in hash) {
    res.push(j);
  }
  return res;
}

function queryToJson(annos, dump) {
  var nss = opts.namespaces;
  var annoObjs = [];

  for (var id in annos) {
    if (topinfo['builtAnnos'].indexOf(id) > -1) {
      continue;
    } else {
      topinfo['builtAnnos'].push(id);
    }

    var anno = new jAnno(id);
    anno.extractInfo(dump);
    // Must be exactly one body. Ignore past first
    var bodid = dump[id][nss['oa']+'hasBody'][0]['value'];
    var bod = new jBodyTarget(bodid);
    bod.extractInfo(dump);
    anno.body = bod;
    var tgts = dump[id][nss['oa']+'hasTarget'];
    var uniqtgts = uniqueValueList(tgts);
    for (t in uniqtgts) {
      var tid = uniqtgts[t];
      var tgt = new jBodyTarget(tid);
      tgt.extractInfo(dump);
      anno.targets.push(tgt)
    }
    annoObjs.push(anno);
  }
  return annoObjs;
}


function jAnno(id) {
  this.id = id;
  this.types = [];
  this.creator = null;
  this.title = "";
  this.body = null;
  this.targets = [];
  this.zOrder = 0;
  this.finished = 1;
  this.painted = 0;
  this.annoType = '';
}


jAnno.prototype.extractInfo = function(info) {
  var nss = opts.namespaces;
  var me = info[this.id];
  var typs = me[nss['rdf']+'type'];
  this.types = uniqueValueList(typs);
  if (me[nss['dc']+'title'] != undefined) {
    this.title = me[nss['dc']+'title'][0]['value'];
  }
  if (me[nss['dc']+'type'] != undefined) {
    this.annoType = me[nss['dc']+'type'][0]['value'];
  }
  if (me[nss['dcterms']+'relation'] != undefined) {
    this.relation = me[nss['dcterms']+'relation'][0]['value'];
  }
  if (me[nss['dcterms']+'hasPart'] != undefined) {
    this.hasPart = me[nss['dcterms']+'hasPart'][0]['value'];
  }
}

var extractSimple = function(info) {

  var me = info[this.id];
  if (me == undefined) {
    // No info about resource at all
    return;
  }

  var nss = opts.namespaces;

  if (me[nss['rdf']+'type'] != undefined) {
    var typs = me[nss['rdf']+'type'];
    this.types= uniqueValueList(typs);
  }
  if (me[nss['dc']+'title'] != undefined) {
    this.title = me[nss['dc']+'title'][0]['value'];
  }
  if (me[nss['dcterms']+'relation'] != undefined) {
    this.relation = me[nss['dcterms']+'relation'][0]['value'];
  }
  if (me[nss['dcterms']+'hasPart'] != undefined) {
    this.hasPart = me[nss['dcterms']+'hasPart'][0]['value'];
  }
  if (me[nss['cnt']+'chars'] != undefined) {
    this.value = me[nss['cnt']+'chars'][0]['value'];
  }
  if (me[nss['dc']+'format'] != undefined) {
    this.format = me[nss['dc']+'format'][0]['value'];
  }
  if (me[nss['exif']+'height'] != undefined) {
    this.height = parseInt(me[nss['exif']+'height'][0]['value']);
  }
  if (me[nss['exif']+'width'] != undefined) {
    this.width = parseInt(me[nss['exif']+'width'][0]['value']);
  }
  if (me[nss['dc']+'extent'] != undefined) {
    this.extent = parseInt(me[nss['dc']+'extent'][0]['value']);
  }

  // Regularize
  if (me[nss['dms']+'imageType'] != undefined) {
    this.imageType = me[nss['dms']+'imageType'][0]['value'];
  }
  if (me[nss['dms']+'colorSpace'] != undefined) {
    this.imageType = me[nss['dms']+'colorSpace'][0]['value'];
  }

}


function jBodyTarget(id) {
  this.id = id;
  this.fragmentInfo = null;
  this.fragmentType = "";
  this.types = [];
  this.title = "";
  this.relation = "";
  this.hasPart = "";
  this.creator = null;
  this.value = "";
  this.constraint = null;
  this.partOf = null;
  this.options = [];
  this.defaultOpt = null;
  this.rotation = 0;

  var hidx = id.indexOf('#');
  if (hidx > -1) {
    // Check for fragment and try to parse
    var frag = id.substring(hidx+1, 1000);
    if (frag.substring(0,2) == 'xy') {
      // xywh=  (x,y,w,h)
      var info = mfRectRe.exec(frag)
      this.fragmentInfo = [parseInt(info[1]), parseInt(info[2]), parseInt(info[3]), parseInt(info[4])];
      this.fragmentType = 'rect';
    } else if (frag.substring(0,2) == 'xp') {
      // xpointer = (jquerySelect, textInfo)
      var info = xptrToJQuery(frag);
      this.fragmentType = 'xml';
      this.fragmentInfo = info
    } else if (frag.substring(0,2) == 't=') {
      // t= (start, end)
      var info = mfNptRe.exec(frag);
      this.fragmentInfo = [parseNptItem(info[2]), parseNptItem(info[3])]	;
      this.fragmentType = 'audio';
    }
  }

}

jBodyTarget.prototype.extractSimple = extractSimple;

jBodyTarget.prototype.extractInfo = function(info) {
  var nss = opts.namespaces;
  var me = info[this.id];
  if (me == undefined) {
    return;
  }

  this.extractSimple(info);

  if (me[nss['dms']+'option'] != undefined) {
    var mopts = me[nss['dms']+'option'];
    mopts = uniqueValueList(mopts);
    for (var o=0,mopt;mopt=mopts[o];o++) {
      var opto = new jBodyTarget(mopt);
      opto.extractInfo(info);
      this.options.push(opto);
    }
  }

  if (me[nss['dms']+'default'] != undefined) {
    var pid = me[nss['dms']+'default'][0]['value'];
    var dflt = new jBodyTarget(pid);
    this.defaultOpt = dflt;
    dflt.extractInfo(info);
  }

  if (me[nss['dms']+'rotation'] != undefined) {
    this.rotation = me[nss['dms']+'rotation'][0]['value'];
  }

  if (me[nss['dcterms']+'isPartOf'] != undefined) {
    var pid = me[nss['dcterms']+'isPartOf'][0]['value'];
    var partOf = new jResource(pid);
    this.partOf = partOf;
    partOf.extractInfo(info);

  }

  // Check for constraint
  if (this.partOf == null) {
    if (me[nss['oa']+'constrains'] != undefined) {
      var pid = me[nss['oa']+'constrains'][0]['value'];
      var partOf = new jResource(pid);
      partOf.extractInfo(info);
      this.partOf = partOf;

      var cid = me[nss['oa']+'constrainedBy'][0]['value'];
      var constraint = new jResource(cid);
      constraint.extractInfo(info);
      this.constraint = constraint;
    }
  }

}


function jResource(id) {
  this.id = id;
  this.types = [];
  this.title = "";
  this.creator = null;
  this.value = "";
  this.relation = "";
  this.hasPart = "";

  this.format = "";
  this.height = 0;
  this.width = 0;
  this.extent = 0;
  this.imageType = '';
}

jResource.prototype.extractInfo = extractSimple;


function jAgent(id) {
  this.name = "";
  this.email = "";
  this.web = "";
}

