# Server / Host Requirements

Your ISLE **host server** can be a virtual machine (VM) on your desktop or on the cloud, it can also be a physical server. An SSH connection with a user that has root or admin privileges is necessary. If an IT department is doing this initial setup, see [this page](../07_appendices/sample-it-department-request.md) for sample language to use in requesting the server.

## Know Working Operating Systems / Distributions
- Windows 10 (only suggested for testing)
- MacOS
- Ubuntu 16.04 LTS
- Ubuntu 18.04 LTS
- CentOS 7.x
- RHEL*
_*Currently RHEL does not support Docker Community Edition (CE), EE can be used._

## Production

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

## Staging

Below are the recommended minimum specifications for a staging server. The server can be a local physical server, a VM, or on a hosted platform such as Amazon's AWS or Google's GCP.  

* Ubuntu 18.04 LTS or CentOS 7.x running on a server or VM
* Minimum 2 CPU's (w/ 1 - 4 cores each)
* 8 - 16 GB of RAM is recommended
* Sufficient HD or attached storage to hold your collection
* 30 - 50GB for the Server OS & overhead

## Development

Below are the recommended minimum specifications for a development server. The server can be a local physical server, a VM, or on a hosted platform such as Amazon's AWS or Google's GCP.  

* Ubuntu 18.04 LTS or CentOS 7.x running on a server or VM
* Minimum 2 CPU's (w/ 1 - 4 cores each)
* 8 - 16 GB of RAM is recommended
* Sufficient HD or attached storage to hold your collection
* 30 - 50GB for the Server OS & overhead

## Testing / Playground

Below are the recommended specifications for a laptop or desktop running a TEST version of ISLE:

* Bring your own OS.
* Minimum of 2 CPU cores
* 8 - 16 GB of RAM is recommended
* 128 - 500GB for the Desktop OS
* Sufficient HD or attached storage to hold a small test collection (depending on your testing ~5-10GB for objects and their derivatives)
