/*jslint browser: true*/
/*global OpenSeadragon, Drupal*/
/**
 * @file
 * Defines the source for exposing Djatoka to OpenSeadragon.
 */
(function($) {
  'use strict';

  // Remove IIFTileSource, as it assumes Djatoka responses belong to it for some reason.
  $.IIIFTileSource = undefined;

  /**
   * @class DjatokaTileSource
   * @classdesc A client implementation for Djatoka
   *
   * @memberof OpenSeadragon
   * @extends OpenSeadragon.TileSource
   */
  $.DjatokaTileSource = function(options) {
    $.extend(true, this, options);
    $.TileSource.apply(this, [options]);
  };

  $.extend($.DjatokaTileSource.prototype, $.TileSource.prototype, {
    success: function(event) {
      if (event.tileSource !== undefined) {
        $.extend(true, this, event.tileSource);
      }
    },

    /**
     * Determine if the data and/or url imply the image service is supported by
     * this tile source.
     * @function
     * @param {Object|Array} data
     * @param {String} optional - url
     */
    supports: function(data, url) {
      return true;
    },

    /**
     * Responsible for parsing and configuring the
     * image metadata pertinent to this TileSources implementation.
     * @function
     * @param {Object} data - the raw configuration
     * {
     *   "identifer" :
     *   "width" : 6000,
     *   "height" : 4000,
     *   "tileWidth" : 256,
     *   "tileHeight" : 256,
     *   "tileOverlap" : 0,
     *   "minLevel" : 1,
     *   "maxLevel" : 5
     * }
     */
    configure: function(options) {
      var data = {
        identifier: options.identifier,
        width: parseInt(options.width),
        height: parseInt(options.height),
        tileWidth: parseInt(options.tileWidth) || 256,
        tileHeight: parseInt(options.tileHeight) || 256,
        tileOverlap: parseInt(options.tileOverlap) || 0,
        minLevel: parseInt(options.minLevel) || 0,
        maxLevel: parseInt(options.maxLevel) || parseInt(options.levels),
      };
      // Identifier is required for fetching the meta-data and rendering.
      if (!data.identifier) {
        throw Error("Can not create DjatokaTileSource without an identifier.");
      }
      // If default's are not provided, fetch meta-data asynchronously.
      if (!data.width || !data.height || !data.maxLevel) {
        data.url = get_image_info_url(data.identifier);
      }
      return data;
    },

    /**
     * Djatoka helper function for determining the region to request.
     * @function
     * @param {String} url
     * @throws {Error}
     */
    getRegion: function(level, x, y) {
      var bounds = this.getTileBounds(level, x, y),
          region = {
            x: (bounds.x * this.dimensions.x).toFixed(),
            y: (bounds.y * this.dimensions.y * this.aspectRatio).toFixed(),
            width: this._tileWidth,
            height: this._tileHeight
          };
      return region.y + ',' + region.x + ',' + region.width + ',' + region.height;
    },

    /**
     * Responsible for retrieving the url which will return an image for the
     * region specified by the given x, y, and level components.
     * @function
     * @param {Number} level - z index
     * @param {Number} x
     * @param {Number} y
     * @throws {Error}
     */
    getTileUrl: function(level, x, y) {
      var baseURL = Drupal.settings.islandoraOpenSeadragon.djatokaServerBaseURL;
      return baseURL + '?' + jQuery.param({
            'url_ver': 'Z39.88-2004',
            'rft_id': this.identifier,
            'svc_id': 'info:lanl-repo/svc/getRegion',
            'svc_val_fmt': 'info:ofi/fmt:kev:mtx:jpeg2000',
            'svc.region': this.getRegion(level, x, y),
            'svc.level': level
          });
    }
  });

  /**
   * Generates a URL to fetch image meta-data from Djatoka.
   * @function
   * @param {string} pid - fedora object identifier.
   * @return {string} URL to fetch image meta-data from Djatoka.
   */
  function get_image_info_url(identifier) {
    var baseURL = Drupal.settings.islandoraOpenSeadragon.djatokaServerBaseURL;
    return baseURL + '?' + jQuery.param({
          url_ver: "Z39.88-2004",
          rft_id: identifier,
          svc_id: "info:lanl-repo/svc/getMetadata"
        });
  }
}(OpenSeadragon));
