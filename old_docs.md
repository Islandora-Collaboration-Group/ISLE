

- '2.4 - ISLE Customization Points': 'customize/customization-points.md'
- '2.5 - ISLE Container Selection + Docker Compose Usage': 'docker/container-selection.md'
- '2.6 - Data Management': 'data/data-management.md'


#    - 'Mount Points': 'data/docker-volumes-paths.md'


- 'How to edit ISLE documentation': 'user-docs/docs-edit.md'
- 'Documentation Style Guide': 'user-docs/docs-styleguide.md'

- 'Mkdocs Requirements': 'user-docs/mkdocs-requirements.md'
- 'Getting Started with ISLE': 'user-docs/getting-started-with-isle.md'

- Contributing:
    - 'How to contribute': 'contribute/contribute.md'
    - 'Committers': 'contribute/committers.md'

- Ansible:
    - 'Installation - Ansible': 'ansible/install-ansible.md'
    - 'Ansible commands': 'ansible/ansible-commands.md'
    - 'ISLE Ansible playbooks': 'ansible/ansible-isle-playbooks.md'
    - 'How to customize the Ansible scripts': 'ansible/ansible-scripts-customize.md'
    - 'Ansible Resources': 'ansible/ansible-resources.md'

- Docker Host Setup:

    - 'Storage Requirements': 'host/host-storage-requirements.md'
    - 'Host Maintenance': 'host/host-maintenance.md'
    - 'How to deploy a Host Server for Docker using Ansible': 'host/host-deploy-ansible.md'
    - 'How to deploy Docker & Docker Compose using Ansible': 'host/host-docker-deploy-ansible.md'
    - 'How to configure fail2ban': 'host/security-fail2ban.md'
    - 'How to configure iptables': 'host/security-iptables.md'
    - 'How to configure CSF firewall': 'host/security-csf.md'
    - 'Network Ports Used': 'host/host-network-ports.md'
    - 'Host Environmental Variables': 'host/host-environmental-vars.md'

- Docker:
    - 'Introduction to Docker': 'docker/intro-to-docker.md'
    - 'Docker Commands': 'docker/docker-commands.md'
    - 'Introduction to Docker Compose': 'docker/intro-to-docker-compose.md'
    - 'Docker Compose Commands': 'docker/docker-compose-commands.md'
    - 'How to customize Docker Compose files': 'docker/containers-deploy.md'
    - 'How to customize Dockerfiles': 'docker/containers-deploy.md'
    - 'Docker Resources': 'docker/docker-resources.md'

- Docker Build Images:
    - 'Docker Build Images': 'docker/docker-build-images.md'
    - 'How to update Docker build images': 'docker/docker-update-build-images.md'
    - 'Docker Tagging Policy': 'docker/docker-tagging-policy.md'

- Docker Containers:
    - 'Introduction to Docker Containers': 'containers/intro-to-docker-containers.md'
    - 'Container specifications': 'containers/containers-specifications.md'
    - 'How to run Docker containers with Docker Compose': 'containers/containers-deploy.md'
    - 'Container Network Ports Exposed': 'containers/container-network-ports.md'
    - 'Container Environmental Variables': 'containers/containers-environmental-vars.md'
    - 'How to access Docker containers': 'containers/containers-access.md'
    - 'How to archive Docker containers': 'containers/containers-archive.md'
    - 'How to remove Docker containers': 'containers/containers-remove.md'
    - 'Default users and passwords': 'containers/containers-users-passwords.md'
    - 'Containers - Apache': 'containers/containers-apache.md'
    - 'Containers - Certbot': 'containers/containers-certbot.md'
    - 'Containers - Fedora': 'containers/containers-fedora.md'
    - 'Containers - Gsearch': 'containers/containers-gsearch.md'
    - 'Containers - MySQL': 'containers/containers-mysql.md'
    - 'Containers - Solr': 'containers/containers-solr.md'

- Docker Container software specifications:
    - 'Software Version List': 'containers/supported-software-versions.md'
    - 'Islandora Modules': 'containers/islandora-modules.md'
    - 'Drupal Modules': 'containers/islandora-modules.md'
    - 'Software Repositories': 'containers/supported-software-repositories.md'
    - 'SSL': 'containers/containers-ssl.md'

- Container Volumes and Data Locations:
    - 'How to mount Docker Volumes to pre-existing data': 'data/docker-volumes-preexistingdata.md'
    - 'Configuration files locations': 'data/config-paths.md'
    - 'Log files': 'data/locations-logs.md'
    - 'Indices': 'data/locations-indicies.md'
    - 'Databases': 'data/locations-databases.md'
    - 'Triplestores': 'data/locations-triplestores.md'

- Data Customization:
    - 'Configuration tools and commands': 'data/config-tools-commands.md'
    - 'Logs - How to configure logs': 'data/logs-customize.md'
    - 'Logs - Tomcat - How to configure logs': 'data/logs-tomcat.md'
    - 'How to configure a Drupal site': 'data/drupal-site.md'
    - 'How to configure a Drupal multi-site': 'data/drupal-multisite.md'
    - 'SSL - Apache - How to manually configure SSL certificates': 'data/ssl-apache-manual.md'
    - 'SSL - Apache - How to configure SSL certificates using Certbot and Lets Encrypt': 'data/ssl-apache-certbot.md'
    - 'SSL - Tomcat - How to manually configure SSL certificates': 'data/ssl-tomcat-manual.md'
    - 'SSL - Tomcat - How to configure SSL certificates using Certbot and Lets Encrypt': 'data/ssl-tomcat-certbot.md'

- System Migration Workflow:
    - 'Introduction to Git': 'migration/intro-to-git.md'
    - 'Container Migration Workflow using Git': 'migration/git-migration-workflow.md'
    - 'How to migrate Customized settings, configurations and data from pre-existing sytems': 'migration/pre-existing-customization.md'
    - 'How to attach pre-existing datastores to new containers': 'migration/pre-existing-datastores.md'
    - 'Data Upgration Process': 'migration/upgration.md'

- Data coordination process:
    - 'Migration Tools and scripts': 'migration/migration-tools-scripts.md'
    - 'How to migrate data from existing systems': 'migration/data-migration.md'

- ISLE Platform types:
    - 'Sandbox': 'platforms/sandbox.md'
    - 'Staging': 'platforms/staging.md'
    - 'Production': 'platforms/production.md'
    - 'Workflow Process across Platforms': 'platforms/workflow.md'

- Test Suites:
  - 'Introduction to Travis CI': 'tests/travis-ci.md'
  - 'Docker Build Testing process': 'tests/docker-build-test-process.md'
  - 'Docker Build Tests List': 'tests/docker-build-tests.md'
