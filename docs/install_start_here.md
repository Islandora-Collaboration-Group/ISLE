Please select your desired use case for ISLE.

**I would like to:**

* [Test ISLE](#test-isle) installation on my laptop or workstation

* [Create a new ISLE site](#create-a-new-isle-site-or-sites) on a production, staging or development server

* [Migrate to ISLE](install_start_here/#migrate-to-isle) on a production, staging or development server

* [Update ISLE](#update-isle) to install the newest improvements and security updates

**Please note:** There is a [Glossary](glossary.md) with relevant terms to help guide installation.

---

## Test ISLE

The recommended non-production environments for using the prebuilt ISLE test site (`isle.localdomain`) are with the following type of laptops or workstations.

Please review the [ISLE Host Server OS requirements](01_installation_host_server/host_server_system_specifications.md) prior to any installation.

Please follow the columns below from left to right, to select:

1. `Desktop OS` is your laptop or desktop running a particular operating system `(OS)`

2. `ISLE Host Server Install instructions` is what you'll follow to host (_run_) ISLE. This may involve additional steps for installing a virtualized OS (CentOS or Ubuntu).

3. `Install ISLE Host Server Dependencies` is the software you may need to run ISLE on a `Host Platform` whether you use CentOS or Ubuntu as the ISLE Host Server.

4. `ISLE install instructions` _(for all OSes and Platform types)_

**Please note:**

* _Depending on the configuration you choose, you may have more or less instructions to follow._

* The operating systems displayed below are the minimum supported. Do not go lower than what is recommended.

| Desktop OS        | ISLE Host Server Install instructions | Install ISLE Host Server Dependencies | ISLE install instructions |
| -------------     | -------------                         | -------------                         |  -------------            |
| MacOS High Sierra | [Docker for Mac](01_installation_host_server/install_docker_for_mac.md) | _Skip to the next column_ |[Test ISLE](/02_installation_test/ild_installation_guide.md)|
| MacOS High Sierra | [Vagrant](01_installation_host_server/install_macos_vagrant.md)         |[On CentOS](01_installation_host_server/install_on_centos.md) | [Test ISLE](/02_installation_test/ild_installation_guide.md)|
|                   |                                       |[On Ubuntu](01_installation_host_server/install_on_ubuntu.md)|                                                            |
|                   |                                       |                                                             |                                                            |
| Windows 10 | [Docker for Windows](01_installation_host_server/install_docker_for_windows.md) |_Skip to the next column_ |[Test ISLE](/02_installation_test/ild_installation_guide.md)|


---

## Create a new ISLE site or site(s)

The recommended environments for setting up a new ISLE site using a single or multiple ISLE environment setup (e.g. an ISLE host server running production, staging or development versions of one site) are with the following type of servers. (_not desktop or workstation_)

Typically new sites are used in production, staging or shared development environments.

* If you are only going to setup one ISLE site within a single environment (e.g. production only) or run all of your ISLE environments on separate ISLE host servers or VMs please using the: [New ISLE Site - Single Env](03_installation_new_site/new_site_installation_guide_single.md)

* If you are going to setup multiple ISLE sites using multiple environments (e.g. dev, stage, and prod etc.) on one ISLE Host server or VM please using the: [New ISLE Sites - Multi Envs](03_installation_new_site/new_site_installation_guide_multi.md)
    * Please note: setting up multiple ISLE sites and environments on one ISLE host server will require adequate resources.
    * A good rule of thumb is to take the specifications outlined in the [ISLE Host Server OS requirements](01_installation_host_server/host_server_system_specifications.md) and divide by three as an initial concept. Bear in mind, that any `dev` environment doesn't need as much resource but any potential `stage` and `prod` environments should mirror each other in resource usage and setup.

Please review the [ISLE Host Server OS requirements](01_installation_host_server/host_server_system_specifications.md) prior to any installation.

Please follow the columns below from left to right, to select:

1. `Host Platform Install instructions` is how you can setup your cloud hosting or server environment for an ISLE Host Server.
2. `Install ISLE Host Server Dependencies` is the software you may need to run ISLE on a `Host Platform` whether you use CentOS or Ubuntu as the ISLE Host Server.
3. `ISLE install instructions` (_for all OSes and Platform types_)


**Please note:**

* _Depending on the configuration you choose, you may have more or less instructions to follow._

* The operating systems displayed below are the minimum supported. Do not go lower than what is recommended.

| Hosting Platform | Install ISLE Host Server Dependencies                         | ISLE new site(s) install instructions                                    |
| -------------    | -------------                                                 | -------------                                                            |
| Amazon (AWS)     | [On CentOS](01_installation_host_server/install_on_centos.md) | [New ISLE Site - Single Env](03_installation_new_site/new_site_installation_guide_single.md) |
|                  | [On Ubuntu](01_installation_host_server/install_on_ubuntu.md) | [New ISLE Sites - Multi Env](03_installation_new_site/new_site_installation_guide_multi.md)  |
|                  |                                                               |                                                                          |
| Google (GCP)     | [On CentOS](01_installation_host_server/install_on_centos.md) | [New ISLE Site - Single Env](03_installation_new_site/new_site_installation_guide_single.md) |
|                  | [On Ubuntu](01_installation_host_server/install_on_ubuntu.md) | [New ISLE Sites - Multi Env](03_installation_new_site/new_site_installation_guide_multi.md)  |
|                  |                                                               |                                                                          |
| Server / VM      | [On CentOS](01_installation_host_server/install_on_centos.md) | [New ISLE Site - Single Env](03_installation_new_site/new_site_installation_guide_single.md) |
|                  | [On Ubuntu](01_installation_host_server/install_on_ubuntu.md) | [New ISLE Sites - Multi Env](03_installation_new_site/new_site_installation_guide_multi.md)  |

---

## Migrate to ISLE

The recommended environments for migrating a current production Islandora site to a new ISLE environment are with the following type of servers. (_not desktop or workstation_)

Typically migrated sites are used in production, staging or shared development environments.

Please review the [ISLE Host Server OS requirements](01_installation_host_server/host_server_system_specifications.md) prior to any installation.

Please follow the columns below from left to right, to select:

1. `Host Platform Install instructions` is how you can setup your cloud hosting or server environment for an ISLE Host Server.
2. `Install ISLE Host Server Dependencies` is the software you may need to run ISLE on a `Host Platform` whether you use CentOS or Ubuntu as the ISLE Host Server.
3. `ISLE install instructions` (_for all OSes and Platform types_)

**Please note:**

* _Depending on the configuration you choose, you may have more or less instructions to follow._

* The operating systems displayed below are the minimum supported. Do not go lower than what is recommended.

| Hosting Platform | Install ISLE Host Server Dependencies                         | ISLE install instructions                                                    |
| -------------    | -------------                                                 | -------------                                                                |
| Amazon (AWS)     | [On CentOS](01_installation_host_server/install_on_centos.md) | [Migrate to ISLE](04_installation_migration/migration_installation_guide.md) |
|                  | [On Ubuntu](01_installation_host_server/install_on_ubuntu.md) |                                                                              |
|                  |                                                               |                                                                              |
| Google (GCP)     | [On CentOS](01_installation_host_server/install_on_centos.md) | [Migrate to ISLE](04_installation_migration/migration_installation_guide.md) |
|                  | [On Ubuntu](01_installation_host_server/install_on_ubuntu.md) |                                                                              |
|                  |                                                               |                                                                              |
| Server / VM      | [On CentOS](01_installation_host_server/install_on_centos.md) | [Migrate to ISLE](04_installation_migration/migration_installation_guide.md) |
|                  | [On Ubuntu](01_installation_host_server/install_on_ubuntu.md) |                                                                              |

## Update ISLE

The following offers a method for updating an existing ISLE installation to install the newest improvements and security updates, regardless of environment.

This process is backwards compatible with your existing ISLE site.

- DETAILS COMING SOON...
