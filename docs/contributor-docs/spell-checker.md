# How to Spell-Check the Documentation 

## Installing Aspell

[GNU Aspell](https://github.com/GNUAspell/aspell) is available in the default repositories of most Linux and Unix distributions, so installation should be easy. To install Aspell on Unix/Linux systems use a package manager on your OS.

For example...
* Ubuntu
  * `apt-get install aspell`
* OS X
  * Using [Homebrew](https://brew.sh/)
    * `brew install aspell`

## Setting up Aspell "word list" for Islandora & ISLE

* Check if you already have an existing Aspell "word list" called ".aspell.en.pws" in your home directory.
  * Note: that this file starts with a "period" and my be hidden by default in your OS.
  * Back up existing "~/.aspell.en.pws" file if you want to preserve data, or skip this step if you are OK with overwriting existing data.
* Go to the ISLE documentation repository.
* Copy the current Aspell "word list" (.aspell.en.pws) that was created for the ISLE/Islandora project from the docs directory into your home directory.
  * Note: Make sure to back up any existing ".aspell.en.pws" before copying if you want to avoid losing your previous word list data.

## Running Aspell

### Manually running Aspell on a single file

`aspell check --dont-backup example.md`

### Batch running Aspell on all .md files

`for f in $(find . -iname '*.md'); do aspell check --dont-backup $f; done;`

### Verifying changes
* Since we are using the Aspell `--dont-backup` flag, Aspell will make changes to the files themselves.
* We can then use `git status` and `git diff file_name` to verify what was changed by Aspell before committing changes back to the repository.

### Interactive commands

Common interactive commands used:
* i = Ignore a word.
* I = Ignore all occurrences of a word in the current file.
* a = add word to Aspell "word list" (~/.aspell.en.pws).
* 1 to 10 = Used to select a suggested word to replace selected word.

## Updating Aspell "word list" after checking spellings

* Copy the existing "~/.aspell.en.pws" file in your home directory to the ISLE documentation repository.
* Commit the change(s) to the Aspell word list file.
* Submit a pull request with your changes to be considered by the project committers.
