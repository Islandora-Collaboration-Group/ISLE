(function ($) {

Drupal.behaviors.webform_ajax = {

  attach: function (context, settings) {

    // Fully declare element_settings, as Drupal's ajax.js seems not to merge defaults correctly.
    var ajax_element_settings_defaults = {
      url: settings.webform_ajax.url + '/',
      event: 'mousedown',
      prevent: 'click',
      keypress: false,
      selector: '#',
      effect: 'none',
      speed: 'none',
      method: 'replaceWith',
      wrapper: '',
      progress: {
        type: 'throbber',
        message: ''
      },
      submit: {
        'js': true
      }
    };

    // Bind Ajax behaviors to Webform confirmation screen's "Go back to form" link.
    $('.links a:not(.ajax-processed)', context).addClass('ajax-processed').each(function () {
      $wrapper = $(this).parents('[id^=webform-ajax-wrapper]');
      if ($wrapper.length) {
        wrapper_id = $wrapper.attr('id');
        setting = settings.webform_ajax.wrappers[wrapper_id];

        var element_settings = ajax_element_settings_defaults;
        element_settings.url += setting.nid + '/' + setting.wrapper_id;
        element_settings.selector += setting.wrapper_id;
        element_settings.wrapper += setting.wrapper_id;

        Drupal.ajax[setting.wrapper_id] = new Drupal.ajax(setting.wrapper_id, this, element_settings);
      }
    });

    // If module set a 'reload' setting, then retrieve the Webform with AJAX.
    if (setting = settings.webform_ajax.reload) {
      var element_settings = ajax_element_settings_defaults;
      element_settings.url += setting.nid + '/' + setting.wrapper_id;
      element_settings.selector += setting.wrapper_id;
      element_settings.wrapper += setting.wrapper_id;

      button = $('#' + setting.button_id, context);

      Drupal.ajax[setting.wrapper_id] = new Drupal.ajax(setting.wrapper_id, button, element_settings);
      Drupal.ajax[setting.wrapper_id].eventResponse(button, null);

      // Remove the reload setting to avoid looping.
      delete settings.webform_ajax.reload;
    }

  }

};

}(jQuery));
