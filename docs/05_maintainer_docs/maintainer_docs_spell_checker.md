# How to check documentation spelling

## Installing Aspell

[GNU Aspell](https://github.com/GNUAspell/aspell) is available in the default repositories of most Linux and Unix distributions, so installation should be easy. To install Aspell on Unix/Linux systems use a package manager on your OS.

For example...
* Ubuntu
  * `apt-get install aspell`
* OS X
  * Using [Homebrew](https://brew.sh/)
    * `brew install aspell`

## Setting up Aspell "word list" for Islandora & ISLE

[content pending]

## Running Aspell

### Manually running Aspell on a single file

`aspell check --dont-backup example.md`

### Batch running Aspell on all .md files

`for f in $(find . -iname '*.md'); do aspell check --dont-backup $f; done;`

### Interactive commands

Common interactive commands used:
* i = ignore a word
* I = ignore all occurrences
 of a word in the current file.
* a = add word to Aspell "word list"

## Updating Aspell "word list" after checking spellings

[content pending]
