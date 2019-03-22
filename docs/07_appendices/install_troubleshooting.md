<!-- Troubleshooting -->

This is a basic guide / area that includes some general troubleshooting tips, techniques, thoughts towards helping ISLE users. This section is to grow over time.

## Docker Service and Firewalls

While it is best practice to run the Docker service with a firewall like `iptables`, some cases, issues can arise depending on which service has been restarted and configured.

* Ensure that all ports exposed on containers and hosts (see your `docker-compose.yml` for these ports) are added to the `iptables` rules and persistent. Do note that only ports `80` & `443` should ever be open to the public Internet.

* `Suggested service start order:`
    * The firewall be started / restart first
    * The Docker service started / restarted second
    * ISLE containers started / restart third

* Some errors like "Failed to find document http://www.loc.gov/standards/mods/v3/mods-3-4.xsd" are an indication of materials being blocked improperly. Try following the `Suggested service start order:` above to resolve these type of issues.

* On occasion, being unable to import an object, or edit a mods datastream are indications of materials being blocked improperly. Try following the `Suggested service start order:` above to resolve these type of issues.
