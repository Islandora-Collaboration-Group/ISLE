/**
 * @file
 * JS for the Webform Workflow module.
 *
 * This JavaScript is attached to several forms:
 *  - The workflow state creating/editing form, usually seen inside an Inline
 *    Entity Form: see webform_workflow_state_form_attach().
 *  - The workflow state change form, shown when viewing a submission: see
 *    webform_workflow_submission_state_form().
 *  - The exposed filters in the View that shows a list of form submissions
 *    (webform_workflow_submissions).
 */
(function ($, Drupal) {
  Drupal.behaviors.webform_workflow_state_colors = {
    attach: function (context, settings) {
      // Split out the <select> element offering color options into separate
      // colored buttons in a <ul> list.
      $('select.webform-workflow-state-color-list').once('webform-workflow-state-colors').each(function() {
        var select_element = $(this);
        var list = $('<ul>').addClass('webform-workflow-state-color-options');
        select_element.find('option').each(function () {
          var option = $(this),
            color = option.attr('value'),
            label = option.text(),
            list_option = $('<li>')
              .addClass('webform-workflow-state-label')
              .addClass('webform-workflow-state-color-' + color)
              .data('color', color)
              .text(label);
          if (option.is(':selected')) {
            list_option.addClass('selected');
          }
          list.append(list_option);
        });
        select_element.hide().after(list);
        var list_items = $('.webform-workflow-state-color-options li');
        list_items.click(function () {
          var list_item = $(this);
          list_items.removeClass('selected');
          list_item.addClass('selected');
          select_element.val(list_item.data('color'));
          return false;
        });
      });
      // Color in the <select> elements that offer a drop-down list of states.
      if (settings.webformWorkflow !== undefined) {
        $('select.webform-workflow-state-select').once('webform-workflow-state-colors').each(function() {
          var select = $(this),
            updateSelectColor = function () {
              var selected = select.find(':selected:eq(0)');
              if (selected.length) {
                var bgcolor = selected.css('background-color') || 'inherit',
                  color = selected.css('color') || 'inherit';
                select.css('background-color', bgcolor).css('color', color);
              }
            };
          select.find('option,optgroup').each(function () {
            var option = $(this),
              wsid = option.attr('value');
            // The object settings.webformWorkflow.stateColors contains a list
            // of colors, for which each of the keys is 'state-WSID'. WSID is
            // the numeric entity ID of the state.
            if (wsid && settings.webformWorkflow.stateColors['state-' + wsid] !== undefined) {
              option.addClass('webform-workflow-state-color-' + settings.webformWorkflow.stateColors['state-' + wsid]);
            }
            else {
              // Make sure that coloring the <select> element in
              // updateSelectColor() does not affect the color of the empty
              // <option> and <optgroup> elements.
              option.addClass('webform-workflow-state-color-none');
            }
          });
          updateSelectColor();
          select.change(updateSelectColor);
        });
      }
    }
  };
})(jQuery, Drupal);
