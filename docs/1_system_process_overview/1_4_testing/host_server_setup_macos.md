### Alpha Manual Build MacOS Pre-Requisites

* This current alpha has been built and tested on **Docker For Mac** only. (11/2017)

* A MacOS System (Sierra 10.12.6 or High Sierra 10.13.x) with 8 - 16 GB RAM and 128GB+ of Storage.

* Please install the [latest version](https://download.docker.com/mac/stable/Docker.dmg) from the [Docker store](https://store.docker.com/editions/community/docker-ce-desktop-mac)
  * Please note this will include Docker & Docker Compose in one package.

* Git will need to be installed prior as well.
     * Open a terminal and enter: `git --version`
     * This will trigger the `Install Command Line Developer Tools` prompt, click on the blue `Install` button for the license agreement; then click the white `Agree` button.
     * The package will take 1-2 minutes to download. Click the `Done` button once finished.

* This alpha build uses `islandora-docker.com` as the test domain along with the Docker Compose service names e.g. `mysql, fedora, apache` etc.
     * To ensure this domain resolves properly, one will need to edit their local `/etc/hosts` file.
     * Open up a terminal and enter: `sudo vi /etc/hosts`
     * Add the following:
      >127.0.0.1       localhost islandora-docker.com solr fedora apache mysql fedora.islandora-docker.com apache.islandora-docker.com

* Clone the ISLE repository
     * Open a terminal and enter: `git clone https://github.com/Islandora-Collaboration-Group/ISLE`
     * `cd /yourpathto/ISLE`
     * This process will take 3 - 5 minutes depending on internet bandwidth

#### Next steps
Once this process has finished one chose to do one of the following:

* Continue next steps with the [1.4. -Testing - isle.localdomain Quickstart Guide](alpha_isle_localdomain_quickstart.md)

* Continue next steps with [1.4. -Testing - Migration Guide](alpha_migration_guide.md)

---
