# ISLE Installation: Second stack on same server

You can install a second instance of ISLE on the same server with some changes to the `docker-compose.(instance).yml` files for both the first and second projects.

## Assumptions

These instructions assume you have a working ISLE site, and wish to run a second ISLE site on the same machine. They presume you are running in a Production environment,
but they apply equally to Local and Staging.

## Step 1: Build an ISLE installation in a new directory.

Follow all the directions from the install docs for a new or migrated ISLE site.

- Install a new copy of the ISLE repo in a new directory (e.g. `/opt/project2-isle`)
- Create a new Data directory for your project (e.g. `/opt/project2-data`)
- Configure your `docker-compose.production.yml` file to use the new project's data directory

## Step 2: Set up network customizations in docker-compose.production.yml

### In the original project's directory (Project 1):
- Bring down the Docker containers with `docker-compose down`
- Make the following edits to `docker-compose.production.yml`:
  - Under the `# Defined Networks` heading:
  ````
  networks:
    isle-internal:
    name: isle-internal
  isle-external:
    name: isle-external
    ```
  - Bring the containers back online with `docker-compose up -d`
  
### In the new project's directory (Project 2):
- In `docker-compose.production.yml`, make the following edits:
  - Comment out the entire `portainer` and `traefik` sections
  - In the `apache` section, under `Depends on`, comment out `-traefik`
  - Under the `# Defined Networks` heading:
  ```
  networks:
    isle-internal:
      name: isle-internal
      external: true
  ```
- Bring the contaiers online with `docker-compose up -d`
