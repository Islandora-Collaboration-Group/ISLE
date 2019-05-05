<!--- PAGE_TITLE --->

# Hardware Requirements

Your ISLE **host server** can be a virtual machine (VM) on your desktop or on the cloud, it can also be a physical server. An SSH connection with a user that has root or admin privileges is necessary. If an IT department is doing this initial setup, see [this page](../appendices/sample-it-department-request.md) for sample language to use in requesting the server.

## Known Working Operating Systems / Distributions
- Ubuntu 18.04 LTS
- Ubuntu 16.04 LTS
- CentOS 7.x
- MacOS
- Windows 10 (for Demo only)
- RHEL*
_*Currently RHEL does not support Docker Community Edition (CE), EE can be used._

## Production

Below are the recommended minimum specifications for a production server. The server can be a local or hosted physical server or virtual machine, or it can be a cloud hosted platform (AWS, GCP, etc.).  

* Ubuntu 18.04 LTS or CentOS 7.x running on a server or virtual machine
* Minimum 2 CPU's (with 1 - 4 cores each)
* 16 - 32 GB of RAM is recommended
* 30 - 50GB for the server OS and overhead
* Sufficient hard drive or attached storage to hold your collection

If you are **migrating** please ensure you have enough storage space for your entire collection or that you can access those data via a network or other file-sharing protocol.

- Open your docker-compose and modify the environment variables called JAVA_MAX_MEM and JAVA_MIN_MEM for fedora, solr, and image-services. Here are some recommendations for production instances:
```yaml
fedora:
  ...
  environment:
    - JAVA_MAX_MEM=2048M
    - JAVA_MIN_MEM=256M
...

solr:
  ...
  environment:
    - JAVA_MAX_MEM=2048M
    - JAVA_MIN_MEM=256M
...

image-services:
  ...
  environment:
    - JAVA_MAX_MEM=2048M
    - JAVA_MIN_MEM=128M
```

## Staging

Below are the recommended minimum specifications for a staging server. The server can be a local or hosted physical server or virtual machine, or it can be a cloud hosted platform (AWS, GCP, etc.).  

* Ubuntu 18.04 LTS or CentOS 7.x running on a server or virtual machine
* Minimum 2 CPU's (with 1 - 4 cores each)
* 8 - 16 GB of RAM is recommended
* 30 - 50GB for the server OS and overhead
* Sufficient hard drive or attached storage to hold your collection

## Development

Below are the recommended minimum specifications for a development server. The server can be a local or hosted physical server or virtual machine, or it can be a cloud hosted platform (AWS, GCP, etc.).  

* Ubuntu 18.04 LTS or CentOS 7.x running on a server or virtual machine
* Minimum 2 CPU's (with 1 - 4 cores each)
* 8 - 16 GB of RAM is recommended
* 30 - 50GB for the server OS and overhead
* Sufficient hard drive or attached storage to hold your collection

## Testing / Playground

Below are the recommended specifications for a laptop or desktop running a TEST version of ISLE:

* Bring your own OS.
* Minimum of 2 CPU cores
* 8 - 16 GB of RAM is recommended
* 128 - 500GB for the Desktop OS
* Sufficient hard drive or attached storage to hold a small test collection (depending on your testing ~5-10GB for objects and their derivatives)
