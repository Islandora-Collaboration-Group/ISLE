# Matomo - Analytics Package

## Installation

### Assumptions

* Previously installed and running Local, Production and Staging ISLE Host systems are in place already

* You'll need to use the ISLE images tagged as `1.3.0` and higher.

---

### Installation Instructions

Adapted from Diego's original example ISLE config: https://github.com/Islandora-Collaboration-Group/isle_matomo_docker

#### Edit 1: Modify Docker-Compose & .env files

* Create a `data/matomo` directory to persist your Matomo application. You will probably want to put this in a git repo if you're deploying to multiple servers and don't want to have to re-install plugins/etc for each environment. We recommend using an appropriate .gitignore before checking that in (e.g. https://raw.githubusercontent.com/matomo-org/matomo/4.x-dev/.gitignore)

* Copy the files in `config/matomo` into a new directory `data/matomo/config` to seed your Matomo application's config. You'll probably want to commit this to your new Git repo. 

* Within your `docker-compose.*.yml`, add the following blocks to enable new services:

```bash
matomo:
  image: matomo:3-fpm
  container_name: isle-matomo-${CONTAINER_SHORT_ID}
  env_file:
    - .env
  networks:
    isle-internal:
  links:
    - mysql
  depends_on:
    - mysql
    - traefik
  volumes:
    - ./data/matomo/html/:/var/www/html
  labels:
    - traefik.enable=false

matomo-nginx:
  image: nginx
  container_name: isle-matomo-nginx-${CONTAINER_SHORT_ID}
  volumes:
    - ./config/matomo-nginx/ngix.conf:/etc/nginx/nginx.conf:ro
    - ./data/matomo/html/:/var/www/html
  links:
    - matomo
  networks:
    - isle-internal
  ports:
    - "9002:9020"
  labels:
    - traefik.enable=true
    - traefik.backend=matomo
    - traefik.frontend.entryPoints=https
    - traefik.port=9020
    - traefik.frontend.passHostHeader=true
    - traefik.frontend.rule=Host:matomo.${BASE_DOMAIN};
    - traefik.docker.network==${COMPOSE_PROJECT_NAME}_isle-internal

whoami:
  image: containous/whoami # A container that exposes an API to show its IP address
  container_name: isle-whoami-${CONTAINER_SHORT_ID}
  labels:
    - "traefik.frontend.rule=Host:whoami.${BASE_DOMAIN};"
```

Your `traefik` service ports definition needs modifying like this:

```bash
    ports:
      - "80:80"
      - target: 443
        published: 443
        protocol: tcp
        mode: host
      - "8080:8080"
```

And your `traefik` labels definition needs an extra line before the line selecting port 8080:

```bash
      - traefik.frontend.passHostHeader=true
```

* Start up the ISLE Docker containers again. `docker-compose up -d`

---

#### Install and Configure Matomo

0. Create your new Matomo database and call it `matomo_db`. Drush should be able to do this, e.g. `drush sql-create --db-su="root" --db-su-pw="MYQSL_ROOT_PWD" --db-url="mysql://DRUPAL_DB_USER:DRUPAL_DB_PWD@mysql/matomo_db"`. If you use a different method, make sure you make a database user and password to access it rather than use root credentials below.

1. Add matomo.<yourdomain> to your `/etc/hosts` file and visit that URL. You should see a Matomo install dialog. Additional support may be found here: https://matomo.org/docs/installation/

2. Use the following values for Database Setup:

 - Database Server: "mysql"
 - Login: the value of DRUPAL_DB_USER in your local.env file
 - Password: the value of DRUPAL_DB_PASS in your local.env file
 - Database Name: "matomo_db"
 - Table Prefix: "matomo_"
 - Adapter: PDO\\MYSQL

FYI that this creates a config file called `config.ini.php` which is gitignored. This means you'll need to do this for each environment, or you'll need to manually copy (or use Git to sync) that file between environments.

3. Click "Next" a couple times until you get to the "Super User" creation page. Enter (and save!) credentials for your Matomo login.

4. The next screen prompts you to "Setup a Website". Enter your website name ("Islandora" is good, if you're planning on following the LASIR instructions for integrating with Islandora), the URL you're using for the environment you're in, and a timezone. Select "Not an Ecommerce site".

5. The next screen will provide you with a tracking code block. Copy it out for use later, or plan to log into the Matomo dashboard later to fetch it again.

6. Log in. Allow Matomo to update if it wants to. You may have to do this inorder to access the settings areas.

7. Don't forget to check in all the new files in your Matomo Git repo, if you're tracking it that way.

---


#### Configure Islandora to coordinate with Matomo

Now you can pick up from here: https://github.com/Islandora-Collaboration-Group/islandora_matomo#setup-matomo-for-this-module

Important sub-steps:

1. Create an extra "website" if you need to track Authors/Department data.

2. Install the "Plugin" for "Custom Dimensions" to enable Islandora integration.

3. Add the Custom Dimensions in the order requested

4. Fetch your API code to paste into Islandora admin. 

---



---

### Testing



---

## Need Help?

We welcome questions, suggestions, contributions, and respond promptly to requests for technical help. Please join us in the following:

* [Islandora ISLE Interest Group](https://github.com/islandora-interest-groups/Islandora-ISLE-Interest-Group) - Meetings are public and open to everybody! [Schedule](https://github.com/islandora-interest-groups/Islandora-ISLE-Interest-Group/#how-to-join)

* [Islandora ISLE Google group](https://groups.google.com/forum/#!forum/islandora-isle) - Post your questions here and subscribe for updates, meeting announcements, and technical support.

* [ISLE GitHub Issues queue](https://github.com/Islandora-Collaboration-Group/ISLE/issues) - Post your issues, bugs and requests for technical documentation here.
