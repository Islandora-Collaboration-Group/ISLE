## Example Server Implementations

This is a place to share details about individual institutions' ISLE implementations.

### [Colgate University's Digital Collections](https://digitalcollections.colgate.edu)

- Collection Size: Approx. 5 TB, over 115,000 objects.
- Hosted on AWS EC2
    - Production server:
        - m4.xlarge instance type: 
            - 4 cores
            - 16 GB RAM
            - 75 GB EBS type gp2 storage for OS and docker images
            - 8 TB EBS type st1 storage for fedorastore
            - 300 GB EBS type gp2 storage for temporary ingest files
    - Staging server:
        - m4.large instance type:
            - 2 cores
            - 8 GB RAM
            - 75 GB EBS type gp2 storage for OS and docker images
            - 8 TB EBS type st1 storage for fedorastore

**Performance:** The production environment performs adequately for public users viewing the collection.  Ingest speed is acceptable, but we are only adding to the collections on a very small scale.  Staging performs fine as a testing platform for ISLE updates, etc, before pushing to production, but is noticeably slower.  I would not suggest m4.large for instutions planning to ingest a lot into staging after initial set-up.  Amazon's newer m5.large may be better for that, but should be tested.

**Note:** Step by step instructions for provisioning the 300 GB ingest storage in the AWS console and configuring docker to mount it for Islandora are available in [this cookbook recipe.](../cookbook-recipes/example-aws-configuration.md)


If you are using ISLE in production, please consider sharing your institution's set up to help others assess their options and plan for their own implementation.  To add your ISLE information here, either issue a PR or put the information in an Issue and the documentation team will add it.