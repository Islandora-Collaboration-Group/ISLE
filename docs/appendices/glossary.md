<!-- Glossary of Terms and Variables -->
**Glossary Topics**

- [ISLE](#isle)
- [Docker](#docker)
- [Documentation / Development](#documentation-development)
- [Solr](#solr)
- [Systems](#systems)
- [Virtualization](#virtualization)

---

## ISLE

* **ISLE**: In simple terms, ISLE is a set of resources that allows one to build a fully functioning Islandora system fairly quickly using a system building tool called [Docker](https://docker.com).

## Docker

* **Containers**: [Docker containers](https://www.docker.com/what-container) are lightweight, stand-alone, executable packages of a piece of software that includes everything needed to run it: code, runtime, system tools, system libraries, settings. Each major component of Islandora runs in its own container.

* **Docker**:  used to build ISLE - [Docker](https://www.docker.com/what-docker) is used to create containers (similar to virtual servers) based on pre-built images. A "recipe" file called docker-compose.yml orchestrates the setting up and networking of the containers.

* **Dockerfile**: Docker can build images automatically by reading the instructions from a [Dockerfile](https://docs.docker.com/engine/reference/builder/). A Dockerfile is a text document that contains all the commands a user could call on the command line to assemble an image. Using docker build users can create an automated build that executes several command-line instructions in succession.

* **Docker for Mac**: [Docker for Mac](https://www.docker.com/docker-mac) is an easy-to-install desktop app for building, debugging and testing Dockerized apps on a Mac. Docker for Mac is a complete development environment deeply integrated with the MacOS Hypervisor framework, networking and filesystem

* **Dockerhub**: [website / repository](https://hub.docker.com/website/repository) that provides access to the latest Docker images for the ISLE containers.

* **Host Server**: Also called "the host" - this is the base computer upon which the entire ISLE stack is built - this can be a virtual machine on a personal computer (LOCAL), or
a server you connected to via ssh (REMOTE).

* **Images**: [Docker images](https://docs.docker.com/engine/reference/commandline/images/) - source for the containers - these are built with installed software and updated by ISLE developers and stored on Dockerhub. You can see what software is used in each of the image's Dockerfiles e.g. `apache/Dockerfile`, etc. This will contain additional information on official Docker images used as a Docker [base image](https://docs.docker.com/develop/develop-images/baseimages/)

* **ISLE on GitHub**: the ISLE repository on github.com contains documentation and configuration files necessary to build ISLE.

* **Network** refers to a defined Docker network that is controlled by docker. This has powerful implications in production.
    * ISLE services: `fedora`, `solr`, `apache`, `mysql`, and `proxy` communicate using an internal private stack network. The service proxy also joins an insecure network that is accessible to the WAN (or for testing "WAN" likely means a smaller internal network). Why two networks? Swarms, scaling, replicating.

* **Volume** a Docker controlled place to hold data on the local file system. Used to persist data across containers.

---

## Documentation / Development

* **Git** - A free and open source distributed software version control system designed to handle everything from small to very large projects with speed and efficiency. Git is easy to learn and has a tiny footprint with lightning fast performance.
    * [https://git-scm.com/](https://git-scm.com/)
    * The ISLE project and its documentation is hosted by an online git service called [Github.com](https://github.com/)

* **Git branches** - Branching means you diverge from the main line of development and continue to do work without messing with that main line. [Learn more about branching here](https://git-scm.com/book/en/v2/Git-Branching-Branches-in-a-Nutshell)

* **Markdown** is a lightweight markup language with plain text formatting syntax. Markdown allows you to write using an easy-to-read, easy-to-write plain text format, then convert it to structurally valid XHTML (or HTML).
    * [https://en.wikipedia.org/wiki/Markdown](https://en.wikipedia.org/wiki/Markdown)
    * [https://daringfireball.net/projects/markdown/syntax](https://daringfireball.net/projects/markdown/syntax)
    * [http://kirkstrobeck.github.io/whatismarkdown.com/](http://kirkstrobeck.github.io/whatismarkdown.com/)
    * [https://help.github.com/categories/writing-on-github/](https://help.github.com/categories/writing-on-github/)

* **Terminal (CLI)** - A command-line interface or command language interpreter (CLI), also known as command-line user interface, console user interface[1] and character user interface (CUI), is a means of interacting with a computer program where the user (or client) issues commands to the program in the form of successive lines of text (command lines). A program which handles the interface is called a command language interpreter or shell.

* **TL;DR** - "Too long; didn't read." - used to indicate a large post, article, etc. that has a brief summary of said post, article, etc. as it might be too long to read.

* **YAML** (YAML Ain't Markup Language) is a human-readable data serialization language. It is commonly used for configuration files, but could be used in many applications where data is being stored (e.g. debugging output) or transmitted (e.g. document headers).
    * [https://en.wikipedia.org/wiki/YAML](https://en.wikipedia.org/wiki/YAML)
    * [http://yaml.org/](http://yaml.org/)

---

## Solr

**Please note:** Online versions of the SOLR documentation are for 6.6+ or higher, ISLE currently users version 4.10.4 only.

Solr - an open source enterprise search platform with the such features as full-text search, hit highlighting, faceted search, real-time indexing, dynamic clustering, database integration, NoSQL features[1] and rich document (e.g., Word, PDF) handling. Providing distributed search and index replication, Solr is designed for scalability and fault tolerance.

* [Learn more about Solr here](https://en.wikipedia.org/wiki/Apache_Solr)

* [Learn more about using Solr 4.10.4](https://archive.apache.org/dist/lucene/solr/ref-guide/apache-solr-ref-guide-4.10.pdf)

* `Core` - In Solr, the term core is used to refer to a single index and associated transaction log and configuration files (including the solrconfig.xml and Schema files, among others). Your Solr installation can have multiple cores if needed, which allows you to index data with different structures in the same server, and maintain more control over how your data is presented to different audiences.

* `schema.xml` - Solr organizes its data into documents, which consist of fields. The schema.xml file contains information about these fields, how they are analyzed and then filtered during searches. Different field types can contain different types of data so Solr uses the schema.xml file to determine how to build indexes from these input documents, and how to perform index and query time processing.

* `solr.xml` - The solr.xml file specifies configuration options for each Solr core, including configuration options for multiple cores. The file also contains mappings for request URLs, and indicates which cores to load when the server starts.

* `solrconfig.xml` - The solrconfig.xml file is the configuration file with the most parameters affecting Solr itself. In solrconfig.xml, you configure important features such as:

    * request handlers, which process the requests to Solr, such as requests to add documents to the index or requests to return results for a query listeners

    * processes that "listen" for particular query-related events. Listeners can be used to trigger the execution of special code, such as invoking some common queries to warm-up caches

    * and much more...

* `stopwords.txt` - The stopwords.txt file is a configuration file that lists the words to be used by the Solr stop filter to be excluded from appearing or being used for searches. Examples of these common words are a large list that can include "a", "the", or "is" which in a page could easily appear hundreds of times or more. When a user performs a search, an algorithm calculates the relevancy of any document in the search index by counting the number of times a word appears in the text being searched and since words like "the" have no bearing on a document's actual relevancy; they are then excluded from this ranking algorithm's results.

---

## Systems

* **cronjob** - The software utility cron is a time-based job scheduler in Unix-like computer operating systems. People who set up and maintain software environments use cron to schedule jobs (commands or shell scripts) to run periodically at fixed times, dates, or intervals. It typically automates system maintenance or administration [Learn more here](https://en.wikipedia.org/wiki/Cron). This is used by such utilities like [tmpreaper](http://manpages.ubuntu.com/manpages/trusty/man8/tmpreaper.8.html) and the Drupal site on the `apache` container.

* **domain** - A domain name is your website name for the IP address where Internet users can access your website. A domain name is used for finding and identifying computers on the Internet. Computers use IP addresses, which are a series of numbers. However, it is difficult for humans to remember strings of numbers. Because of this, domain names were developed and used to identify entities on the Internet rather than using IP addresses. [Learn more about domains here](https://en.wikipedia.org/wiki/Domain_name) or [here](https://en.wikipedia.org/wiki/Fully_qualified_domain_name)

* **dhparam** - A script to create this file can be found in the `/config/isle-sample/proxy/config/ssl-certs` directory. This file is used in conjunction with the nginx reverse proxy on the `proxy` container to further secure the communication / traffic between the external network (ISLE Host) and the internal ISLE network (other four containers.) The Diffie–Hellman key exchange is an alternative to RSA-based key exchange using an ephemeral but slower Diffie-Hellman algorithm which generates session keys in such a way that only the two parties involved in the communication can obtain them. No one else can, even if they have access to the server's private key. After the session is complete, and both parties destroy the session keys, the only way to decrypt the communication is to break the session keys themselves. This protocol feature is known as Forward Secrecy. [Learn more about Diffie-Hellman parameters](https://en.wikipedia.org/wiki/Diffie%E2%80%93Hellman_key_exchange)

* **environment** - An ISLE environment can be a production, development or staging environment. However ISLE is flexible and can be named anything using any convention of the end user's desire, one is not forced to do the following. Following a recommended workflow of "code up, data down" for Islandora development typically:

    * a **production** ISLE environment is a separate ISLE instance running all production (final and tested) potentially public facing code, themes, databases and a full amount of collection data or objects.

    * a **staging** ISLE environment is a separate ISLE instance running all running all pre-production (final and tested) private (non-public) facing code, themes, databases and a full amount of collection data or objects.
        * **staging** should be an exact mirror of production (RAM, CPU, storage etc) with the exception of new code to be reviewed and tested internally prior to pushing to production.

    * a **development** ISLE environment is a separate ISLE instance running all development (experimental and untested) private (non-public) facing code, themes, databases and a limited amount of collection data or objects.
        * **development** should not be a mirror of production and can be greatly limited and underpowered in its resource footprint e.g. less RAM, CPU, HD, etc.  

* **personal computer** - a personal computer represents a personal laptop or desk computer (workstation).

* **Root User** - the user name or account that by default has access to all commands and files on a Linux or other Unix-like operating system. It is also referred to as the root account, root, root user and the superuser. [Learn more about root user](http://www.linfo.org/root.html)

* **SSH** - is a cryptographic network protocol for operating network services securely over an unsecured network. The best known example application is for remote login to computer systems by users. [Learn more about SSH](https://en.wikipedia.org/wiki/Secure_Shell)

<!-- TODO: Review SSL certificates and cut out obsolete stuff -->
* **SSL certificates** - files that digitally bind a cryptographic key to an organization’s web domain (URL). To enable HTTPS on your website, you need to get a certificate (a type of file) from a Certificate Authority (CA). When installed on a web server, the browser will display a padlock icon and `https` will appear as the prefix to the URL to indicate a secure connection between the web server and user's browser. SSL certificates are used to secure credit card transactions, data transfer, logins, and more. 

    * Generate SSL certificates by using ONE of the following methods:

    * Ask your IT department to provision a complete [SSL Certificate Chain](https://support.dnsimple.com/articles/what-is-ssl-certificate-chain/). On your remote server ISLE Installation you will place the SSL certificates into the `./ISLE/config/proxy/ssl-certs` directory. These SSL certificates are used by the `apache` & `proxy` Docker containers.

    * Use [Let's Encrypt](https://letsencrypt.org/), a free, automated, and open Certificate Authority for generating SSL certificates for your ISLE environment. In order to get a certificate for your website’s domain from Let’s Encrypt, you have to demonstrate control over the domain. With Let’s Encrypt, you do this using software that uses the ACME protocol, which typically runs on your web host. To begin, visit the [Let's Encrypt: Free SSL Certificates](../appendices/configuring-lets-encrypt.md) ISLE guide.

    * Additional resources:
        * [Why SSL? The Purpose of using SSL Certificates](https://www.sslshopper.com/why-ssl-the-purpose-of-using-ssl-certificates.html)
        * [How to Configure Nginx SSL Certifcate Chain](https://futurestud.io/tutorials/how-to-configure-nginx-ssl-certifcate-chain)
        * The [SSLCertificateChainFile Directive](https://httpd.apache.org/docs/2.4/mod/mod_ssl.html#sslcertificatechainfile) directive sets the optional all-in-one file where you can assemble the certificates of Certification Authorities (CA) which form the certificate chain of the server certificate. (e.g. `/certs/sample-interm.pem` used as an example in later documentation. This file may be **optional** in some setups but is generally recommended for use by the `apache` container when available.)
        * The [SSLCertificateFile Directive](https://httpd.apache.org/docs/2.4/mod/mod_ssl.html#sslcertificatefile)	directive points to a file with certificate data in PEM format. At a minimum, the file must include an end-entity (leaf) certificate. (e.g. `/certs/sample.pem` used as an example in later documentation. This file is required in all setups.)
        * The [SSLCertificateKeyFile Directive](https://httpd.apache.org/docs/2.4/mod/mod_ssl.html#sslcertificatekeyfile) directive points to the PEM-encoded private key file for the server. If the contained private key is encrypted, the pass phrase dialog is forced at startup time. (e.g. `/certs/sample-key.pem` used as an example in later documentation. This file is required in all setups.)

* **sudo** - short for "substitute user do" and it allows the user to have root permissions when entered in front of a command. [Learn more about sudo](https://www.sudo.ws/intro.html)

---

## Virtualization

* **Vagrant**: [Vagrant](https://www.vagrantup.com/) provides easy to configure, reproducible, and portable work environments. Vagrant works on Mac, Linux, Windows, and more. Within the ISLE project there is a vagrant folder.

* **Vagrantfile**: [Vagrantfile](https://www.vagrantup.com/docs/vagrantfile/) used to describe the type of virtual machine required for the ISLE project, and how to configure and provision these virtual  machines. Vagrantfiles are called Vagrantfiles because the actual literal filename for the file is Vagrantfile. Vagrant is meant to run with one Vagrantfile per project which allows endusers to check out the code, run vagrant up, and be on their way. Vagrantfiles are used primarily for use on personal computers running a Ubuntu or CentOS ISLE Host server.

* **Virtualbox**: [VirtualBox](https://www.virtualbox.org/wiki/Downloads) is a general-purpose full virtualizer for x86 hardware, targeted at server, desktop and embedded use.

* **VM**: Virtual Machine - A virtual machine is a software computer that, similar to a physical computer, runs an operating system and applications comprised of specification and configuration files backed by the resources of a host (the physical computer).

---
