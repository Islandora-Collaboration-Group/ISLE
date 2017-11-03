#!/bin/bash

EXCLUDE=(
  'sites/all/modules/islandora_scholar/modules/bibutils/tests/data/RIS.txt'
)

containsElement () {
  local e
  for e in "${@:2}"; do [[ "$e" == "$1" ]] && return 0; done
  return 1
}

RETURN=0
FILES=`find -L $1 -name "*.info" -o -name "*.txt" -o -name "*.md"`
echo "Testing for files with DOS line endings..."
for FILE in $FILES
do
  if ! containsElement "$FILE" "${EXCLUDE[@]}"; then
    file $FILE | grep CRLF
    if [ $? == 0 ]
    then
      RETURN=1
    fi
  fi
done
exit $RETURN
