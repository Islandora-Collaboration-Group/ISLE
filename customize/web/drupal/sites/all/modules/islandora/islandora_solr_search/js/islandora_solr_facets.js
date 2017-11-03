/**
 * @file
 * Javascript file for islandora solr search facets
 */

(function ($) {

  // Adds facet toggle functionality
  Drupal.behaviors.islandoraSolrToggle = {
    attach: function(context, settings) {
      // show more
      if (!$(".soft-limit").hasClass('processed')) {
        $(".soft-limit").click(function(e) {
          // toggle class .hidden
          $(this).prev(".islandora-solr-facet").toggleClass('hidden');
          if ($(this).text() == Drupal.t('Show more')) {
            $(this).text(Drupal.t('Show less'));
          }
          else {
            $(this).text(Drupal.t('Show more'));
          }
          e.preventDefault();
        });
        $(".soft-limit").addClass('processed');
      }
    }
  }

  // Show/hide date filter
  Drupal.behaviors.islandoraSolrDateFilter = {
    attach: function(context, settings) {
      // set variables
      var stringHide = Drupal.t('Hide');
      var stringShow = Drupal.t('Show');

      if (!$('.toggle-date-range-filter').hasClass('processed')) {

        // hide all regions that should be collapsed
        $('.date-range-collapsed').parent('.date-filter-toggle-text').next('.date-range-filter-wrapper').css({'display': 'none'});

        $('.toggle-date-range-filter').click(function() {
          // toggle strings
          if ($(this).html() == stringHide) {
            $(this).html(stringShow);
          }
          else {
            $(this).html(stringHide);
          }

          // toggle wrapper
          $(this).parent('.date-filter-toggle-text').next('.date-range-filter-wrapper').slideToggle('fast');

          return false;
        });

        $('.toggle-date-range-filter').addClass('processed');
      }
    }
  }

  // Datepicker
  Drupal.behaviors.islandoraSolrDatepicker = {
    attach: function(context, settings) {
      if (!settings.islandoraSolrDatepickerRange) {
        return;
      }
      var datepickerRange = settings.islandoraSolrDatepickerRange;
      $.each(datepickerRange, function() {
        var formKey = this.formKey;
        var yearRangeVal = this.datepickerRange;
        // set datepicker
        $(".islandora-solr-datepicker-" + formKey).datepicker({
          changeMonth: true,
          changeYear: true,
          dateFormat: "yy/mm/dd",
          yearRange: yearRangeVal
        });
      });
    }
  }

  // Range slider
  Drupal.behaviors.islandoraSolrRangeSlider = {
    attach: function(context, settings) {

      // get year range variable
      var rangeSliderVals = settings.islandoraSolrRangeSlider;
      if (rangeSliderVals) {
        // loop over each range slider facet
        $.each(rangeSliderVals, function() {
          // set variables
          var sliderData = this.data;
          var form_key = this.form_key;
          var sliderId = '#date-range-slider-' + form_key;
          var amountId = '#slider-amount-' + form_key;
          var canvasId = '#date-range-slider-canvas-' + form_key;
          var rangeSliderColor = this.slider_color;
          if (!rangeSliderColor) {
            rangeSliderColor = '#edc240';
          }
          var sliderMax = sliderData.length - 1;
          var sliderMin = 0;
          var sliderStep = 1;

          // set jquery ui slider
          $(sliderId).slider({
            range: true,
            handles: [{start:sliderMin, min:sliderMin, max:sliderMax, id:'range-slider-handle-min-' + form_key}, {start:sliderMax, min:sliderMin, max:sliderMax, id:'range-slider-handle-max-' + form_key}],
            values: [sliderMin, sliderMax],
            min: sliderMin,
            max: sliderMax,
            step: sliderStep,
            slide: function(event, ui) {
              slider_update(ui.values[0], ui.values[1]);
            },
            slide: function(event, ui) {
              slider_update(ui.values[0], ui.values[1]);
            }
          });

          // function to update the slider values and position
          function slider_update(fromVal, toVal) {
            // get dates
            var fromDate = sliderData[fromVal].date;
            var toDate = sliderData[toVal].date;

            // assign to hidden field
            $('.range-slider-hidden-from-' + form_key).val(fromDate);
            $('.range-slider-hidden-to-' + form_key).val(toDate);

            // get formatted dates
            var formatFromDate = sliderData[fromVal].bucket;
            var formatToDate = sliderData[toVal].bucket;

            // update slider values
            $(sliderId).slider('values', 0, fromVal);
            $(sliderId).slider('values', 1, toVal);

            // assign to popup
            $(sliderId + ' .slider-popup-from').html(formatFromDate);
            $(sliderId + ' .slider-popup-to').html(formatToDate);

            // update plots
            plot.unhighlight();
            for (i = fromVal; i < toVal; i++) {
              plot.highlight(0, i);
            }
          }

          // set canvas width to auto for responsiveness
          $(canvasId).width('auto').height('120px');

          // set color for the slider.
          $(sliderId + ' .ui-slider-range').css({'background': rangeSliderColor});

          // add classes to slider handles
          $(sliderId + ' > a:eq(0)').addClass('handle-min').prepend('<div class="slider-popup-from-wrapper slider-popup"><span class="slider-popup-from">' + sliderData[0].bucket + '</span></div>').hover(function() {
            $('#range-slider-tooltip').remove();
            $(this).find('.slider-popup-from-wrapper').stop(false, true).fadeIn(0);
          }, function() {
            $(this).find('.slider-popup-from-wrapper').stop(false, true).fadeOut('slow');
          });

          $(sliderId + ' > a:eq(1)').addClass('handle-max').prepend('<div class="slider-popup-to-wrapper slider-popup"><span class="slider-popup-to">' + sliderData[sliderData.length-1].bucket + '</span></div>').hover(function() {
            $('#range-slider-tooltip').remove();
            $(this).find('.slider-popup-to-wrapper').stop(false, true).fadeIn(0);
          }, function() {
            $(this).find('.slider-popup-to-wrapper').stop(false, true).fadeOut('slow');
          });

          // Flot
          // prepare flot data
          var d1 = [];
          for (var i = 0; i <= sliderMax - 1; i += 1) {
            d1.push([i, this.data[i].count]);
          }

          // render Flot graph
          var plot = $.plot($(canvasId), [d1], {
            colors: [rangeSliderColor],
            xaxis: {  ticks: [], min: 0, autoscaleMargin: 0},
            yaxis: {  ticks: [], min: 0, autoscaleMargin: 0},
            series: {
              stack: false,
              lines: {
                show: false
              },
              bars: {
                show: true,
                lineWidth: 1, // in pixels
                barWidth: 0.8, // in units of the x axis
                fill: true,
                fillColor: null,
                align: "left", // or "center"
                horizontal: false
              }
            },
            grid: {
              show: true,
              labelMargin: null, // in pixels
              axisMargin: null, // in pixels
              borderWidth: null, // in pixels
              markingsLineWidth: null,
              // interactive stuff
              clickable: true,
              hoverable: true,
              autoHighlight: false, // highlight in case mouse is near
              mouseActiveRadius: 0 // how far the mouse can be away to activate an item
            }
          });


          // add plotclick event to update the sliders
          $(canvasId).bind("plotclick", function (event, pos, item) {
            if (item !== null) {
              // get variable
              var dataIndexValue = item.dataIndex;
              // update the slider and form values
              slider_update(dataIndexValue, dataIndexValue + 1);
            }
          });

          // show tooltip
          function show_tooltip(x, y, contents) {
            //  hide or remove all other popups
            $('#range-slider-tooltip').remove();
            $('.slider-popup').hide();
            $('<div id="range-slider-tooltip"></div>').css( {
                top: y - 50,
                left: x - 125
            }).html('<span>' + contents + '</span>').appendTo("body").fadeIn(0);
          }

          var previousPoint = null;
          // bind plothover
          $(canvasId).bind("plothover", function (event, pos, item) {
            if (item) {
                previousPoint = item.dataIndex;

                // fadeout and remove
                $('#range-slider-tooltip').fadeOut('slow', function() {
                  $(this).remove();
                });

                // update mouse position
                var x = pos.pageX,
                    y = pos.pageY;

                // get variable
                var dataIndexValue = item.dataIndex;
                var dataIndexValueNext = dataIndexValue + 1;
                var tooltipContent = sliderData[dataIndexValue].bucket + ' - ' + sliderData[dataIndexValueNext].bucket + ' (<em>' + sliderData[dataIndexValue].count + '</em>)';

                // call show tooltip function
                show_tooltip(pos.pageX, pos.pageY, tooltipContent);
            }
            else {
              // fadeout and remove
              $('#range-slider-tooltip').fadeOut('slow', function() {
                $(this).remove();
              });
              previousPoint = null;
            }
          });
        }); // end $.each()
      }
    }
  }

})(jQuery);
