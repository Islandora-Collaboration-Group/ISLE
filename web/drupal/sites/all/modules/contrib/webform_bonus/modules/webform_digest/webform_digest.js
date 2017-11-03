Drupal.behaviors.webform_digest = function(context) {

  var update_fields = function() {
    var period = $('#webform-digest-form #edit-period').val();
    switch (period) {
      case 'day':
        $('#webform-digest-form #edit-daily-granularity-month-wrapper').hide();
        $('#webform-digest-form #edit-daily-granularity-week-wrapper').hide();
        break;
      case 'week':
        $('#webform-digest-form #edit-daily-granularity-month-wrapper').hide();
        $('#webform-digest-form #edit-daily-granularity-week-wrapper').show();
        break;
      case 'month':
        $('#webform-digest-form #edit-daily-granularity-month-wrapper').show();
        $('#webform-digest-form #edit-daily-granularity-week-wrapper').hide();
        break;
    }
  }

  update_fields();

  $('#webform-digest-form #edit-period').change(function() {
    update_fields();
  });
}