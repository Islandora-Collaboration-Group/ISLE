# Start TICK - Services

  influxdb:
    image: influxdb:latest
    container_name: isle-influxdb-${CONTAINER_SHORT_ID}
    volumes:
      # Mount for influxdb data directory
      - isle-influxdb-data:/var/lib/influxdb
      # Mount for influxdb configuration
      - ./config/tick/influxdb/influxdb.conf:/etc/influxdb/influxdb.conf
    ports:
      # The API for InfluxDB is served on port 8086
      - "8086:8086"
      - "8088:8088"
      # UDP Port
      - "8089:8089"
    networks:
      isle-internal:
    logging:
      driver: syslog
      options:
        tag: "{{.Name}}"

  telegraf:
    image: telegraf:latest
    # Telegraf requires network access to InfluxDB
    container_name: isle-telegraf-${CONTAINER_SHORT_ID}
    volumes:
      # Mount for telegraf configuration
      - ./config/tick/telegraf/telegraf.conf:/etc/telegraf/telegraf.conf:ro
      # Mount for Docker API access
      - /var/run/docker.sock:/var/run/docker.sock:ro
      # To get metrics off the host
      - /:/hostfs:ro
      - /etc:/hostfs/etc:ro
      - /proc:/hostfs/proc:ro
      - /sys:/hostfs/sys:ro
      - /var/run/utmp:/var/run/utmp:ro
    depends_on:
      - influxdb
    networks:
      isle-internal:
    ports:
      # This port should be for rsyslog
      - "6514:6514"
    logging:
      driver: syslog
      options:
        tag: "{{.Name}}"

  kapacitor:
    image: kapacitor:latest
    container_name: isle-kapacitor-${CONTAINER_SHORT_ID}
    volumes:
      # Mount for kapacitor data directory
      - isle-kapacitor-data:/var/lib/kapacitor
      # Mount for kapacitor configuration
      - ./config/tick/kapacitor/kapacitor.conf:/etc/kapacitor/kapacitor.conf
    # Kapacitor requires network access to Influxdb
    ports:
      # The API for Kapacitor is served on port 9092
      - "9092:9092"
    networks:
      isle-internal:
    logging:
      driver: syslog
      options:
        tag: "{{.Name}}"

  chronograf:
    image: chronograf:latest
    container_name: isle-chronograf-${CONTAINER_SHORT_ID}    
    environment:
      - RESOURCES_PATH="/usr/share/chronograf/resources"
      - LOG_LEVEL=error
    volumes:
      # Mount for chronograf database
      - isle-chronograf-data:/var/lib/chronograf/
    ports:
      # The WebUI for Chronograf is served on port 8888
      - "8888:8888"
    depends_on:
      - kapacitor
      - influxdb
      - telegraf
    networks:
      isle-internal:
    logging:
      driver: syslog
      options:
        tag: "{{.Name}}"


# END TICK - Services