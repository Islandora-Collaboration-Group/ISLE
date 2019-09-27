# Utilizing and Extending ISLE’s Test Suite

## What is this component?

* Early in the history of ISLE, we did all build tests manually. We even developed a checklist spreadsheet for it: https://docs.google.com/spreadsheets/d/1L-wrivXq2pUz7vcGsMCx3X7yKf27uokoaR8SovU_BsU/edit#gid=0
  * The test coverage section of this document identifies (by Spreadsheet id number) where in the test suite we have addressed the testing requirement.
  * The tests themselves are currently in a temporary location: https://github.com/Born-Digital-US/isle-ingest-samples/tree/master/behat
* ISLE Phase 2 included budget to automate this test suite, and to make it easy to trigger for various ISLE use cases on every build, as well as for use by implementing institutions to check the integrity of their ISLE builds.
* We chose to use Behat to do behavioral testing as it is the most similar to an end-user, and ultimately we want to make sure that ISLE delivers a good end-user experience. Running Unit tests was deemed insufficient, as it only tests programatic aspects, not whether the Drupal UI allows utilization of Islandora's features.
* We chose to use Selinium and Chrome as Docker sidecars for actually executing the tests.


---



## Running ISLE Tests

* The most straightforward example of use is ISLE's testing environment: https://github.com/Islandora-Collaboration-Group/ISLE/blob/master/docker-compose.test.yml as configured by the CircleCI configuration: https://github.com/Islandora-Collaboration-Group/ISLE/blob/master/.circleci/config.yml
* This use case is abstracted and summarized below:

### Installation Instructions:

* Docker-compose.yml additions: several new services, for running Chrome/Selenium
```bash
chrome:
  image: selenium/node-chrome:3.141.59-neon # latest
  container_name: isle-chrome-${CONTAINER_SHORT_ID}
  environment:
    - HUB_PORT=4444
    - HUB_HOST=hub    
  networks:
    - isle-internal    
  depends_on:
    - apache
    - hub
  volumes:
    - /dev/shm:/dev/shm
    - ./data/apache/html:/var/www/html:ro

hub:
  image: selenium/hub:3.141.59-neon # latest
  container_name: isle-hub-${CONTAINER_SHORT_ID}
  networks:
    - isle-internal
  ports:
    - "4444:4444"
```
* Notes:
  * To allow uploads in tests, we must mount in the Apache file root into the "chrome" container.
  * `/dev/shm` must be shared by all testing services or they will run out of RAM and crash
  * Newer `selenium` images may be available, but are untested. YMMV.
* No changes to `.env` file(s) are necessary, although for the examples shown here we have:
  * `COMPOSE_PROJECT_NAME=isle_test`
  * `BASE_DOMAIN=isle.localdomain`
  * `CONTAINER_SHORT_ID=td`
  * `COMPOSE_FILE-COMPOSE-FILE=test.env`
  * If your use case uses other values for these variables, you may need to find/replace in the instructions below (e.g. `isle-apache-td` corresponds with the CONTAINER_SHORT_ID above)
* The full series of commands to install a clean new ISLE with a clean new Islandora Drupal, then add extra modules to support our test cases, and then execute the tests are as follows.
  * Get a good ISLE base for this test run:
```bash
git clone https://github.com/Islandora-Collaboration-Group/ISLE.git
```
  * Then edit the `.env` file to contain the test suite config:
```bash
COMPOSE_PROJECT_NAME=isle_test
BASE_DOMAIN=isle.localdomain
CONTAINER_SHORT_ID=td
COMPOSE_FILE-COMPOSE-FILE=test.env
```
  * Then fire everything back up
```bash
docker-compose pull
docker-compose up -d

sleep 40
```
  * Now install Islandora from scratch, and init your Fedora repository using the tools provided by ISLE:
```bash
set -x && docker exec -it isle-apache-td bash /utility-scripts/isle_drupal_build_tools/isle_islandora_installer.sh
```
  * Now install bulk-ingest dependencies (some not required for our tests) and extra modules required by test coverage:
