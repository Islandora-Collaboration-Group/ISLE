<!--- PAGE_TITLE --->

# Example AWS configuration

This is the current (as of October 2019) configuration used by Colgate University on AWS.  It's purpose is to provide one example of a working ISLE environment, or as a starting point for institutions with similar needs.

## Overview

Colgate University Libraries' Digital Collections currently holds over 115000 individual objects/pages.  The collection uses just under 5 Tb of storage, which includes high resolution TIFFs for the majority of the objects.

## AWS configuration

 - Production:
   - m4.xlarge EC2 Reserved Instance **Note**: have a 3 year contract for the m4.xlarge.  Amazon offers newer m5 instances for this tier which would be preferred.
   - 75 GB EBS storage, type gp2, for the operating system, docker images, etc.
   - 8 TB EBS storage, toe st1, for the Fedora datastore.  This is where all digital objects, derivatives, and metadata are stored.
    - 300 GB EBS storage, type gp2, used as a temporary holding location for objects to be ingested.  After successful ingest, the objects are deleted from this volume.  
 - Staging:
   - Staging differs from Production in 2 ways:
   - m4.large instance rather than xlarge, as performance is less of a concern on staging.  The system works, but can be sluggish compared to production.
   - No 300 GB holding location permanently attached.  This can be added fairly easily if a need to test a large ingest arose.

Rather than a separate 300 EBS volume, it would be possible to simply increase the size of the OS disk from 75 GB to something greater to allow room for object prior to ingestion.  However, having it separate provides a few advantages:
  - Volumes cannot be resized on the fly.  If temporary storage needs exceed what is available, the server would need to be shut down, a new volume created, and the existing volume copied over to it.  
  - If we are not ingesting anything for a period of time, we can easily turn off the 300 GB volume.  Because all data on it is meant to be temporary, we could delete it entirely to avoid being charged for storage we are not using.  A new volume could be quickly and easily re-attached as needed at a later date with minimal interruption to the production site.

## AWS Setup

Adding volumes in AWS is a fairly simple process, and is well documented [on their site](https://docs.aws.amazon.com/AWSEC2/latest/UserGuide/ebs-using-volumes.html)  Be sure to check the AWS site for current pricing.  EBS volumes are billed based on the size allotted, not used.

Colgate's ISLE host server' fstab has the following entry:

  >UUID=xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx  /mnt/tempstorage  xfs  defaults,nofail  0  2

which mounts the 300 GB volume at /mnt/tempstorage

In order for the Apache docker container to be able to access /mnt/tempstorage, it must be added as a bind mount.  If ISLE is currently running, issue `docker-compose down` to shut it down before making changes to the configuration file.

Colgate's docker-compose.production.yml contains the following line under the "apache" "volumes" section:

  >\- /mnt/tempstorage:/mnt/ingest

Note that either of these paths can point to any location as long as it is not otherwise in use (e.g do not bind this to /var/www/ on the docker container, as that directory already contains the Drupal files).  /mnt is commonly used as the default directory for mounting volumes in Linux.

Any files or directories added to /mnt/tempstorage/ on the host server will be immediately available to the Apache docker container aft er bringing ISLE back up with `docker-compose up -d`.

## Workflow

Colgate primarily uses the Islandora Multi Importer (IMI) module for ingesting new objects.  The basic workflow is as follows:

 - An archivist has a directory of recently scanned student newspapers in a directory on their computer
 - TIFFs are uploaded via SFTP to /mnt/tempstorage/studentnews
 - The metadata spreadsheet has a column for object location called "filepath" that refers to where Islandora will find it within the Docker container, e.g. /mnt/ingest/studentnews/page1.tif
 - In the IMI GUI, the "local" location is selected, and object is mapped to the spreadsheet column "filepath"
 - Files are ingested.  Upon completion and verification that the ingest was successful, the archivist deletes /studentnews subdirectory.  This can be done at any time after ingest so long as there is still capacity on the volume.  Because AWS charges for the GB allocated rather than used, there is no cost savings for deleting the files quickly.  Only deleting the volume entirely via the AWS console would avoid charges.

## Removing or resizing the ingest volume

**Note**: These instructions assume that the *only* data stored in the temporary volume is meant to be ephemeral and everything on it can be safely deleted.  

 - If no ingests are planned, the ingest volume can easily be removed to avoid unnecessary charges.
  - As always, bring the docker containers down with `docker-compose down` before editing docker-compose.production.yml
 - If you plan on provisioning the volume again later, comment out the lines added to  /etc/fstab and docker-compose.production.yml by adding a \# to the front.  ex:

  >\#UUID=xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx  /mnt/tempstorage  xfs  defaults,nofail  0  2

  And
  
  >\#\- /mnt/tempstorage:/mnt/ingest
 - Or delete both lines if you are sure you will not need them again.
 - Unmount the drive from the host server:
    - `sudo umount -d /mnt/tempstorage` replacing /mnt/tempstorage with the path you used on the host server.
 - See the AWS site for further instructions for [detaching](https://docs.aws.amazon.com/AWSEC2/latest/UserGuide/ebs-detaching-volume.html) then [deleting](https://docs.aws.amazon.com/AWSEC2/latest/UserGuide/ebs-deleting-volume.html) volumes.
