# ISLE Release 1.1.0

* Base Images to serve them all:
    * In Docker fashion two new base-images were created for the 1.1 ISLE stack. There are major benefits both for the committers and users of ISLE.
    * Keeping our users safe has never been easier.  For users and committers alike when a security update or patch arrives we (the ISLE committers) need only update the base image.  By virtue of inheritance these updates are automatically inherited and built into each and every component of the ISLE stack.
    * Quick downloads, faster launch: these shared base images decrease the time to pull and launch the ISLE stack.  Since the images have the same layers (like a cake) they are downloaded only once and reused as building blocks for the rest of the ISLE stack.
    * This also significantly decreases the overall size of all images because we are able to finely tune and trim our base images so they’re stable, without being bloated by unneeded components.
        * [ISLE-Ubuntu-Base](https://github.com/Islandora-Collaboration-Group/isle-ubuntu-basebox)
        * [ISLE-Tomcat-Base](https://github.com/Islandora-Collaboration-Group/isle-tomcat)
* Image-Services
    * Image Services with an `S`. ImageServices is a 1.2 GB container that simultaneously runs both Adore-Djatoka and [Cantaloupe IIIF](https://medusa-project.github.io/cantaloupe/).  
    * By default the ISLE-Installer prefers Cantaloupe IIIF over AD, but AD remains a component for backward compatibility to be deprecated in a future release.
* ISLE-Apache
    * Automatically pull the newest build tools on launch, never be with an outdated stack.  By default the Apache container will connect to GitHub and pull the ISLE-build.  This is controlled by an environment variable and can be disabled.
    * In future releases we intend to make it so that you can fork our build tools, or make your own, and set your own repository for the container to fetch on boot.
        * Note that while currently running the build tools is only done once we’re thinking ahead. In future planned iterations of ISLE-Apache the build tools will separate core Drupal code (`core`) from the Islandora modules (`contrib`).  TL;DR: We hope to bring automated Drupal core updates, as well as offer an option to update Islandora modules -- though we acknowledge this might not be for everyone!
* ImageMagick 7.x (ISLE images involved: ImageServices and Apache)
    * ImageMagick, the tool which converts and handles derivative generation (in most part) has been updated to the latest 7.x branch and is compiled with JP2 read and write support from [OpenJPEG](http://www.openjpeg.org/).
    * This update also includes the PHP Imagick extension which is now built from source to support the new version of ImageMagick with JP2 support.
* ISLE-Proxy is now magiek
    * We have replaced NGiNX with [Traefik](https://traefik.io/)
    * Traefik is a modern reverse proxy with several key benefits
        * Institutions and users alike can use ACME for the automatic and free creation and maintenance of SSL certificates from Let's Encrypt (https://letsencrypt.org/)
* Added [Portainer](https://portainer.io), a visual dashboard of your running stack(s)
    * Provides quick access to logs, shells, and startup/shutdown functions for running containers.
* Logging
    * Services are now logged out to Docker while also being stored on the containers.
    * In case we’ve missed anything all uncaught logs are still logged by S6 overlay.
* .env for your Environment
    * Lorem Ipsum
* Muli-Environment (e.g., dev, stage, prod) in a greatly simplified manner
    * Repo for multi-env incoming