```bash
docker exec -it isle-apache-td bash -c "cd /var/www/html/sites/all/modules/islandora && git clone https://github.com/Islandora-Labs/islandora_solution_pack_oralhistories.git"
docker exec -it isle-apache-td bash -c "cd /var/www/html && drush -y -u 1 en islandora_oralhistories"
docker exec -it isle-apache-td bash -c "cd /var/www/html && drush dis overlay -y"
docker exec -it isle-apache-td bash -c "cd /var/www/html && drush -y -u 1 en islandora_batch"
docker exec -it isle-apache-td bash -c "cd /var/www/html/sites/all/modules/islandora && git clone https://github.com/mjordan/islandora_batch_with_derivs.git"
docker exec -it isle-apache-td bash -c "cd /var/www/html && drush -y -u 1 en islandora_batch_with_derivs"
docker exec -it isle-apache-td bash -c "cd /var/www/html && drush -y -u 1 en islandora_book_batch"
docker exec -it isle-apache-td bash -c "cd /var/www/html && drush -y -u 1 en islandora_newspaper_batch"
docker exec -it isle-apache-td bash -c "cd /var/www/html/sites/all/modules/islandora && git clone https://github.com/MarcusBarnes/islandora_compound_batch.git"
docker exec -it isle-apache-td bash -c "cd /var/www/html && drush -y -u 1 en islandora_compound_batch"
```
  * Make sure your `/etc/hosts` has an entry for the domain you'll be using (`isle.localdomain` in this example)
  * Now clone the repository with the tests, put it where Drupal expects it (`sites/all/behat`), and install Behat dependencies with Composer:
```bash
git clone git@github.com:Born-Digital-US/isle-behat.git data/isle-behat
docker cp data/isle-behat isle-apache-td:/var/www/html/sites/behat
docker exec -it isle-apache-td bash -c "chown -R islandora:www-data /var/www/html/sites/behat/"
docker exec -it isle-apache-td bash -c "cd /var/www/html/sites/behat && composer install "
docker exec -it isle-apache-td bash -c "chmod 700 /var/www/html/sites/behat/run-isle-tests.sh"
sudo chmod -R 777 ~/isle/data/apache/html/sites/behat/debug
```
   * And now we're ready to run tests:
```bash
docker exec -it isle-apache-td bash -c "cd /var/www/html/sites/behat && ./run-isle-tests.sh --run=services"
docker exec -it isle-apache-td bash -c "cd /var/www/html/sites/behat && ./run-isle-tests.sh --run=apache"
```

## Additional Notes
* If you have your own ISLE already, just omit that initial `git clone` for ISLE. If you use the script, we're assuming you've cloned our whole repository, switched to the `travis` branch, and are running it from there.
* Modifications to tests may be necessary based on your institutional theme and configuration.

---

## Automated test triggers

* CircleCI runs these tests every time an update is pushed to the main ISLE repo. Sometimes things fail because the wind was blowing the wrong way, so we run it again and it will pass.


---


## Troubleshooting

* Come discuss on the ISLE Interest Group email list and/or bi-weekly meetings.


---

## Test Coverage

#### ISLE/DOCKER IMAGES

* 1-1	Able to create new ISLE instance?
    * N/A - our CI tool can set up various situations, one of which could be a new ISLE instance.
* 1-2	Able to view Traefik admin port?
    * Covered by isle-services.feature "traefik online"
* 1-3	Able to view Portainer Admin panel?
    * Covered by isle-services.feature "portainer online"
* 1-4	Able to view Tomcat admin panel for solr container?
    * Covered by isle-services.feature "solr online"
* 1-5	Able to view the Solr admin panel for solr container?
    * Covered by isle-services.feature "solr online"
* 1-6	Able to view Tomcat admin panel for image-services container?
    * Covered by isle-services.feature "imageservices online
* 1-7	Able to view Cantaloupe admin panel for image-services container?
    * Covered by isle-services.feature "imageservices online"
* 1-9	Able to view Tomcat admin panel for fedora container?
    * Covered by isle-services.feature "fedora online"
* 1-10	Able to view Fedora services panels e.g. /objects, /describe etc
    * Covered by isle-services.feature "fedora online"
* 1-11	Able to view Fedoragsearch panels e.g. /fedoragsearch/rest?operation=updateIndex
    * Covered by isle-services.feature "fedora online"

#### DRUPAL INSTALLATION
* 2-1	Able to view domain / website url as anonymous user?
    * Covered by isle-drupal.feature "Viewing homepage"
* 2-2	Able to login to Drupal site as an admin user?
    * Covered by isle-drupal.feature "Viewing login page as admin"
