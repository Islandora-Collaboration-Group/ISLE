<!--- PAGE_TITLE --->

# Let's Encrypt: Free SSL Certificates

[Let's Encrypt](https://letsencrypt.org/) is a free, automated, and open Certificate Authority for generating SSL certificates for your ISLE environment.

_Time required: approximately 30 minutes._


## Prerequisites
 - An internet accessible ISLE server with a fully qualified domain name (FQDN).
 - The FQDN set in your .env file's `BASE_DOMAIN` is the FQDN we're requesting for SSL certificates.


## Steps

_Note:_ You do **not** need to stop your stack to complete these steps.

1. Open a terminal on your personal computer and ssh to the server or VM **that is hosting your ISLE environment**.
2. Change to the "ISLE" directory that contains the "docker-compose.yml": `cd /opt/ISLE`
3. Change to the "config/proxy" directory: `cd config/proxy`
4. Create a blank file called "acme.json": `touch acme.json`
5. Modify the permissions of "acme.json" to be user read-write ONLY: `chmod 600 acme.json`
6. Use a text editor (emacs, nano, vi, etc.) to open "traefik.toml" (_note:_ this file is in your "config/proxy" folder)
7. Locate the following lines

        [[entryPoints.https.tls.certificates]]  
        certFile = "/certs/isle.localdomain.cert"  
        keyFile = "/certs/isle.localdomain.key"  

8. Comment the three lines out by adding a `#` to the beginning of the line:

        # [[entryPoints.https.tls.certificates]]  
        # certFile = "/certs/isle.localdomain.cert"  
        # keyFile = "/certs/isle.localdomain.key"  

9. On a new line add the following, substituting in your email address:

        [acme]  
        email = "your-email@my-institution.org/edu"  
        storage = "acme.json"  
        entryPoint = "https"  
        onHostRule = true  
        onDemand = false  
          [acme.httpChallenge]  
          entryPoint = "http"  

10. Save and close the file.

11. Change back to the directory with your "docker-compose.yml" in terminal.

12. Use a text editor to open "docker-compose.yml".

13. Locate the section for "traefik" and under the "volumes" area for "traefik" add:

        - ./config/proxy/acme.json:/acme.json  ## Automated cert storage.

14. Save and close the file.

15. Reload your ISLE environment by running `docker-compose up -d`.

16. Visit your website and inspect your certificates.

---

## Additional Resources

* [Let's Encrypt & Docker](https://docs.traefik.io/user-guide/docker-and-lets-encrypt/)

