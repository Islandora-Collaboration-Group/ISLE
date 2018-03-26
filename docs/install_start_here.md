Please select from one of the following four possible ISLE use cases.

I would like to:

* Test ISLE

* Setup a new ISLE site

* Migrate to ISLE

* Build ISLE (Maintainer)

---

## Test ISLE

The recommended non-production environments for using the prebuilt ISLE test site (`isle.localdomain`) are with the following type of laptops or workstations.

Please review the [ISLE Host Server OS requirements](01_installation_host_server/host_server_system_specifications.md) prior to any installation.

**Please note:** There is a [Glossary](glossary.md) with relevant terms to help guide installation.

Please follow the columns below from left to right, to select:

1. `Desktop OS` is your laptop or desktop running a particular operating system `(OS)`

2. `ISLE Host Server Install instructions ` is what you'll follow to host (_run_) ISLE. This may involve additional steps for installing a virtualized OS (CentOS or Ubuntu).

3. `Install ISLE Host Server Dependencies` is the software you may need to run ISLE on a `Host Platform` whether you use CentOS or Ubuntu as the ISLE Host Server.

4. `ISLE install instructions` _(for all OSes and Platform types)_

**Please note:**

* _Depending on the configuration you choose, you may have more or less instructions to follow._

* The operating systems displayed below are the minimum supported. Do not go lower than what is recommended.

| Desktop OS           | ISLE Host Server Install instructions                    | Install ISLE Host Server Dependencies                       | ISLE install instructions                                  |
| -------------        | -------------                                            | -------------                                               |  -------------                                             |
| MacOS High Sierra    | [Docker for Mac](01_installation_host_server/install_docker_for_mac.md)          |_Skip to the next column_                                    |[Test ISLE](/02_installation_test/ild_installation_guide.md)|
| MacOS High Sierra    | [Vagrant](install_macos_vagrant.md)                      |[On CentOS](01_installation_host_server/install_on_centos.md)|[Test ISLE](/02_installation_test/ild_installation_guide.md)|
|                      |                                                          |[On Ubuntu](01_installation_host_server/install_on_ubuntu.md)|                                                            |
| MacOS High Sierra    | [Virtualbox VM (only)](install_macos_vbox.md)            |[On CentOS](01_installation_host_server/install_on_centos.md)|[Test ISLE](/02_installation_test/ild_installation_guide.md)|
|                      |                                                          |[On Ubuntu](01_installation_host_server/install_on_ubuntu.md)|                                                            |
|                      |                                                          |                                                             |                                                            |
| Ubuntu 16.04 Desktop | [Docker on Ubuntu Desktop](install_ubuntu_desktop_docker.md)| _Skip to the next column_                                |[Test ISLE](/02_installation_test/ild_installation_guide.md)|
| Ubuntu 16.04 Desktop | [Vagrant](install_ubuntu_vagrant.md)                     |[On CentOS](01_installation_host_server/install_on_centos.md)|[Test ISLE](/02_installation_test/ild_installation_guide.md)|
|                      |                                                          |[On Ubuntu](01_installation_host_server/install_on_ubuntu.md)|                                                            |
| Ubuntu 16.04 Desktop | [Virtualbox VM (only)](install_ubuntu_vbox.md)           |[On CentOS](01_installation_host_server/install_on_centos.md)|[Test ISLE](/02_installation_test/ild_installation_guide.md)|
|                      |                                                          |[On Ubuntu](01_installation_host_server/install_on_ubuntu.md)|                                                            |
|                      |                                                          |                                                             |                                                            |
| Windows 10           | [Docker for Windows](01_installation_host_server/install_docker_for_windows.md) |_Skip to the next column_                                    |[Test ISLE](/02_installation_test/ild_installation_guide.md)|
| Windows 10           | [Vagrant](install_windows_vagrant.md)                    |[On CentOS](01_installation_host_server/install_on_centos.md)|[Test ISLE](/02_installation_test/ild_installation_guide.md)|
|                      |                                                          |[On Ubuntu](01_installation_host_server/install_on_ubuntu.md)|                                                            |
| Windows 10           | [Virtualbox VM (only)](install_windows_vbox.md)          |[On CentOS](01_installation_host_server/install_on_centos.md)|[Test ISLE](/02_installation_test/ild_installation_guide.md)|
|                      |                                                          |[On Ubuntu](01_installation_host_server/install_on_ubuntu.md)|                                                            |