* 2-3	Able to view persistent data folders for xml/xslt/sites and for fedora data store?
    * WONTFIX- N/A
* 2-4	If Drupal multisite, able to login to multisite (not parent) site?
    * MAYBE TODO Not covered. Need test case?
* 2-5	Drupal Status Report not showing errors? https://<domain>/admin/reports/status
    * WONTFIX- Intentionally commented out, but left relic in isle-drupal.feature

#### SAMPLE OBJECT INGESTION
##### Collections
* 3-1	Able to create a new collection e.g. test:collection?
    * This currently happens on test setup
* 3-2	Able to modify a new collection's Collection policies and add Content Models?
    * Covered in the first steps of almost all content model feature files

##### Audio Content Model
* 3-3	Able to ingest the test AUDIO sample objects?
    * Covered by audio.feature: "Ingest Audio Sample Object"
* 3-4	Able to upload thumbnail for Audio object?
    * Covered by audio.feature: "Replace Audio Thumbnail
* 3-5	Able to view / hear an AUDIO object?
    * WONTFIX Not covered - does not appear to be possible?
* 3-6	Able to download an AUDIO object?
    * Covered by audio.feature: "Check for Audio OBJ download"
* 3-7	Able to search for newly ingested AUDIO object using Islandora simple search?
    * Covered by audio.feature: "Check for Audio Objects using simple search"
* 3-8	Able to edit AUDIO object’s title using the XML form?
    * Covered by audio.feature: "Edit Audio object title"
* 3-9	Able to search for newly edited AUDIO object’s title using Islandora simple search?
    * Covered by audio.feature: "Edit Audio object title"
* 3-10	Able to edit the Item Label of an AUDIO object's Properties?
    * Covered by audio.feature: "Edit Audio object Item Label"
* 3-11	Able to search for newly edited Item Label of an AUDIO object's Properties using Islandora simple search?
    * Covered by audio.feature: "Edit Audio object Item Label"
* 3-12	Able to edit MODS datastream for AUDIO object?
    * Covered by audio.feature: "Replace MODS datastream for Audio Object"
* 3-13	Able to search for newly edited MODS datastream for AUDIO object using Islandora simple search?
    * Covered by audio.feature: "Replace MODS datastream for Audio Object"
* 3-14	Able to delete TN derivative for AUDIO object?
    * Covered by audio.feature: "Delete TN derivative for Audio Object"
* 3-15	Able to regenerate all derivatives for AUDIO object?
    * Covered by audio.feature: "Regenerate all derivatives for Audio Object"

##### Basic Image Content Model
* 3-16	Able to ingest these test BASIC IMAGE sample objects?
    * Covered by basicimage.feature: "Ingest Basic Image Sample Object"
* 3-17	Able to view a BASIC IMAGE object?
    * Covered by basicimage.feature: "Check for Basic Image OBJ download"
* 3-18	Able to download a BASIC IMAGE object?
    * Covered by basicimage.feature: "Check for Basic Image OBJ download"
* 3-19	Able to search for newly ingested BASIC IMAGE object using Islandora simple search?
    * Covered by basicimage.feature: "Check for Basic Image Objects using simple search"
* 3-20	Able to edit BASIC IMAGE object’s title using the XML form?
    * Covered by basicimage.feature: "Edit Basic Image object title"
* 3-21	Able to search for newly edited BASIC IMAGE object’s title using Islandora simple search?
    * Covered by basicimage.feature: "Edit Basic Image object title"
* 3-22	Able to edit the Item Label of an BASIC IMAGE object's Properties?
    * Covered by basicimage.feature: "Edit Basic Image object Item Label"
* 3-23	Able to search for newly edited Item Label of an BASIC IMAGE object's Properties using Islandora simple search?
    * Covered by basicimage.feature: "Edit Basic Image object Item Label"
* 3-24	Able to edit MODS datastream for BASIC IMAGE object?
    * Covered by basicimage.feature: "Replace MODS datastream for Basic Image Object"
* 3-25	Able to search for newly edited MODS datastream for BASIC IMAGE object using Islandora simple search?
    * Covered by basicimage.feature: "Replace MODS datastream for Basic Image Object"
* 3-26	Able to delete TN derivative for BASIC IMAGE object?
    * Covered by basicimage.feature: "Delete TN derivative for Basic Image Object"
