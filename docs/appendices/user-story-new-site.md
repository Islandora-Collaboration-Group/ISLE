# User Story: Building a New Islandora Site Using ISLE

Sam wants to spin up Islandora on an Ubuntu Linux server she has in the institutional data center. She accesses the server using ssh and determines that it meets the [Hardware Requirements](../install/host-hardware-requirements.md) (basically - has enough processor, memory, and storage space).

Sam already has a domain name she wants to use for this Islandora instance - https://islandoratest.university.edu (note: this is a made up sample URL) and has requested the DNS update from IT so the name resolves to the IP address of her server.

Next Sam follows the [Software Dependencies](../install/software-dependencies/#ubuntu) instructions to setup the Ubuntu server to be a host for ISLE (install some software including docker, download/clone the ISLE repository, create and configure a Linux user for islandora).

Because Sam is building a new site with her own URL, she will need to edit a number of the included ISLE configuration files and folders found in the `config` directory of the cloned ISLE repository per the detailed instructions in the ISLE Installation section.

Once all the configurations have been set, Sam is ready to install ISLE itself - she connects to the server via ssh as the islandora user she just created and follows the steps in the ISLE Installation section to copy over the configuration changes and pull down the Dockerhub images one-by-one and start. The final step involves running a provisioning script from the ISLE repository folder.

At this point, Sam should be able to point a browser at her URL and see a functioning Islandora install!
