This module extends the date_popup module to provide a JQuery UI inline 
datepicker, rather than a popup out of a textfield. For an example of this see
http://jqueryui.com/demos/datepicker/#inline

There are 3 ways to use this module.

1. Using the Form API setting #type to datepicker, eg
   $form['date'] = array(
     '#type' => 'datepicker',
     '#date_format' => 'd/m/Y',
   );
   
2. Using the datepicker_block submodule, which provides a block that can be
   used to filter another view. This module will simply submit a $_GET variable
   to an internal Drupal path, for example submit might take you to
   http://example.com/events?date_filter[value][date]=2012-06-20
   
1. Using the Views filter handler for Date filters. You must enable the 
   Datepicker Views sub-module to use this.