* 3-27	Able to regenerate all derivatives for BASIC IMAGE object?
    * Covered by basicimage.feature: "Regenerate all derivatives for Basic Image Object"

##### Book Content Model
* 3-28	Able to ingest these test BOOK sample objects?
    * Covered by book.feature: "Ingest Book Sample Object"
* 3-29	Able to view a BOOK object?
    * Covered by book.feature: "Check for BOOK OBJ download"
* 3-30	Able to download a BOOK object?
    * Covered by book.feature: "Check for BOOK OBJ download"
* 3-31	Able to search for newly ingested BOOK object using Islandora simple search?
    * Covered by book.feature: "Check for BOOK Objects using simple search"
* 3-32	Able to edit BOOK object’s title using the XML form?
    * Covered by book.feature: "Edit BOOK object title"
* 3-33	Able to search for newly edited BOOK object’s title using Islandora simple search?
    * Covered by book.feature: "Edit BOOK object title"
* 3-34	Able to edit the Item Label of an BOOK object's Properties?
    * Covered by book.feature: "Edit BOOK object Item Label"
* 3-35	Able to search for newly edited Item Label of an BOOK object's Properties using Islandora simple search?
    * Covered by book.feature: "Edit BOOK object Item Label"
* 3-36	Able to edit MODS datastream for BOOK object?
    * Covered by book.feature: "Replace MODS datastream for BOOK Object"
* 3-37	Able to search for newly edited MODS datastream for BOOK object using Islandora simple search?
    * Covered by book.feature: "Replace MODS datastream for BOOK Object"
* 3-38	Able to delete TN derivative for BOOK object?
    * Covered by book.feature: "Delete TN derivative for BOOK Object"
* 3-39	Able to regenerate all derivatives for BOOK object?
    * Covered by book.feature: "Regenerate all derivatives for BOOK Object"

##### Compound Object Content Model
* 3-40	Able to ingest these test COMPOUND OBJECT sample objects?
    * Covered by compound.feature: "Ingest Compound Object Sample Object"
* 3-41	Able to view a COMPOUND OBJECT object?
    * This is theme specific.
* 3-42	Able to download a COMPOUND OBJECT object?
    * This is theme specific.
* 3-43	Able to search for newly ingested COMPOUND OBJECT object using Islandora simple search?
    * Covered by compound.feature: "Check for Compound Object Objects using simple search"
* 3-44	Able to edit COMPOUND OBJECT object’s title using the XML form?
    * Covered by compound.feature: "Edit Compound Object object title"
* 3-45	Able to search for newly edited COMPOUND OBJECT object’s title using Islandora simple search?
    * Covered by compound.feature: "Edit Compound Object object title"
* 3-46	Able to edit the Item Label of an COMPOUND OBJECT object's Properties?
    * Covered by compound.feature: "Edit Compound Object object Item Label"
* 3-47	Able to search for newly edited Item Label of an COMPOUND OBJECT object's Properties using Islandora simple search?
    * Covered by compound.feature: "Edit Compound Object object Item Label"
* 3-48	Able to edit MODS datastream for COMPOUND OBJECT object?
    * Covered by compound.feature: "Replace MODS datastream for Compound Object Object"
* 3-49	Able to search for newly edited MODS datastream for COMPOUND OBJECT object using Islandora simple search?
    * Covered by compound.feature: "Replace MODS datastream for Compound Object Object
* 3-50	Able to delete TN derivative for COMPOUND OBJECT object?
    * Covered by compound.feature: "Delete TN derivative for Compound Object Object"
* 3-51	Able to regenerate all derivatives for COMPOUND OBJECT object?
    * WONTFIX- Does not appear to be possible as derivatives are tied to child objects
    * Testing regen derivs is already covered in the other content types that appear as children of compounds
* Testing viewers
    * WONTFIX- this is theme dependent. All we do is ingest and test relationships and data integrity

##### Large Image Content Model
* 3-52	Able to ingest these test LARGE IMAGE sample objects?
    * Covered by largeimage.feature: "Ingest Large Image Sample Object"
* 3-53	Able to view a LARGE IMAGE object?
    * Covered by largeimage.feature: "Check for Large Image OBJ download"
* 3-54	Able to download a LARGE IMAGE object?
    * Covered by largeimage.feature: "Check for Large Image OBJ download"
