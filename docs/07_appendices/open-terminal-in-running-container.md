<!--- PAGE_TITLE --->

# Opening a Terminal/Shell Inside a Running Container

At times you may find it necessary to open a terminal window or 'shell' inside a running container.  In ISLE this will usually involve the 'Apache' container.

In a terminal window on your **host**, enter `docker ps` to show a list of all running containers.  Each container's NAME should appear in the right-most column of the output.  In an *isle.localdomain* environment the name of the Apache container will be `isle-apache-ld`.  In other configurations the container name may be different.

Use `docker exec -it isle-apache-ld bash`, where 'isle-apache-ld' represents the name of the target container, to open a bash shell inside the isle-apache-ld container.  No password will be required.  If successful, your terminal prompt will look something like this: `root@dd9ee02aa718:/#`.
