# Varnish

## What is this component?

*
*

---

## Why use this component with ISLE?

*
*

---

## Requirements

*
*

---

## Assumptions

*
*

---

## Installation

* Assumptions: you have a running ISLE...
* Docker-compose down, make the below edits (d-c and .env)
* dc pull
* spin up contianers
* verify it's working by ___

---

### Docker-compose.yml edits

* Add a new service to your docker-compose file:

```bash 
  varnish:
    # build:
    #   context: ../images/isle-varnish
    image: islandoracollabgroup/isle-varnish:1.1.1
    container_name: isle-varnish-${CONTAINER_SHORT_ID}
    env_file:
      - .env
    networks:
      isle-internal:
    depends_on:
      - mysql
      - fedora
      - solr
      - apache
      - traefik
    labels:
      - traefik.docker.network=${COMPOSE_PROJECT_NAME}_isle-internal
      - traefik.port=6081
      - traefik.enable=true
      - "traefik.frontend.rule=Host:${BASE_DOMAIN}; PathPrefix: /, /adore-djatoka, /cantaloupe"
  ```
* If you're pushing log events to TICK, add logging instructions to this new service
```bash
    logging:
      driver: syslog
      options:
        tag: "{{.Name}}"
```
* Edit your apache service to remove the traefik labels

```bash
    #labels:
    #  - traefik.docker.network=${COMPOSE_PROJECT_NAME}_isle-internal
    #  - traefik.enable=true
    #  - "traefik.frontend.rule=Host:${BASE_DOMAIN}; PathPrefix: /, /adore-djatoka, /cantaloupe"
```

* No change to traefik service: the varnish service now handles web traffic to your main domain.


### .env edits
* Add a new block of ENV variables to your main .env file:

```bash
## Varnish
## Varnish Admin port is 6082
## varnish is substituted for default value of localhost to be open to apache
## otherwise Drupal varnish module cannot connect via CLI
VARNISH_ADMIN=varnish
VARNISH_ADMIN_PORT=6082
## Varnish backend
## the apache service is the "backend" for varnish
VARNISH_BACKEND=apache
VARNISH_BACKEND_PORT=80
## Varnish Cache Memory Allocation
VARNISH_MALLOC=256m
## Maximum amount of connections
VARNISH_MAX_CONNECTIONS=300
## Varnish secret aka Control key
VARNISH_SECRET=isle_varnish_secret
## Varnish port
VARNISH_VARNISH_PORT=6081
```
---

## Troubleshooting

*
*

---

## Release Notes

*
*

---

## Additional Resources
*
*

---
