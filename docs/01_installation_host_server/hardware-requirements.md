# Hardware Requirements

The ISLE **host server** can be a laptop or workstation, physical server, virtual machine (VM) or cloud service. An SSH connection with a user that has root or admin privileges is necessary. If an IT department is providing the server environment, please see the [Sample IT Department Request Letter](../07_appendices/sample-it-department-request.md) for sample language to use in requesting the server.

## Known Working Operating Systems / Distributions
- Ubuntu 18.04 LTS
- Ubuntu 16.04 LTS
- CentOS 7.x
- MacOS
- Windows 10 (for Demo ISLE Site only)
- Red Hat Enterprise Linux (RHEL) -
*Please read this [Warning about Installing on Redhat](../07_appendices/redhat.md). Currently RHEL does not support Docker Community Edition (CE), although Docker Enterprise Edition (Docker EE) can be used.*

## Production Server

Below are the recommended minimum specifications for a production server. The server can be a local physical server, a VM, or on a hosted platform such as Amazon's AWS or Google's GCP.  

* Ubuntu 18.04 LTS or CentOS 7.x running on a server or VM
* Minimum 2 CPU's (w/ 1 - 4 cores each)
* 16 - 32 GB of RAM is recommended
* Sufficient HD or attached storage to hold your collection
* 30 - 50GB for the Server OS & overhead

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

## Staging Server

Below are the recommended minimum specifications for a staging server. The server can be a local physical server, a VM, or on a hosted platform such as Amazon's AWS or Google's GCP.  

* Ubuntu 18.04 LTS or CentOS 7.x running on a server or VM
* Minimum 2 CPU's (w/ 1 - 4 cores each)
* 8 - 16 GB of RAM is recommended
* Sufficient HD or attached storage to hold your collection
* 30 - 50GB for the Server OS & overhead

## Development Server

Below are the recommended minimum specifications for a development server. The server can be a local physical server, a VM, or on a hosted platform such as Amazon's AWS or Google's GCP.  

* Ubuntu 18.04 LTS or CentOS 7.x running on a server or VM
* Minimum 2 CPU's (w/ 1 - 4 cores each)
* 8 - 16 GB of RAM is recommended
* Sufficient HD or attached storage to hold your collection
* 30 - 50GB for the Server OS & overhead

## Demo ISLE Site

Below are the recommended specifications for a laptop or desktop running a TEST version of ISLE:

* Bring your own OS.
* Minimum of 2 CPU cores
* 8 - 16 GB of RAM is recommended
* 128 - 500GB for the Desktop OS
* Sufficient HD or attached storage to hold a small test collection (depending on your testing ~5-10GB for objects and their derivatives)