---

## Create a new ISLE site

The recommended environments for setting up a new ISLE site are with the following type of servers. (_not desktop or workstation_)

Typically new sites are used in production, staging or shared development environments.

Please review the [ISLE Host Server OS requirements](01_installation_host_server/host_server_system_specifications.md) prior to any installation.

**Please note:** There is a [Glossary](glossary.md) with relevant terms to help guide installation.

Please follow the columns below from left to right, to select:

1. `Host Platform Install instructions` is how you can setup your cloud hosting or server environment for an ISLE Host Server.
2. `Install ISLE Host Server Dependencies` is the software you may need to run ISLE on a `Host Platform` whether you use CentOS or Ubuntu as the ISLE Host Server.
3. `ISLE install instructions` (_for all OSes and Platform types_)


**Please note:**

* _Depending on the configuration you choose, you may have more or less instructions to follow._

* The operating systems displayed below are the minimum supported. Do not go lower than what is recommended.

| Hosting Platform  Install instructions | Install ISLE Host Server Dependencies                         | ISLE install instructions                                                |
| -------------                          | -------------                                                 | -------------                                                            |
| [Amazon (AWS)](install_aws.md)         | [On CentOS](01_installation_host_server/install_on_centos.md) | [New ISLE Site](03_installation_new_site/new_site_installation_guide.md) |
|                                        | [On Ubuntu](01_installation_host_server/install_on_ubuntu.md) |                                                                          |
|                                        |                                                               |                                                                          |
| [Google (GCP)](install_gcp.md)         | [On CentOS](01_installation_host_server/install_on_centos.md) | [New ISLE Site](03_installation_new_site/new_site_installation_guide.md) |
|                                        | [On Ubuntu](01_installation_host_server/install_on_ubuntu.md) |                                                                          |
|                                        |                                                               |                                                                          |
| [Server / VM](install_server_vm.md)    | [On CentOS](01_installation_host_server/install_on_centos.md) | [New ISLE Site](03_installation_new_site/new_site_installation_guide.md) |
|                                        | [On Ubuntu](01_installation_host_server/install_on_ubuntu.md) |                                                                          |

---

## Migrate to ISLE

The recommended environments for migrating a current production Islandora site to a new ISLE environment are with the following type of servers. (_not desktop or workstation_)

Typically migrated sites are used in production, staging or shared development environments.

Please review the [ISLE Host Server OS requirements](01_installation_host_server/host_server_system_specifications.md) prior to any installation.

**Please note:** There is a [Glossary](glossary.md) with relevant terms to help guide installation.

Please follow the columns below from left to right, to select:

1. `Host Platform Install instructions` is how you can setup your cloud hosting or server environment for an ISLE Host Server.
2. `Install ISLE Host Server Dependencies` is the software you may need to run ISLE on a `Host Platform` whether you use CentOS or Ubuntu as the ISLE Host Server.
3. `ISLE install instructions` (_for all OSes and Platform types_)

**Please note:**

* _Depending on the configuration you choose, you may have more or less instructions to follow._

* The operating systems displayed below are the minimum supported. Do not go lower than what is recommended.

| Hosting Platform  Install instructions | Install ISLE Host Server Dependencies                         | ISLE install instructions                                                    |
| -------------                          | -------------                                                 | -------------                                                                |
| [Amazon (AWS)](install_aws.md)         | [On CentOS](01_installation_host_server/install_on_centos.md) | [Migrate to ISLE](04_installation_migration/migration_installation_guide.md) |
|                                        | [On Ubuntu](01_installation_host_server/install_on_ubuntu.md) |                                                                              |
|                                        |                                                               |                                                                              |
| [Google (GCP)](install_gcp.md)         | [On CentOS](01_installation_host_server/install_on_centos.md) | [Migrate to ISLE](04_installation_migration/migration_installation_guide.md) |
|                                        | [On Ubuntu](01_installation_host_server/install_on_ubuntu.md) |                                                                              |
|                                        |                                                               |                                                                              |
| [Server / VM](install_server_vm.md)    | [On CentOS](01_installation_host_server/install_on_centos.md) | [Migrate to ISLE](04_installation_migration/migration_installation_guide.md) |
|                                        | [On Ubuntu](01_installation_host_server/install_on_ubuntu.md) |                                                                              |

