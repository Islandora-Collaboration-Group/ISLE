# Demo or Local: Edit the "/etc/hosts" File

Edit the `/etc/hosts` file to view ISLE locally on a personal computer browser.

**Please select your operating system and software:**

- [Mac or Ubuntu Using Docker](#mac-or-ubuntu-using-docker)
- [Windows Using Docker](#windows-using-docker)

---

## Mac or Ubuntu Using Docker

* Open a terminal on the personal computer.

* Enter: `sudo nano /etc/hosts`
    * _For end users familiar with editing files on the command line, vim, emacs or alternative tools can be used in lieu of nano_

* Enter the personal computer end user password

* Select the appropriate installation scenario, below:
    1. **For Demo ISLE Installations:**
        * Find the `127.0.0.1 localhost` entry in the `/etc/hosts` file.
        * Go to end of same line, add one space, and then paste the following:
        * `isle.localdomain admin.isle.localdomain images.isle.localdomain portainer.isle.localdomain`

    2. **For Local ISLE Installations:**
        * Find the `127.0.0.1 localhost` entry in the `/etc/hosts` file.
        * Go to end of same line, add one space, and then paste the following, substituting `yourprojectnamehere` with your preferred name:
        * `yourprojectnamehere.localdomain admin.yourprojectnamehere.localdomain images.yourprojectnamehere.localdomain portainer.yourprojectnamehere.localdomain`
        * Note: While not required, it is perfectly okay to have multiple lines so as to define both the Demo and the Local ISLE installations, as shown in the example below:
```
127.0.0.1 localhost isle.localdomain admin.isle.localdomain images.isle.localdomain portainer.isle.localdomain
127.0.0.1 localhost yourprojectnamehere.localdomain admin.yourprojectnamehere.localdomain images.yourprojectnamehere.localdomain portainer.yourprojectnamehere.localdomain
```

* Enter `Cntrl` and the letter `o` together to write the changes to the file.

* Enter `Cntrl` and the letter `x` together to exit the file.

**Please return to the appropriate installation:**

* [Demo ISLE Installation](../install/install-demo.md#step-1-edit-etchosts-file) and continue with _Step 2_.
* [Local ISLE Installation: New Site](../install/install-local-new.md#step-1-edit-etchosts-file) and continue with _Step 2_.
* [Local ISLE Installation: Migrate Existing Islandora Site](../install/install-local-migrate.md#step-1-edit-etchosts-file) and continue with _Step 2_.

---

## Windows Using Docker

* For end users running Windows 10:

* Press the Windows key.

* Type `Notepad`.

* In the search results, RIGHT-CLICK `Notepad`, select `Run as administrator`, and enter `Yes` to prompt.

* Select `File -> Open`.

* In the `File name:` input box, paste this path `C:\Windows\System32\Drivers\etc\hosts`.

* Click `Open`.

* Select the appropriate installation scenario, below:
    1. **For Demo ISLE Installations:**
        * Find the `127.0.0.1 localhost` entry and uncomment it (by deleting the preceding `#` character).
        * Go to end of same line, add one space, and then paste the following:
        * `isle.localdomain admin.isle.localdomain images.isle.localdomain portainer.isle.localdomain`

    2. **For Local ISLE Installations:**
        * Find the `127.0.0.1 localhost` entry and uncomment it (by deleting the preceding `#` character).
        * Go to end of same line, add one space, and then paste the following, substituting `yourprojectnamehere` with your preferred name:
        * `yourprojectnamehere.localdomain admin.yourprojectnamehere.localdomain images.yourprojectnamehere.localdomain portainer.yourprojectnamehere.localdomain`
        * Note: While not required, it is perfectly okay to have multiple lines so as to define both the Demo and the Local ISLE installations, as shown in the example below:
```
127.0.0.1 localhost isle.localdomain admin.isle.localdomain images.isle.localdomain portainer.isle.localdomain
127.0.0.1 localhost yourprojectnamehere.localdomain admin.yourprojectnamehere.localdomain images.yourprojectnamehere.localdomain portainer.yourprojectnamehere.localdomain
```

* Click `File > Save`, and then `File -> Exit`.

**Please return to the appropriate installation:**

* [Demo ISLE Installation](../install/install-demo.md#step-1-edit-etchosts-file) and continue with _Step 2_.
* [Local ISLE Installation: New Site](../install/install-local-new.md#step-1-edit-etchosts-file) and continue with _Step 2_.
* [Local ISLE Installation: Migrate Existing Islandora Site](../install/install-local-migrate.md#step-1-edit-etchosts-file) and continue with _Step 2_.
