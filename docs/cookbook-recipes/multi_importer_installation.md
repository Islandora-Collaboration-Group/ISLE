# IMI Ingest: Setup and Use with ISLE

These instructions will walk users through the installation of the Islandora Multi-Importer (IMI) Module and a test batch ingest of sample items.  They assume you have followed the instructions for [Local ISLE Installation: New Site](../../install/install-local-new/).  Staging and Production servers may require a different entry in the docker-compose.yml volume section.

## Docker Down, Add New Volume in the Apache Section for Staging Server Side Ingest, Docker Up

In a terminal, cd to directory containing your local ISLE repository.  If you have just completed installing the local ISLE site, your are likely already in that directory.  Otherwise:

`cd {yourprojectnamehere}-isle`

Run `docker-compose down`

Create an ingest data directory on the host computer, to be shared with the Docker container:

`mkdir ./ingest_data`

In a text editor, edit docker-compose.yml - add "./ingest_data:/mnt/ingest" into the Apache > Volumes section and save.

```
apache:
# build:
#   context: ../images/isle-apache
image: islandoracollabgroup/isle-apache:1.1.1
container_name: isle-apache-${CONTAINER_SHORT_ID}
env_file:
.env
networks:
isle-internal:
depends_on:
mysql
fedora
solr
traefik
volumes:
isle-apache-data:/var/www/hpl:cached
./ingest_data:/mnt/ingest
```
run `docker-compose up -d`

## Set Up Batch Functionality and the IMI Module

`cd ingest_data`

Copy and paste the entire block of code below into the command line to create the installer script:

```
echo 'cd /var/www/html && drush -y -u 1 en islandora_batch
cd /var/www/html/sites/all/modules/islandora && git clone https://github.com/mjordan/islandora_batch_with_derivs.git
cd /var/www/html && drush -y -u 1 en islandora_batch_with_derivs
cd /var/www/html && drush -y -u 1 en islandora_book_batch
cd /var/www/html && drush -y -u 1 en islandora_newspaper_batch
cd /var/www/html/sites/all/modules/islandora && git clone https://github.com/MarcusBarnes/islandora_compound_batch.git
cd /var/www/html && drush -y -u 1 en islandora_compound_batch
cd /var/www/html/sites/all/modules/islandora && git clone https://github.com/mnylc/islandora_multi_importer.git
cd /var/www/html/sites/all/modules/islandora/islandora_multi_importer &&  composer install
cd /var/www/html && drush en islandora_multi_importer -y
cd /mnt/ingest && git clone https://github.com/Born-Digital-US/isle-ingest-samples.git
unzip isle-ingest-samples/IMI/IMIasset_files.zip -d .
chown -R islandora:www-data /mnt/ingest' > batch_installer.sh
```

Update the script file permissions on the container:

`docker exec -it isle-apache-ld bash -c "chmod 744 /mnt/ingest/batch_installer.sh"`

Run batch_installer.sh on the container:

`docker exec -it isle-apache-ld bash -c "/mnt/ingest/batch_installer.sh"`


## Islandora ingest overview and IMI demonstration

The script will pack a set of sample files in the local ingest_data directory

Drill down to the ./ingest_data/isle-ingest-samples/IMI directory and look for the following three files:

* mods_twig_base.txt
* testCol-tn.png.zip
* testCol.csv

Visit one of these two pages:

  * on your Demo site > https://isle.localdomain/multi_importer
  * on your Local site > https://yourlocalnamehere.localdomain/multi_importer

Use the data in mods_twig_base.txt to create and save an IMI metadata template

In the Drupal site, in the Navigation section on the left, click the "Multi Import Objects" link

In the "Choose your Data source type" drop down, select "Spreadsheet File to be uploaded" aka a ".csv file" and click the "Next" button.

Click the "Choose File" button and select the csv file: "testCol.csv" and then click the "Upload" button.

Click the "Preprocess" button

In the "Your data" tab, toggle / click the "Preview submitted..." drop-down to verify there's data in your rows

Go to the "Templating" tab:

  * If there is not yet a twig template:

    * Delete the sample `{% block content %} Hello {{ name }}{% endblock %}` from the "Twig Template Input" field.

    * Copy and paste the "mods_twig_base.txt" template text into the template text area.

    * Within the "Manage your templates" section at the bottom, click the "Save Template As"

    * Enter a new name e.g. "bd_mods_template" in the "Name for your new template" field and then click the "Save Template" button. A green check mark will indicate a successful save.

  * Else if there is a preexisting twig template -select the previously saved template.

Go to the "CMODEL Mapping" tab:

  * Select from the drop-down list; "cmodel" and click the "Check CMODELS" button.

  * A block of drop-downs should appear for each CModel you have in your input (.csv)
      * DC - "default XSLT"

      * TN - select "tn"

