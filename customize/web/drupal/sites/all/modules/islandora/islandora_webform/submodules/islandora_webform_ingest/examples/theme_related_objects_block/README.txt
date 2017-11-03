This is an example of one way modify the output of the associated objects block
in your theme configuration to show particular data that you are interested in.

These files would be placed into your sites/all/themes/YOUR_THEME folder:

1 - Copy the YOUR_THEME_preprocess_islandora_basic_collection_grid function to
your theme's template.php file. Edit "YOUR_THEME" to match the name of your
theme.

2 - Edit the function to pull in the data for each associated object that you
want to display if it is not present already. In this example we used a
function called iw_get_solr_data_array(), which has  better performance than
getting the full contents of an object's xml datastream and parsing out the
field we are interested in. You can query for any number of fields with this
function: just list them in one string, separated by commas.

3 - One thing you can inspect inside this preprocess function is the list of
theme hook suggestions, which informs you about possible template file
names. Or you can add your own suggestions in this function. In this case, we chose to copy the
islandora-basic-collection-grid.tpl.php file, and put it inside our theme's templates
folder, renaming it as islandora-basic-collection-grid--islandora-webform-ingest.tpl.php
This ensures that it will not be inadvertently used for regular islandora
basic collection grid views.

4 - In the .tpl file, inside the loop that displays the associated objects,
we added a test for the presence of the value we queried for, and if present
print the value, wrapped in suitable html.