* 3-55	Able to search for newly ingested LARGE IMAGE object using Islandora simple search?
    * Covered by largeimage.feature: "Check for Large Image Objects using simple search"
* 3-56	Able to edit LARGE IMAGE object’s title using the XML form?
    * Covered by largeimage.feature: "Edit Large Image object title"
* 3-57	Able to search for newly edited  LARGE IMAGE object’s title using Islandora simple search?
    * Covered by largeimage.feature: "Edit Large Image object title"
* 3-58	Able to edit the Item Label of an LARGE IMAGE object's Properties?
    * Covered by largeimage.feature: "Edit Large Image object Item Label"
* 3-59	Able to search for newly edited Item Label of an LARGE IMAGE object's Properties using Islandora simple search?
    * Covered by largeimage.feature: "Edit Large Image object Item Label"
* 3-60	Able to edit MODS datastream for LARGE IMAGE object?
    * Covered by largeimage.feature: "Replace MODS datastream for Large Image Object"
* 3-61	Able to search for newly edited MODS datastream for LARGE IMAGE object using Islandora simple search?
    * Covered by largeimage.feature: "Replace MODS datastream for Large Image Object"
* 3-62	Able to delete TN derivative for LARGE IMAGE object?
    * Covered by largeimage.feature: "Delete TN derivative for Large Image Object"
* 3-63	Able to regenerate all derivatives for LARGE IMAGE object?
    * Covered by largeimage.feature: "Regenerate all derivatives for Large Image Object"

##### Newspaper Content Model
* 3-64	Able to ingest these test NEWSPAPER sample objects?
    * Covered by newspaper.feature: "Ingest Newspaper Sample Object"
* 3-65	Able to view a NEWSPAPER object?
    * Covered by newspaper.feature: "Check for Newspaper OBJ download"
* 3-66	Able to download a NEWSPAPER object?
    * Covered by newspaper.feature: "Check for Newspaper OBJ download"
* 3-67	Able to search for newly ingested NEWSPAPER object using Islandora simple search?
    * Covered by newspaper.feature: "Check for Newspaper Objects using simple search"
* 3-68	Able to edit NEWSPAPER object’s title using the XML form?
    * Covered by newspaper.feature: "Edit Newspaper object title"
* 3-69	Able to search for newly edited NEWSPAPER object’s title using Islandora simple search?
    * Covered by newspaper.feature: "Edit Newspaper object title"
* 3-70	Able to edit the Item Label of an NEWSPAPER object's Properties?
    * Covered by newspaper.feature: "Edit Newspaper object Item Label"
* 3-71	Able to search for newly edited Item Label of an NEWSPAPER object's Properties using Islandora simple search?
    * Covered by newspaper.feature: "Edit Newspaper object Item Label"
* 3-72	Able to edit MODS datastream for NEWSPAPER object?
    * Covered by newspaper.feature: "Replace MODS datastream for Newspaper Object"
* 3-73	Able to search for newly edited MODS datastream for NEWSPAPER object using Islandora simple search?
    * Covered by newspaper.feature: "Replace MODS datastream for Newspaper Object"
* 3-74	Able to delete TN derivative for NEWSPAPER object?
    * Covered by newspaper.feature: "Delete TN derivative for Newspaper Object"
* 3-75	Able to regenerate all derivatives for NEWSPAPER object?
    * Covered by newspaper.feature: "Regenerate all derivatives for Newspaper Object"

##### Oral History Content Model
* 3-76	Able to ingest these test ORAL HISTORY sample objects?
    * Covered by oralhistories.feature: "Ingest ORAL HISTORIES Sample Object"
* 3-77	Able to view a ORAL HISTORY object?
    * Covered by oralhistories.feature: "Check for ORAL HISTORIES OBJ download"
* 3-78	Able to download a ORAL HISTORY object?
    * Covered by oralhistories.feature: "Check for ORAL HISTORIES OBJ download"
* 3-79	Able to search for newly ingested ORAL HISTORY object using Islandora simple search?
    * Covered by oralhistories.feature: "Check for ORAL HISTORIES Objects using simple search"
* 3-80	Able to edit ORAL HISTORY object’s title using the XML form?
    * Covered by oralhistories.feature: "Edit ORAL HISTORIES object title"
* 3-81	Able to search for newly edited ORAL HISTORY object’s title using Islandora simple search?
    * Covered by oralhistories.feature: "Edit ORAL HISTORIES object title"
