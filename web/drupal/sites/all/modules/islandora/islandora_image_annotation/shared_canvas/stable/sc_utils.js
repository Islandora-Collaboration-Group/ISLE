

// mfre.exec(frag) --> [full, x, y, w, h]
var mfRectRe = new RegExp('xywh=([0-9]+),([0-9]+),([0-9]+),([0-9]+)');
// mfNptRe.exect(frag) --> [full, npt, start, end]
var mfNptRe = new RegExp('t=(npt:)?([0-9.:]+)?,([0-9.:]+)?');

var textre = new RegExp('/text\\(\\)$');          
var slashs = new RegExp('^[/]+');
var xptr = new RegExp('^#?xpointer\\((.+)\\)$');
var attrq = new RegExp('\\[([^\\]]+)=([^\\]"]+)\\]', 'g')
var strrng = new RegExp('^string-range\\((.+),([0-9]+),([0-9]+)\\)')


function xptrToJQuery(xp) {
  // Strip xpointer(...)
  var xp = xp.replace(/^\s+|\s+$/g, '');
  var m = xp.match(xptr);
  if (m) {
    xp = m[1];
  }
  // We want to support string-range(xp, start, end)
  xp = xp.replace(/^\s+|\s+$/g, '');
  m = xp.match(strrng);
  if (m) {
    xp = m[1];
    var start = parseInt(m[2]);
    var end = parseInt(m[3]);
    var wantsText = [start, end]
  } else {
    // /text() --> return that we want .text()
    var wantsText = false;
    m = xp.match(textre)
    if (m) {
      xp = xp.replace(textre, '');
      wantsText = true;
    }
  }
  //strip initial slashes
  xp = xp.replace(slashs, '');
  // Parent and Descendant axes
  xp = xp.replace(new RegExp('//', 'g'), ' ');
  xp = xp.replace(new RegExp('/', 'g'), ' > ');
  // Ensure quotes in attribute values
  xp = xp.replace(attrq, '[$1="$2"]');
  // id(bla) --> #bla
  xp = xp.replace(/id\((.+)\)/g, '#$1')
  // Final trim
  xp = xp.replace(/^\s+|\s+$/g, '');
  return [xp, wantsText];
}


function getRect(tgt) {
	
  if (tgt.constraint != null) {
    // Extract from SVG
    var pth = $.parseXML(tgt.constraint.value);
    var doc = $(pth)
    var rect = doc.children()[0];
		
    var tx = rect.getAttribute('x');
    var ty = rect.getAttribute('y');
    var tw = rect.getAttribute('width');
    var th = rect.getAttribute('height');
		
    if (!tx || !tw) {
      return null;
    }
		
  } else if (tgt.fragmentInfo != null && tgt.fragmentInfo.length == 4) {
    var tx = tgt.fragmentInfo[0];
    var ty = tgt.fragmentInfo[1];
    var tw = tgt.fragmentInfo[2];
    var th = tgt.fragmentInfo[3];
  } else {
    return null;
  }
  return [tx,ty,tw,th];
}


// Pull rdf/xml file and parse to triples with rdfQuery
function fetchTriples(uri, qry, fn) {
  
  // Store that we've fetched this URI1
  topinfo['done'].push(uri);
	
  // Check if we need to go through echo service
  if (uri.indexOf('http') == 0 && uri.indexOf('www.shared-canvas.org/') == -1) {
    furi = 'http://www.shared-canvas.org/services/echo/?uri='+uri;
  } else {
    furi = uri;
  }

  $.ajax({
    type:'POST',
    async:false,
    url: uri,
    // + '?' + new Date().toString(),
    // accepts: "application/rdf+xml",
    success: function(data, status, xhr) {
      try {
        var resp = qry.databank.load(data);
      } catch(e) {
        alert('Broken RDF/XML: ' + e)
      }
      if (qry != null) {
        fn(qry, uri);
      }
      return;
    },
    error:  function(XMLHttpRequest, status, errorThrown) {
      alert('Can not fetch any data from ' + uri + ': ' + errorThrown);
    }
  });
}


// Take top of RDF list and convert to javascript array
// Allow callback for per item processing in this loop
function rdf_to_list(qry, uri) {
  // Approach 2:  Get ALL first/rests and then step through by hand
  // in one function. Means fewer queries, = faster
  var l = [];
  var firsts = {};
  var rests = {};
  qry.where('?what rdf:first ?frst')
  .where('?what rdf:rest ?rst')
  .each(function() {
    firsts[this.what.value] = this.frst.value;
    rests[this.what.value] = this.rst.value
  });

  // Start list from first
  l.push(firsts[uri]);
  var nxt = rests[uri];

  // And now iterate through linked list
  while (nxt) {
    if (firsts[nxt] != undefined) {
      l.push(firsts[nxt]);
    }
    nxt = rests[nxt];
  }
  return l;
}


function makeUriHash() {
  var hsh = '!';
  for (k in topinfo['uriParams']) {
    hsh += (k +'='+topinfo['uriParams'][k]);
    hsh += '&';
  }
  return hsh.substr(0,hsh.length-1);
}


function parseNptItem(npt) {
  if (npt.indexOf(':') > -1) {
    // [hh:]mm:ss[.xx]
    var arr = npt.split(':');
    var secs = parseFloat(arr.pop());
    var mins = parseInt(arr.pop());
    if (arr.length > 0) {
      var hrs = parseInt(arr.pop());
    } else {
      var hrs = 0
    };
    return secs + (mins * 60) + (hrs * 3600);
  } else {
    return parseFloat(npt)
  }
}

function isodate(d) {
  var dt = '' + d.getUTCFullYear();
  var m = '' + (d.getUTCMonth() + 1);
  m = m.length == 1 ? '0' + m : m;
  var dy = '' + d.getUTCDate();
  dy = dy.length == 1 ? '0' + dy : dy;
  var hr = '' + d.getUTCHours();
  hr = hr.length == 1 ? '0' + hr : hr;
  var mn = '' + d.getUTCMinutes();
  mn = mn.length == 1 ? '0' + mn : mn;
  var sc = '' + d.getUTCSeconds();
  sc = sc.length == 1 ? '0' + sc : sc;
  return dt + '-' + m + '-' + dy + ' ' + hr + ':' + mn + ':' + sc + ' UTC';
}


function log(txt) {
  var now = new Date();
// $('#log').append('::' + ( (new Date().getTime() - startDate) / 1000 ) + ': ' + txt + '<br/>');
}
