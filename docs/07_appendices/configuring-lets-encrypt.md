<!--- PAGE_TITLE --->

# Using Let's Encrypt with ISLE

Utilizing [Let's Encrypt](https://letsencrypt.org/) for SSL certificates for your ISLE stack.

Time required: approximately 30 minutes

## Prerequisites
 - An internet accessible ISLE server with a fully qualified domain name (FQDN).
 - The FQDN set in your .env file's `BASE_DOMAIN` is the FQDN we're requesting SSL certificates.


## Steps
On the server (host) hosting your ISLE instance

_Note:_ These commands are run on a shell (terminal).

_Note:_ You do **not** need to stop your stack to complete these steps.

1. Change to the directory with your `docker-compose.yml`
2. Change to the `config/proxy` directory
   `cd config/proxy`
3. Create a blank file called `acme.json`
   `touch acme.json`
4. Modify the permissions of `acme.json` to be user read-write ONLY.
   `chmod 600 acme.json`
5. Using your favorite text editor open `traefik.toml` (_note:_ this file is in your `config/proxy` folder)
6. Locate the following lines

        [[entryPoints.https.tls.certificates]]  
        certFile = "/certs/isle.localdomain.cert"  
        keyFile = "/certs/isle.localdomain.key"  

7. Comment the three lines out by adding a `#` to the beginning of the line:

        # [[entryPoints.https.tls.certificates]]  
        # certFile = "/certs/isle.localdomain.cert"  
        # keyFile = "/certs/isle.localdomain.key"  

8. On a new line add the following, substituting in your email address:

        [acme]  
        email = "your-email@my-institution.org/edu"  
        storage = "acme.json"  
        entryPoint = "https"  
        onHostRule = true  
        onDemand = false  
          [acme.httpChallenge]  
          entryPoint = "http"  

8. Save and close the file.

9. Change back to the directory with your `docker-compose.yml` in terminal.

10. Using your favorite text editor open `docker-compose.yml`.

11. Locate the section for `traefik` and under the `volumes` area for `traefik` add:

        - ./config/proxy/acme.json:/acme.json  ## Automated cert storage.

12. Save and close the file.

13. Reload your ISLE instance by running `docker-compose up -d`.

14. Visit your website and inspect your certificates.

## Resources

https://docs.traefik.io/user-guide/docker-and-lets-encrypt/