* 3-82	Able to edit the Item Label of an ORAL HISTORY object's Properties?
    * Covered by oralhistories.feature: "Edit ORAL HISTORIES object Item Label"
* 3-83	Able to search for newly edited Item Label of an ORAL HISTORY object's Properties using Islandora simple search?
    * Covered by oralhistories.feature: "Edit ORAL HISTORIES object Item Label"
* 3-84	Able to edit MODS datastream for ORAL HISTORY object?
    * Covered by oralhistories.feature: "Replace MODS datastream for ORAL HISTORIES Object"
* 3-85	Able to search for newly edited MODS datastream for ORAL HISTORY object using Islandora simple search?
    * Covered by oralhistories.feature: "Replace MODS datastream for ORAL HISTORIES Object"
* 3-86	Able to delete TN derivative for ORAL HISTORY object?
    * Covered by oralhistories.feature: "Delete TN derivative for ORAL HISTORIES Object"
* 3-87	Able to regenerate all derivatives for ORAL HISTORY object?
    * Covered by oralhistories.feature: "Regenerate all derivatives for ORAL HISTORIES Object"

##### PDF Content Model
* 3-88	Able to ingest these test PDF sample objects?
    * Covered by pdf.feature: "Ingest PDF Sample Object"
* 3-89	Able to view a PDF object?
    * Covered by pdf.feature: "Check for PDF OBJ download"
* 3-90	Able to download a PDF object?
    * Covered by pdf.feature: "Check for PDF OBJ download"
* 3-91	Able to search for newly ingested PDF object using Islandora simple search?
    * Covered by pdf.feature: "Check for PDF Objects using simple search"
* 3-92	Able to edit PDF object’s title using the XML form?
    * Covered by pdf.feature: "Edit PDF object title"
* 3-93	Able to search for newly edited PDF object’s title using Islandora simple search?
    * Covered by pdf.feature: "Edit PDF object title"
* 3-94	Able to edit the Item Label of an PDF object's Properties?
    * Covered by pdf.feature: "Edit PDF object Item Label"
* 3-95	Able to search for newly edited Item Label of an PDF object's Properties using Islandora simple search?
    * Covered by pdf.feature: "Edit PDF object Item Label"
* 3-96	Able to edit MODS datastream for PDF object?
    * Covered by pdf.feature: "Replace MODS datastream for PDF Object"
* 3-97	Able to search for newly edited MODS datastream for PDF object using Islandora simple search?
    * Covered by pdf.feature: "Replace MODS datastream for PDF Object"
* 3-98	Able to delete TN derivative for PDF object?
    * Covered by pdf.feature: "Delete TN derivative for PDF Object"
* 3-99	Able to regenerate all derivatives for PDF object?
    * Covered by pdf.feature: "Regenerate all derivatives for PDF Object"

##### Video Content Model
* 3-100	Able to ingest these test VIDEO sample objects?
    * Covered by video.feature: "Ingest Video Sample Object"
* 3-101	Able to view a VIDEO object?
    * Covered by video.feature: "Check for Video OBJ download"
* 3-102	Able to download a VIDEO object?
    * Covered by video.feature: "Check for Video OBJ download"
* 3-103	Able to search for newly ingested VIDEO object using Islandora simple search?
    * Covered by video.feature: "Check for Video Objects using simple search"
* 3-104	Able to edit VIDEO object’s title using the XML form?
    * Covered by video.feature: "Edit Video object title"
* 3-105	Able to search for newly edited VIDEO object’s title using Islandora simple search?
    * Covered by video.feature: "Edit Video object title"
* 3-106	Able to edit the Item Label of an VIDEO object's Properties?
    * Covered by video.feature: "Edit Video object Item Label"
* 3-107	Able to search for newly edited Item Label of an VIDEO object's Properties using Islandora simple search?
    * Covered by video.feature: "Edit Video object Item Label"
* 3-108	Able to edit MODS datastream for VIDEO object?
    * Covered by video.feature: "Replace MODS datastream for Video Object"
* 3-109	Able to search for newly edited MODS datastream for VIDEO object using Islandora simple search?
    * Covered by video.feature: "Replace MODS datastream for Video Object"
* 3-110	Able to replace MODS datastreams
    * Covered by video.feature: "Replace MODS datastream for Video Object"
