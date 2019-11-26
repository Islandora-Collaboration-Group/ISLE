#!/bin/bash
## Date: November 2019
## This script isle-v140-git-cleanup-script.sh is for the express purpose of removing
## files, binaries and folders from your forked or cloned ISLE project git repository. 
## Please consult https://islandora-collaboration-group.github.io/ISLE/cookbook-recipes/isle-v140-git-cleanup.md/
## Prior to running this script

# Script will fail if a variable hasn’t been set.
set -u

# Script will exit immediately if a command fails.
set -e

echo ""

echo ""

echo "Type in the name of your git repo only e.g. ISLE (do not include .git) and then press the [ENTER] or [RETURN] key to continue"
read -p 'Enter the name here:  ' reponame

echo ""

echo "Checking size of $reponame.git"
function checksize {
  du -sh $reponame.git
}

sizebefore=$(checksize)

echo ""

echo "Downloading BFG clean up tools about 13 MB"
curl -o bfg-1.13.0.jar http://search.maven.org/classic/remotecontent?filepath=com/madgag/bfg/1.13.0/bfg-1.13.0.jar

echo ""

echo "Deleting files & folders. Please note this process will take 30 - 60 seconds with no output"
java -jar bfg-1.13.0.jar --delete-files dg.localdomain.key $reponame.git > /dev/null 2>&1
java -jar bfg-1.13.0.jar --delete-files dg.localdomain.pem $reponame.git > /dev/null 2>&1
java -jar bfg-1.13.0.jar --delete-files docker-compose.DG-FEDORA.yml  $reponame.git > /dev/null 2>&1
java -jar bfg-1.13.0.jar --delete-files fcrepo-drupalauthfilter-3.8.1.jar  $reponame.git > /dev/null 2>&1
java -jar bfg-1.13.0.jar --delete-files fcrepo-installer-3.8.1.jar  $reponame.git > /dev/null 2>&1
java -jar bfg-1.13.0.jar --delete-files fedoragsearch-2.8.1.zip  $reponame.git > /dev/null 2>&1
java -jar bfg-1.13.0.jar --delete-files log4j-over-slf4j-1.6.6.jar  $reponame.git > /dev/null 2>&1
java -jar bfg-1.13.0.jar --delete-files search_index.json  $reponame.git > /dev/null 2>&1
java -jar bfg-1.13.0.jar --delete-files sitemap.xml  $reponame.git > /dev/null 2>&1
java -jar bfg-1.13.0.jar --delete-files sitemap.xml.gz  $reponame.git > /dev/null 2>&1
java -jar bfg-1.13.0.jar --delete-files slf4j-jdk14-1.6.6.jar  $reponame.git > /dev/null 2>&1
java -jar bfg-1.13.0.jar --delete-folders drupal $reponame.git > /dev/null 2>&1
java -jar bfg-1.13.0.jar --delete-folders solr-4.10.4 $reponame.git > /dev/null 2>&1
rm -rf $reponame.git.bfg-report

echo "Finished deleting files & folders"


echo ""

echo "cd into the repository"
cd $reponame.git || exit

echo ""

echo "Running Git commands"
git reflog expire --expire=now --all && git gc --prune=now --aggressive

echo ""

echo "cd back to parent directory"
cd ../ || exit

echo ""

echo "Checking size of $reponame.git"
function checksize {
  du -sh $reponame.git
}

sizeafter=$(checksize)

echo "Old size $sizebefore"
echo "New size $sizeafter"

echo ""
echo ""

echo "This script has now completed but there are more steps. Please continue to read the directions at https://islandora-collaboration-group.github.io/ISLE/cookbook-recipes/isle-v140-git-cleanup/"

echo ""

exit