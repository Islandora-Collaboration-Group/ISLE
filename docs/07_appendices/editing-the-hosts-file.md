Below are instructions on how to edit the hosts file to connect to ISLE on a test system.


#### Mac or Ubuntu Desktop Using Virtualbox VM (non-Vagrant)

* From the instructions in setting up the Virtualbox VM on your OS (MacOS, Ubuntu or Windows), the IP used to setup the Host-Only network was `10.10.10.130`

* Add the value of `10.10.10.130 isle.localdomain admin.isle.localdomain images.isle.localdomain portainer.isle.localdomain` to the laptop / workstation's `/etc/hosts` file.   

* For endusers running MacOS and Ubuntu Desktop:

   * Open a terminal on the local laptop

   * Enter: `sudo nano /etc/hosts`
     * _For endusers familiar with editing files on the command line, vim or alternative tools can be used in lieu of nano_

   * Enter the laptop enduser password

   * Add the values below the `127.0.0.1` entry in the `/etc/hosts` file.

       * `10.10.10.130 localhost isle.localdomain admin.isle.localdomain images.isle.localdomain portainer.isle.localdomain`  
       * **Example**

```
        127.0.0.1 localhost
        10.10.10.130 localhost isle.localdomain admin.isle.localdomain images.isle.localdomain portainer.isle.localdomain
```


* Enter `Cntrl` and the letter `o` together to write the changes to the file.
* Enter `Cntrl` and the letter `x` together to exit the file.

---

#### Mac or Ubuntu Using Docker

* **Docker For Mac** If you are using Docker For Mac, then use the IP address of `127.0.0.1` to resolve to `localhost` and to the `isle.localdomain localhost isle.localdomain admin.isle.localdomain images.isle.localdomain portainer.isle.localdomain` domain names

* Open a terminal on the local laptop

* Enter: `sudo nano /etc/hosts`
  * _For endusers familiar with editing files on the command line, vim or alternative tools can be used in lieu of nano_

* Enter the laptop enduser password

* Add the values next to the `127.0.0.1 localhost` entry in the `/etc/hosts` file.

    * `127.0.0.1 localhost isle.localdomain admin.isle.localdomain images.isle.localdomain portainer.isle.localdomain` with a space in between the entries.  

  * Enter `Cntrl` and the letter `o` together to write the changes to the file.

  * Enter `Cntrl` and the letter `x` together to exit the file.

---

#### Windows Using Docker

* For endusers running Windows 10:

    * Press the Windows key.

    * Type `Notepad` in the search field.

    * In the search results, right-click `Notepad` and select `Run as administrator`.

    * From `Notepad`, open the following file: `C:\Windows\System32\Drivers\etc\hosts`

    * Add the values next to the `127.0.0.1 localhost` entry in the `/etc/hosts` file.

        * `127.0.0.1 localhost isle.localdomain admin.isle.localdomain images.isle.localdomain portainer.isle.localdomain` with a space in between the entries.

    * Click File > Save to save your changes.

---

#### Windows Using Virtualbox VM (non-Vagrant)

* For endusers running Windows 10:

    * Press the Windows key.

    * Type `Notepad` in the search field.

    * In the search results, right-click `Notepad` and select `Run as administrator`.

    * From `Notepad`, open the following file: `C:\Windows\System32\Drivers\etc\hosts`

    * Add the values below the `127.0.0.1` entry in the `/etc/hosts` file.

        * `10.10.10.130 isle.localdomain admin.isle.localdomain images.isle.localdomain portainer.isle.localdomain`  

    * Click File > Save to save your changes.