Go to the "Object Properties Tab" and within the "Source Field Mapping" table:

  * Set "Object Pid" to "collection_pid" and **uncheck** the checkbox for "Check to let Islandora build PID..."

  * Set "Parent Object" to "parent" and leave checkbox **checked** for "If value is not a well formed PID..."

  * Set "Object Label" to "title"

  * Set "Sequence and Ordering" to "collection_pid" (_most likely default_)

  * Set "Remote DS sources" to "ZIP"

Leave the "What Type of Batch Action…" as it is ("ingest new objects") (_most likely default_)

Click on the "Ingest" button

Click the "Choose File" button and select the zip file: "testCol-tn.png.zip" and the click the "Upload" button.

**Please note:** There will be this warning "For security reasons, your upload has been renamed to testCol-tn.png_.zip." Ignore it.


Click the "Ingest" button

Now you should see a green message box e.g. "You are all set( id = 1)!" with a set ID that is a link. Click on the "id = " link.

The "Set __ Batch Queue" prompt / overlay should appear. Click on the link: "Process Set"

Click on the "Start Batch Processing" button

You should see "Processing complete" message.

Click on the "home" icon at the top left hand of the site to get back to the homepage.

Within the "Navigation" links, click on "Islandora Repository".

Within the list of collections you should see your new "Test Collection" with a witty thumbnail icon. Click on the icon.

To set the CMODELS for this collection, click on the "Manage" tab.

Go to the "Collection" sub-tab, then to the "Manage Collection Policy" sub-tab (_should be there by default_) and then check **ALL** the check boxes within the "About Collection Policies" table to the right.

Click the "Update Collection Policy" button. A green check mark message "updated collection policy..." should appear.

### Proceed to test a full IMI ingest using the local file system option.  Objects will be ingested in to the test collection created above.

In this section, we will use a [Google sheet](https://docs.google.com/spreadsheets/d/1sZ0FY27nxkhM4lCrSZ9nY9mLbr-ljKaZdqTyiRmknTM/edit?usp=sharing) for our sample metadata

In the Drupal site, Navigation section on the left, click the "Multi Import Objects" link

In the "Choose your Data source type" drop down, select "Google Sheet URL" and click the "Next" button.

In the "ID of your Google sheet" box, paste in: `1sZ0FY27nxkhM4lCrSZ9nY9mLbr-ljKaZdqTyiRmknTM`

Cell Range: `Sheet1!A1:AF14`

Click the "Preprocess" button

In the "Your data" tab, toggle / click the "Preview submitted..." drop-down to verify there's data in your rows

Go to the "Templating" tab and select the template created above.  E.g. "bd_mods_template"

Go to the "CMODEL Mapping" tab:

  * Select from the drop-down list; "cmodel" and click the "Check CMODELS" button.

  * A block of drop-downs should appear for each CModel you have in your input: islandora:sp_large_image_cmodel, islandora:sp-audioCModel, islandora:compoundCModel, islandora:bookCModel, islandora:pageCModel and islandora:sp_pdf
      * For each Content Model, set the following:
      
      * DC - "default XSLT"

      * MODS - "bd_mods_template"

      * OBJ - "obj_file"

      * TN - select "tn"

      * All others: "Build Using islandora generated derivatives" OR "-- Don't Create --", depending on how fully you want to test ingest.  The more derivatives you create, the longer the ingest process will be.

Go to the "Object Properties Tab" and within the "Source Field Mapping" table:

  * Set "Object Pid" to "collection_pid" and **uncheck** the checkbox for "Check to let Islandora build PID..."

  * Set "Parent Object" to "parent" and leave checkbox **checked** for "If value is not a well formed PID..."

  * Set "Object Label" to "title"

  * Set "Sequence and Ordering" to "sequence"

  * Set "Remote DS sources" to "local"

Leave the "What Type of Batch Action…" as it is ("ingest new objects") (_most likely default_)

Click on the "Ingest" button

Now you should see a green message box e.g. "You are all set( id = #)!" with a set ID that is a link. Click on the "id = " link.

The "Set __ Batch Queue" prompt / overlay should appear. Click on the link: "Process Set"

Click on the "Start Batch Processing" button

You should see "Processing complete" message.

### View the test objects

Click on the "home" icon at the top left hand of the site to get back to the homepage.

Within the "Navigation" links, click on "Islandora Repository".

Click on the "Test Collection" Thumbnail

The ingested objects should be listed.


### Other Useful links:
* [ISLE Documentation Site](https://github.com/Islandora-Collaboration-Group/ISLE)
* [More Sample Ingest Objects from Born Digital](https://github.com/Born-Digital-US/isle-ingest-samples)
* [IMI Github](https://github.com/mnylc/islandora_multi_importer)
* [Islandora Documentation - How to Add an Item to a Digital Collection](https://wiki.lyrasis.org/display/ISLANDORA/How+to+Add+an+Item+to+a+Digital+Collection)
* [Islandora Documentation - How to Batch Ingest Files](https://wiki.lyrasis.org/display/ISLANDORA7112/How+to+Batch+Ingest+Files)