* 3-111	Able to search for newly replaced MODS datastreams using Islandora simple search?
    * Covered by video.feature: "Replace MODS datastream for Video Object"
* 3-112	Able to delete TN derivative for VIDEO object?
    * Covered by video.feature: "Delete TN derivative for Video Object"
* 3-113	Able to regenerate all derivatives for VIDEO object?
    * Covered by video.feature: "Regenerate all derivatives for Video Object"

##### WARC Content Model
* 3-114	Able to ingest these test WEB ARCHIVE sample objects?
    * Covered by warc.feature: "Ingest WARC Sample Object"
* 3-115	Able to view a WEB ARCHIVE object?
    * N/A There is no WARC viewer
* 3-116	Able to download a WEB ARCHIVE object?
    * Covered by warc.feature: "Check for WARC OBJ download"
* 3-117	Able to search for newly ingested WEB ARCHIVE object using Islandora simple search?
    * Covered by warc.feature: "Check for WARC Objects using simple search"
* 3-118	Able to edit WEB ARCHIVE object’s title using the XML form?
    * Covered by warc.feature: "Edit WARC object title"
* 3-119	Able to search for newly edited WEB ARCHIVE object’s title using Islandora simple search?
    * Covered by warc.feature: "Edit WARC object title"
* 3-120	Able to edit the Item Label of an WEB ARCHIVE object's Properties?
    * Covered by warc.feature: "Edit WARC object Item Label"
* 3-121	Able to search for newly edited Item Label of an WEB ARCHIVE object's Properties using Islandora simple search?
    * Covered by warc.feature: "Edit WARC object Item Label"
* 3-122	Able to edit MODS datastream for WEB ARCHIVE object?
    * Covered by warc.feature: "Replace MODS datastream for WARC Object"
* 3-123	Able to search for newly edited MODS datastream for WEB ARCHIVE object using Islandora simple search?
    * Covered by warc.feature: "Replace MODS datastream for WARC Object"
* 3-124	Able to delete TN derivative for WEB ARCHIVE object?
    * Covered by warc.feature: "Delete TN derivative for WARC Object"
* 3-125	Able to regenerate all derivatives for WEB ARCHIVE object?
    * Covered by warc.feature: "Regenerate all derivatives for WARC Object"

##### MAINTENANCE TASKS
* 4-1	Able to view the expected number of data objects?
    * N/A
* 4-2	Able to run Drupal cron manuall as signed in user?
    * Covered by )-sile-drupal.feature: "Run cron"
* 4-3	Confirm that Drupal cron runs automatically as scheduled?
    * N/A
* 4-4	Able to view / tail logs from within containers
    * N/A
* 4-5	Able to view / tail logs on Host server?
    * N/A
* 4-6	Able to run a Solr search from website and find ingested objects?
    * Covered above
* 4-7	Able to reindex / rebuild Fedora RI?
    *
* 4-8	Able to reindex / rebuild Fedora db MYSQL?
* 4-9	Able to reindex SOLR?

##### Additional Testing
* 5-1	Able to create single host server with multi-sites (link to Ben's instructions)
    * WONTFIX - ISLE doesn't support this anymore
* 5-2	Able to use mounts instead of Docker
    * N/A - To be covered by one or more CI configurations
* 5-3	Able to create new config directories for services
    * N/A - To be covered by one or more CI configurations

#### FEDORA - Able to overide existing settings for:
* 5-4	      - ./config/fedora/akubra-llstore.xml:/usr/local/fedora/server/config/spring/akubra-llstore.xml (Allows for deeper hash directories for larger Fedora collections)
    * N/A - To be covered by one or more CI configurations
* 5-5	foxmltoSolr.xslt
    * N/A - To be covered by one or more CI configurations
* 5-6	islandora_transforms
    * N/A - To be covered by one or more CI configurations

#### SOLR - Able to overide existing settings for:
* 5-7	schema.xml
    * N/A - To be covered by one or more CI configurations
* 5-8	solrconfig.xml
    * N/A - To be covered by one or more CI configurations
* 5-9 	Use IMI for sample ingest testing to speed things up
    * N/A
* 5-10	Test bind mounting - with nested ancestors set to true foxmltoSolr.xslt and all islandora_transforms, along with solrconfig and schemas
    * N/A - To be covered by one or more CI configurations

---

## Additional Resources
*
*

---
