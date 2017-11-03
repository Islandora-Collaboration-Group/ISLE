var zpr = function(viewFinderId, inputValues) {

  var viewFinder = $('#' + viewFinderId); // viewFinder DOM element
  var imgFrame   = undefined; // imgFrame DOM element
  var imgFrameId = viewFinderId + "-img-frame";
  
  var settings = {
    tileSize: 256,         // dimension for a square tile 
    preloadTilesOffset: 2, // rows/columns of tiles to preload
    marqueeImgSize: 125       // max marquee dimension
  };  
  
  var jp2 = {
    width: 0,
    height: 0,
    levels: undefined,    
    url: '', 
    djatokaURL: "http://african.lanl.gov/adore-djatoka/resolver?url_ver=Z39.88-2004&svc_id=info:lanl-repo/svc/getRegion&svc_val_fmt=info:ofi/fmt:kev:mtx:jpeg2000&svc.format=image/jpeg"
  }
      
  var currentLevel = 1;
  var currentRotation = 0;
  var imgFrameAttrs = { relativeLoc: {}, proportionalWidth: 0, proportionalHeight: 0 };
  var marqueeAttrs = { imgWidth: 0, imgHeight: 0, width: 0, height: 0 }; 


  /* init() function */
  var init = function() {
    // copy data from function arguments
    jp2.url    = inputValues.jp2URL;
    jp2.width  = inputValues.width;
    jp2.height = inputValues.height;    
    settings.marqueeImgSize = inputValues.marqueeImgSize;
    
    if (typeof inputValues.levels !== 'undefined') { 
      jp2.levels  = inputValues.levels;
    } else {
      jp2.levels = getNumLevels();  
    }

    jp2.djatokaURL = jp2.djatokaURL + '&rft_id=' + escape(jp2.url);
    
    viewFinder.addClass('zpr-view-finder');
    
    // add imgFrame
    viewFinder.append($('<div>', { 'id': imgFrameId, 'class': 'zpr-img-frame' }));        
    imgFrame = $('#' + imgFrameId);
    
    currentLevel = getLevelForContainer(viewFinder.width(), viewFinder.height());
    setImgFrameSize(currentLevel);
            
    setupImgFrameDragging();
    storeRelativeLocation();
    addControlElements();
  };

  
  /* add zoom and other controls */
  var addControlElements = function() {
    
    // add zoom/rotate controls
    viewFinder
    .append($('<div>').attr({ 'class': 'zpr-controls' })
      .append($('<img>')
        .attr({ 'id': viewFinderId + '-zoom-in', 'src': 'images/zpr-zoom-in.png' })
        .click(function() { zoom('in'); }))
      .append($('<img>')
        .attr({ 'id': viewFinderId + '-zoom-out', 'src': 'images/zpr-zoom-out.png' })
        .click(function() { zoom('out'); }))
      .append($('<img>')
        .attr({ 'id': viewFinderId + '-rotate-cw', 'src': 'images/zpr-rotate-cw.png' })
        .click(function() { rotate('cw'); }))
      .append($('<img>')
        .attr({ 'id': viewFinderId + '-rotate-ccw', 'src': 'images/zpr-rotate-ccw.png' })
        .click(function() { rotate('ccw'); }))
    );

    if (settings.marqueeImgSize > 50) {
      setupMarquee();
    }
  }  
    
  
  /* get total levels for the jp2 */
  var getNumLevels = function() {
    var longestSide = Math.max(jp2.width, jp2.height);
    var djatokaLimit = 92;
    var level = 0;
    
    while (longestSide > djatokaLimit) {
      level++;
      longestSide = Math.round(longestSide / 2);
    }
        
    return level;
  };

  
  /* set imgFrame dimensions */
  var setImgFrameSize = function(level) {    
    imgFrame.width(getImgDimensionsForLevel(level)[0]);    
    imgFrame.height(getImgDimensionsForLevel(level)[1]);

    imgFrameAttrs.proportionalWidth  = Math.round(jp2.width / Math.pow(2, jp2.levels - currentLevel));
    imgFrameAttrs.proportionalHeight = Math.round(jp2.height / Math.pow(2, jp2.levels - currentLevel));
    
    setMarqueeDimensions();    
    positionImgFrame();   
  };

  
  /* get dimensions for a given level */
  var getImgDimensionsForLevel = function(level) {
    var divisor = Math.pow(2, (jp2.levels - level));
    var height  = Math.round(jp2.height / divisor);    
    var width   = Math.round(jp2.width / divisor);
    
    return ([width, height]);    
  };
  
  
  /* calculate level for a given container */
  var getLevelForContainer = function(ctWidth, ctHeight) {
    var maxJp2Dimension = Math.max(jp2.width, jp2.height);
    var minContainerDimension = ctWidth;
    
    if (typeof ctHeight !== 'undefined') {
      minContainerDimension = Math.min(ctWidth, ctHeight);
    }
        
    for (var level = jp2.levels; level >= 0; level--) {
      if (minContainerDimension >= maxJp2Dimension) {
        return level;
      }    
      
      maxJp2Dimension = Math.round(maxJp2Dimension / 2);
    }
    
    return 0;  
  };
  
  
  /* position imgFrame */
  var positionImgFrame = function() {
    var left = 0;
    var top = 10;
    
    if (imgFrame.width() < viewFinder.width()) {     
      left = Math.floor((viewFinder.width() / 2) - (imgFrame.width() / 2));
      
      if (imgFrame.height() < viewFinder.height()) {
        top = Math.floor((viewFinder.height() / 2) - (imgFrame.height() / 2));
      }
    } 

    // if relative location is defined, use it to position imgFrame
    if (typeof imgFrameAttrs.relativeLoc.x !== 'undefined' && 
        typeof imgFrameAttrs.relativeLoc.y !== 'undefined') {
    
       left = Math.round(viewFinder.width() / 2) - Math.ceil(imgFrameAttrs.relativeLoc.x * imgFrame.width());        
       top = Math.round(viewFinder.height() / 2) - Math.ceil(imgFrameAttrs.relativeLoc.y * imgFrame.height());        
    }    

    imgFrame.css({ 'top': top + 'px', 'left': left + 'px' });
    showTiles(); 
  };
  
  
  /* get list of visible tiles */
  var getVisibleTiles = function() {
    
    var visibleImgSpace = { left: 0, top: 0, right: 0, bottom: 0 };
    var visibleTileIds  = { leftmost: 0, topmost: 0, rightmost: 0, bottommost: 0 };
    var numVisibleTiles = { x: 0, y: 0 };
    var totalTiles      = { x: 0, y: 0 };

    var visibleTileArray = [];
    var ctr = 0;
    var tileSize = settings.tileSize;
    
    // total available tiles for imgFrame 
    totalTiles.x = Math.ceil(imgFrame.width() / tileSize);
    totalTiles.y = Math.ceil(imgFrame.height() / tileSize);
    
    // calculate visibleImgSpace location
    if (imgFrame.position().left > 0) {
      visibleImgSpace.left = imgFrame.position().left;
    }
    
    if (imgFrame.position().top > 0) {
      visibleImgSpace.top = imgFrame.position().top;
    }
    
    visibleImgSpace.right  = imgFrame.position().left + imgFrame.width();
    visibleImgSpace.bottom = imgFrame.position().left + imgFrame.height();
    
    if (visibleImgSpace.right > viewFinder.width()) {
      visibleImgSpace.right = viewFinder.width();
    }
    
    if (visibleImgSpace.bottom > viewFinder.height()) {
      visibleImgSpace.bottom = viewFinder.height();
    }
    
    // total tiles visible in viewFinder
    numVisibleTiles.x = Math.ceil((visibleImgSpace.right - visibleImgSpace.left) / tileSize) + 1;
    numVisibleTiles.y = Math.ceil((visibleImgSpace.bottom - visibleImgSpace.top) / tileSize) + 1;
        
    if (imgFrame.position().left < 0) {
      visibleTileIds.leftmost = Math.abs(Math.ceil(imgFrame.position().left / tileSize));
    }

    if (imgFrame.position().top < 0) {
      visibleTileIds.topmost = Math.abs(Math.ceil(imgFrame.position().top / tileSize));
    }
    
    visibleTileIds.rightmost  = visibleTileIds.leftmost + numVisibleTiles.x;
    visibleTileIds.bottommost = visibleTileIds.topmost + numVisibleTiles.y;
    
    // preload extra tiles for better user experience
    visibleTileIds.leftmost   -= settings.preloadTilesOffset;
    visibleTileIds.topmost    -= settings.preloadTilesOffset;
    visibleTileIds.rightmost  += settings.preloadTilesOffset;
    visibleTileIds.bottommost += settings.preloadTilesOffset;
    
    // validate visible tile ids
    if (visibleTileIds.leftmost < 0) { 
      visibleTileIds.leftmost = 0; 
    }
    
    if (visibleTileIds.topmost < 0) { 
      visibleTileIds.topmost = 0; 
    }
    
    if (visibleTileIds.rightmost > totalTiles.x) {
      visibleTileIds.rightmost = totalTiles.x;
    }

    if (visibleTileIds.bottommost > totalTiles.y) {
      visibleTileIds.bottommost = totalTiles.y;
    }

    for (var x = visibleTileIds.leftmost; x < visibleTileIds.rightmost; x++) {          
      for (var y = visibleTileIds.topmost; y < visibleTileIds.bottommost; y++) {
        visibleTileArray[ctr++] = [x, y];
      }
    }
        
    return visibleTileArray;
  };
  
  
  /* add tiles to the imgFrame */
  var showTiles = function() {
    var visibleTiles = getVisibleTiles();
    var visibleTilesMap = [];
    var multiplier = Math.pow(2, jp2.levels - currentLevel);
    var tileSize = settings.tileSize;
    
    // prepare each tile and add it to imgFrame
    for (var i = 0; i < visibleTiles.length; i++) {
      var attrs = { x: visibleTiles[i][0], y: visibleTiles[i][1] };      
      var tile = undefined;
      
      var insetValueX = attrs.x * tileSize * multiplier;
      var insetValueY = attrs.y * tileSize * multiplier;        
      
      attrs.id = 'tile-x' + attrs.x + 'y' + attrs.y + 'z' + currentLevel + 'r' + currentRotation + '-' + viewFinderId;      
      attrs.src = jp2.djatokaURL + '&svc.level=' + currentLevel + '&svc.region=' + insetValueY + ',' + insetValueX + ',' + tileSize + ',' + tileSize + '&svc.rotate=' + currentRotation;
      
      visibleTilesMap[attrs.id] = true; // useful for removing unused tiles       
      tile = $('#' + attrs.id);
                        
      if (tile.length == 0) {
        tile = $(document.createElement('img'));
        
        tile.css({
          'position': 'absolute'
        }).attr({
          'id': attrs.id,
          'src': attrs.src
        });
        
        switch (parseInt(currentRotation, 10)) {
          case 90:
            tile.css({ 
              'right': (attrs.y * tileSize) + 'px', 
              'top': (attrs.x * tileSize) + 'px' 
            });
            break;
          
          case 180:
            tile.css({ 
              'right': (attrs.x * tileSize) + 'px', 
              'bottom': (attrs.y * tileSize) + 'px' 
            });
            break;
          
          case 270:
            tile.css({ 
              'left': (attrs.y * tileSize) + 'px', 
              'bottom': (attrs.x * tileSize) + 'px' 
            });
            break;
          
          default:
            tile.css({ 
              'left': (attrs.x * tileSize) + 'px', 
              'top': (attrs.y * tileSize) + 'px' 
            });
        }
        
        imgFrame.append(tile);
      }      
    }
    
    removeUnusedTiles(visibleTilesMap);
    storeRelativeLocation();
    drawMarquee();    
  };
  
  
  /* remove unused tiles to save memory */
  var removeUnusedTiles = function (visibleTilesMap) {    
    imgFrame.find('img').each(function(index) {
      if (/^tile-x/.test(this.id) && !visibleTilesMap[this.id]) {
        $('#' + this.id).remove();       
      }       
    });    
  }
  
  
  /* setup zoom controls */
  var zoom = function(direction) {
    var newLevel = currentLevel;

    switch (direction) {
      case 'in': 
        newLevel = util.clampLevel(newLevel + 1); 
        break;
        
      case 'out': 
        newLevel = util.clampLevel(newLevel - 1); 
        break;
        
      default: ;
    }
    
    if (newLevel != currentLevel) {
      currentLevel = newLevel;
      setImgFrameSize(currentLevel);
    }
  }
  

  /* setup rotate controls */
  var rotate = function(direction) {
    var newRotation = currentRotation;
    var angle = 90;

    switch (direction) {
      case 'cw': 
        newRotation = currentRotation + 90;         
        break;
        
      case 'ccw': 
        newRotation = currentRotation - 90;
        angle = -90;
        break;
        
      default: ;
    }
    
    if (newRotation < 0) { newRotation += 360; }
    if (newRotation >= 360) { newRotation -= 360; }
    
    if (newRotation != currentRotation) {
      currentRotation = newRotation;
      swapJp2Dimensions();
      swapRelativeLocationValues(angle);
      setImgFrameSize(currentLevel);
      setupMarquee();
    }
  }


  /* for rotate actions, swap jp2 dimensions */
  var swapJp2Dimensions = function() {
    var tmpWidth = jp2.width;
    
    jp2.width = jp2.height;
    jp2.height = tmpWidth; 
  }


  /* for rotate actions, swap relative location values based on given value */ 
  var swapRelativeLocationValues = function(angle) {
    var tmpX = imgFrameAttrs.relativeLoc.x;
    
    if (parseInt(angle, 10) > 0) {
      imgFrameAttrs.relativeLoc.x = 1 - imgFrameAttrs.relativeLoc.y;
      imgFrameAttrs.relativeLoc.y = tmpX;
    } else {
      imgFrameAttrs.relativeLoc.x = imgFrameAttrs.relativeLoc.y;
      imgFrameAttrs.relativeLoc.y = 1 - tmpX;      
    }
  }

  
  /* store imgFrame relative location - for positioning after zoom/rotate */
  var storeRelativeLocation = function() {
    
    imgFrameAttrs.relativeLoc.x = 
      (Math.round((viewFinder.width() / 2) - imgFrame.position().left) / imgFrame.width()).toFixed(2);

    imgFrameAttrs.relativeLoc.y = 
      (Math.round((viewFinder.height() / 2) - imgFrame.position().top) / imgFrame.height()).toFixed(2);
  }
  
  
  /* setup mouse events for imgFrame dragging */
  var setupImgFrameDragging = function() {    
    var attrs = {
      isDragged: false, 
      left: 0,
      top: 0,
      drag: { left: 0, top: 0 },
      start: { left: 0, top: 0 },
    };
 
    imgFrame.bind({
      mousedown: function(event) {        
        if (!event) { event = window.event; } // required for IE
        
        attrs.drag.left = event.clientX;
        attrs.drag.top  = event.clientY;
        attrs.start.left = imgFrame.position().left;
        attrs.start.top  = imgFrame.position().top;
        attrs.isDragged  = true;         

        imgFrame.css({
          'cursor': 'default', 
          'cursor': '-moz-grabbing',
          'cursor': '-webkit-grabbing'
        });
           
        return false;
      },
      
      mousemove: function(event) {
        if (!event) { event = window.event; } // required for IE
        
        if (attrs.isDragged) {
          attrs.left = attrs.start.left + (event.clientX - attrs.drag.left);      
          attrs.top = attrs.start.top + (event.clientY - attrs.drag.top);
  
          imgFrame.css({
            'left': attrs.left + 'px',
            'top': attrs.top + 'px'
          });
                    
          showTiles();        
        }        
      }      
    });

    imgFrame.ondragstart = function() { return false; } // for IE    
    $(document).mouseup(function() { stopImgFrameMove();  });

    var stopImgFrameMove = function() {
      attrs.isDragged = false;      
      imgFrame.css({ 'cursor': '' });
    };        
  };
  
  
  /* setup mouse events for marquee dragging */
  var setupMarqueeDragging = function() {
    var marqueeBgId = viewFinderId + '-marquee-bg';    
    var marqueeId = viewFinderId + '-marquee';        
    
    var attrs = {
      isDragged: false, 
      left: 0,
      top: 0,
      drag: { left: 0, top: 0 },
      start: { left: 0, top: 0 },
    };
 
    var marquee = $('#' + marqueeId);   
 
    marquee.bind({
      mousedown: function(event) {        
        if (!event) { event = window.event; } // required for IE
        
        attrs.drag.left = event.clientX;
        attrs.drag.top  = event.clientY;
        attrs.start.left = marquee.position().left;
        attrs.start.top  = marquee.position().top;
        attrs.isDragged  = true;         

        marquee.css({
          'cursor': 'default', 
          'cursor': '-moz-grabbing',
          'cursor': '-webkit-grabbing'          
        });
           
        return false;
      },
      
      mousemove: function(event) {
        if (!event) { event = window.event; } // required for IE
        
        if (attrs.isDragged) {
          attrs.left = attrs.start.left + (event.clientX - attrs.drag.left);      
          attrs.top = attrs.start.top + (event.clientY - attrs.drag.top);
  
          // limit marquee dragging to within the marquee background image
          var maxLeft = marqueeAttrs.imgWidth - marquee.width();
          var maxTop  = marqueeAttrs.imgHeight - marquee.height();
  
          // validate positioning values
          if (attrs.left < 0) { attrs.left = 0; } 
          if (attrs.top < 0) { attrs.top = 0; } 
          
          if (attrs.left > maxLeft) { attrs.left = maxLeft; }
          if (attrs.top > maxTop) { attrs.top = maxTop; }
          
          marquee.css({
            'left': attrs.left + 'px',
            'top': attrs.top + 'px'
          });                    
        }        
      }      
    });

    marquee.ondragstart = function() { return false; } // for IE    
    //$(document).mouseup(function() { stopMarqueeMove();  });
    marquee.mouseup(function() { stopMarqueeMove();  });

    var stopMarqueeMove = function() {
      attrs.isDragged = false;      
      marquee.css({ 'cursor': '' });

      imgFrameAttrs.relativeLoc.x = ((marquee.position().left + (marquee.width() / 2)) / marqueeAttrs.imgWidth).toFixed(2);
      imgFrameAttrs.relativeLoc.y = ((marquee.position().top + (marquee.height() / 2)) / marqueeAttrs.imgHeight).toFixed(2);
      positionImgFrame();      
    };            
  }
  
  
  /* setup marquee box, background image and marquee  */
  var setupMarquee = function() {
    var level = util.clampLevel(getLevelForContainer(settings.marqueeImgSize) + 1);
    var marqueeBoxId = viewFinderId + '-marquee-box';
    var marqueeBgId = viewFinderId + '-marquee-bg';    
    var marqueeId = viewFinderId + '-marquee';        
    
    if ($('#' + marqueeBoxId).length != 0) {
      $('#' + marqueeBoxId).remove();
    }
    
    storeRelativeLocation();
    setMarqueeImgDimensions();
        
    var marqueeURL = jp2.djatokaURL + 
      '&svc.level=' + level + 
      '&svc.scale=' + marqueeAttrs.imgWidth + ',' + marqueeAttrs.imgHeight + 
      '&svc.rotate=' + currentRotation;
      
    viewFinder
    .append($('<div>', { 'id': marqueeBoxId, 'class': 'zpr-marquee-box' })
      .append($('<div>', { 'id': marqueeBgId }))    
    );    
      
    var divMarqueeBgImg = $('#' + marqueeBgId);
    
    divMarqueeBgImg.css({
      'height': (marqueeAttrs.imgHeight + 4) + 'px', // 4 = marquee border  
      'width':  (marqueeAttrs.imgWidth + 4) + 'px', // 4 = marquee border
      'position': 'relative', 
      'background': '#fff url(\'' + marqueeURL + '\')  no-repeat center center'  
    });
    
    // append marquee to div with marquee background image
    divMarqueeBgImg.append($('<div>', { 
      'id': marqueeId, 
      'class': 'zpr-marquee'      
    }));
    
    setupMarqueeDragging();
    drawMarquee();
  };


  /* draw marquee and position it */
  var drawMarquee = function() {    
    var left = Math.ceil((imgFrameAttrs.relativeLoc.x * marqueeAttrs.imgWidth) - (marqueeAttrs.width / 2));
    var top = Math.ceil((imgFrameAttrs.relativeLoc.y * marqueeAttrs.imgHeight) - (marqueeAttrs.height / 2));
    
    $('#' + viewFinderId + '-marquee').css({
      'left': left + 'px',
      'top': top + 'px',
      'height': marqueeAttrs.height + 'px',
      'width': marqueeAttrs.width + 'px'                  
    });
  };


  /* set initial marquee dimensions */
  var setMarqueeImgDimensions = function() {
    var aspectRatio = (jp2.width / jp2.height).toFixed(2);
       
    if (aspectRatio > 1) {
      marqueeAttrs.imgWidth  = settings.marqueeImgSize;
      marqueeAttrs.imgHeight = Math.round(settings.marqueeImgSize / aspectRatio);            
    } else {
      marqueeAttrs.imgWidth  = Math.round(settings.marqueeImgSize * aspectRatio);                  
      marqueeAttrs.imgHeight = settings.marqueeImgSize;
    }
    
    setMarqueeDimensions();
  };  
  
  var setMarqueeDimensions = function() {
    marqueeAttrs.width = Math.ceil((viewFinder.width() / imgFrameAttrs.proportionalWidth) * marqueeAttrs.imgWidth) - 4;
    marqueeAttrs.height = Math.ceil((viewFinder.height() / imgFrameAttrs.proportionalHeight) * marqueeAttrs.imgHeight) - 4;    
  }

  /* utility functions */
  var util = {
    // clamp jp2 level between 0 and max level 
    clampLevel: function(level) {
      if (level < 0) { level = 0; }
      if (level > jp2.levels) { level = jp2.levels; }    
      return level;
    }
  };
  
  init();
};
