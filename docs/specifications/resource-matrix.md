This document is a suggested guide for allocating ISLE host resources based on your ISLE environment.

* **Please note:** These are recommended defaults which sometimes include a range. Your needs may require more resources to be added. It is **not recommended** to go below the defaults for the Staging and Production environments.

* For the Production and Staging storage needs, ISLE Endusers should plan on these numbers increasing over time as their collections grow.

* It is generally recommended that Staging and Production mirror each other in size.

---

## Resource matrix by ISLE environment / system

| Resource  | Demo  | Local | Test | Staging    | Production   |
| ---       | ---   | ---   |  --- | ---        | ---          |
| OS | Mac/Windows/Linux | Mac/Windows/Linux | Mac/Windows/Linux | Ubuntu / Centos | Ubuntu / Centos |
| RAM | 4 GB | 4 - 8+ GB | 4 GB | 16 GB | 16 - 32+ GB |
| CPU | 1 | 1 - 2 | 1 - 2 | 2 - 4 | 2 - 4+ |
| Storage - OS | 3 - 5+ GB | 6 - 10+ GB | 3 - 5+ GB | 50 - 100+ GB | 50 - 100+ GB |
| Storage - Fed data | 5 - 10+ GB | 5 - 10+ GB | 5 - 10+ GB | 3 - 6+ TB | 3 - 6+ TB |

* _For Fed data storage_, it is recommended that a minimum overhead of about `100 - 250 GB` of free space is kept on the disk so that Fedora's ActiveMQ system doesn't shutdown due to a lack of space. You might be able to get away with less overhead.
* _If using Blazegraph_, add an additional mandatory minimum of `4 GB` to any system's RAM resource requirements to the overall total.
* _If using Varnish_, you may find the system will be more performant if you increase the system memory by an additional `1 -2 GB` of memory to the overall total.
* _For Production and Staging ISLE host servers_, a minimum of an additional `4 - 8` GB should be used for a swapfile.

* KEY
  * `OS` = Operating System
  * `RAM` = Memory
  * `CPU` = Processor(s) / Cores
  * `Fed data` = Fedora datastreamStore (/mnt/data/fedora/datastreamStore) storage
  * `Storage - OS` includes the Docker volumes that store additional data

---

### Memory allocation and tuning concepts

The tuning and configuration of ISLE can vary based on system resources and traffic, it is recommended that you start out with the default memory allocations and test the results as you increase or decrease in key areas.

| Container         | Ranges    | Demo      | Local     | Test      | Staging   | Production    |
| ---               | ---       | ---       | ---       |  ---      | ---       | ---           |
| isle-fedora       | MIN       | 128M      | 128M      | 128M      | 1024M     | 1024M         |
|                   | MAX       | 768M      | 768M      | 768M      | 4096M     | 4096M         |
| isle-solr         | MIN       | 0         | 0         | 0         | 1024M     | 1024M         |
|                   | MAX       | 512M      | 512M      | 512M      | 2048M     | 2048M         |
| isle-images       | MIN       | 0         | 0         | 0         | 512M      | 512M          |
|                   | MAX       | 512M      | 512M      | 512M      | 1024M     | 1024M         |
| isle-blazegraph   | MIN       | 1024M     | 1024M     | 1024M     | 4096M     | 4096M         |
|                   | MAX       | 4096M     | 4096M     | 4096M     | 6144M     | 6144M         |

* The remainder of free memory is assumed to be taken or used by the remaining ISLE containers e.g. `isle-apache` or `isle-mysql` as needed.
  * If you would like to specifically set memory by container which is **out of scope** of current ISLE setups and documentation, please read more in the [Docker-compose documentation](https://docs.docker.com/compose/compose-file/) under the `Resources` section or search by the term `mem_limit`.

* KEY
  * MAX = JAVA_MAX_MEM (_you'll adjust these settings in the docker-compose.*.yml file_)
  * MIN = JAVA_MIN_MEM (_you'll adjust these settings in the docker-compose.*.yml file_)

---

### High Volume Ingest

When planning for ingesting objects at a high volume and rate, it would be highly advisable to consider the following prior to starting any large ingest:

* As the Apache container will be under a high load to create derivatives:
  * Add a minimum of at least `4 GB` of memory to your Staging and Production systems.
  * If possible, add an additional `1 -2 CPUs` to your Staging and Production systems.

* Anticipate the size of the incoming objects or collections (data) to be ingested and double-check that you have storage capacity on your Staging and Production systems.
  * This can vary greatly depending on content types and size of objects to be ingested.
  * A good rule of thumb is to conservatively factor in that newly created derivatives and additional datastreams will take up to 1.25 to 1.5 x your storage space.  
    * For example if an ISLE enduser has `50 GB` of tiffs to be ingested, they should plan on having a minimum of `60 - 75 GB` of available storage space. This isn't always a hard and fast rule but will avoid situations where space runs out mid-ingest.

---

#### Varnish allocation for a Production system

* **Example configuration** for a Production ISLE host server using `16 GB` of memory.
  * Expect to allocate about 40 - 50% of the host server memory for all of the Java / Tomcat based images
    * `isle-fedora` should have the most e.g. `4096 MB` or `4 GB`
    * `isle-solr` should have the second most e.g. `2048 MB` or `2 GB`
    * `isle-imageservices` should have the third most e.g. `1024 MB` or `1 GB`
    * If using `isle-blazegraph`, this image should also be using a minimum of `4096 MB` or `4 GB` of memory
  * Keep about `2 -3 GB` free for the remaining images e.g. `isle-apache`, `isle-mysql` etc.
    * This would leave you roughly `1 - 2 GB` to allocate to the Varnish cache. Start with `256` to `512 MB` and work your way up as needed.
      * You can adjust the amount that Varnish puts into memory in the supplied `.env` file
        * On the line: `VARNISH_MALLOC=256m` you can change the amount of memory to a higher value other than the default `256` Megabytes. We recommend that you start with 1-2 GB for now to tune further as your resources and needs warrant.
  * There are additional potential memory allocation and tuning recommendations for Varnish from [Varnish-software](https://info.varnish-software.com/blog/understanding-varnish-cache-memory-usage)
  * If you need to read more about how to use or change Varnish's cache settings, please start [here](https://varnish-cache.org/docs/4.1/users-guide/storage-backends.html)

* **Recommendation** Adding more memory to the Production ISLE host system from the default recommended `16 GB` might be recommended here if running all optional components e.g. `isle-varnish`,  `isle-blazegraph`, the TICK stack, etc.