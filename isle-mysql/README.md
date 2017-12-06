This is the ISLE MYSQL COMMUNITY container.  

Since it is an image you do not need to build it.

See https://github.com/docker-library/mysql

 - place any init DBs as .sql, .sql.gz, .tar in folder `entrypoint` (renamed from the original, why?)
 - place any confs in folder `config` and they will overrride the defaults.

Note that users on host systems with SELinux enabled may see issues with this. The current workaround is to assign the relevant SELinux policy type to your new config file so that the container will be allowed to mount it:
```
$ chcon -Rt svirt_sandbox_file_t /my/custom
```