## Build ISLE (Maintainer)

The recommended environments for building ISLE images are with the following types of laptop or workstation.

Please review the [ISLE Host Server OS requirements](01_installation_host_server/host_server_system_specifications.md) prior to any installation.

**Please note:** There is a [Glossary](glossary.md) with relevant terms to help guide installation.

Please follow the columns below from left to right, to select:

1. `Desktop OS` is your laptop or desktop running a particular operating system `(OS)`

2. `ISLE Host Server Install instructions ` is what you'll follow to host (_run_) ISLE. This may involve additional steps for installing a virtualized OS (CentOS or Ubuntu).

3. `Install ISLE Host Server Dependencies` is the software you may need to run ISLE on a `Host Platform` whether you use CentOS or Ubuntu as the ISLE Host Server.

4. `ISLE install instructions` _(for all OSes and Platform types)_

**Please note:**

* _Depending on the configuration you choose, you may have more or less instructions to follow._

* The operating systems displayed below are the minimum supported. Do not go lower than what is recommended.

| Desktop OS           | ISLE Host Server Install instructions                     | Install ISLE Host Server Dependencies                    | ISLE install instructions                                    |
| -------------        | -------------                                                | -------------                                           |  -------------                                     |
| MacOS High Sierra    | [Docker for Mac](01_installation_host_server/install_docker_for_mac.md)              | _Skip to the next column_                               | [Build ISLE](05_developer_docs/dev_build_guide.md) |
| MacOS High Sierra    | [Vagrant](install_macos_vagrant.md)                          | [On CentOS](01_installation_host_server/install_on_centos.md) | [Build ISLE](05_developer_docs/dev_build_guide.md) |
|                      |                                                              | [On Ubuntu](01_installation_host_server/install_on_ubuntu.md) |                                                    |
| MacOS High Sierra    | [Virtualbox VM (only)](install_macos_vbox.md)                | [On CentOS](01_installation_host_server/install_on_centos.md) | [Build ISLE](05_developer_docs/dev_build_guide.md) |
|                      |                                                              | [On Ubuntu](01_installation_host_server/install_on_ubuntu.md) |                                                    |
|                      |                                                              |                                                         |                                                    |
| Ubuntu 16.04 Desktop | [Docker on Ubuntu Desktop](install_ubuntu_desktop_docker.md) | _Skip to the next column_                               | [Build ISLE](05_developer_docs/dev_build_guide.md) |
| Ubuntu 16.04 Desktop | [Vagrant](install_ubuntu_vagrant.md)                         | [On CentOS](01_installation_host_server/install_on_centos.md) | [Build ISLE](05_developer_docs/dev_build_guide.md) |
|                      |                                                              | [On Ubuntu](01_installation_host_server/install_on_ubuntu.md) |                                                    |
| Ubuntu 16.04 Desktop | [Virtualbox VM (only)](install_ubuntu_vbox.md)               | [On CentOS](01_installation_host_server/install_on_centos.md) | [Build ISLE](05_developer_docs/dev_build_guide.md) |
|                      |                                                              | [On Ubuntu](01_installation_host_server/install_on_ubuntu.md) |                                                    |
|                      |                                                              |                                                         |                                                    |
| Windows 10           | [Docker for Windows](01_installation_host_server/install_docker_for_windows.md)     | _Skip to the next column_                               | [Build ISLE](05_developer_docs/dev_build_guide.md) |
| Windows 10           | [Vagrant](install_windows_vagrant.md)                        | [On CentOS](01_installation_host_server/install_on_centos.md) | [Build ISLE](05_developer_docs/dev_build_guide.md) |
|                      |                                                              | [On Ubuntu](01_installation_host_server/install_on_ubuntu.md) |                                                    |
| Windows 10           | [Virtualbox VM (only)](install_windows_vbox.md)              | [On CentOS](01_installation_host_server/install_on_centos.md) | [Build ISLE](05_developer_docs/dev_build_guide.md) |
|                      |                                                              | [On Ubuntu](01_installation_host_server/install_on_ubuntu.md) |                                                    |
