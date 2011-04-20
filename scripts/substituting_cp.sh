#!/bin/bash

set -e 

error ()
{
	echo "Error:" "$@"
	exit 1
}

[[ $# -eq 2 ]] || error "$0 expects exactly two arguments"

SRC_FILE="$1"
DEST_FILE="$2"

if [ -d "$DEST_FILE" ]
then
	DEST_FILE="$DEST_FILE/$(basename "$SRC_FILE")"
fi

sed \
	-e 's/{{VERSION}}/'"${VERSION}"'/g' \
	-e 's/{{EMAIL}}/'"${EMAIL}"'/g' \
	-e 's@{{URL}}@'"${URL}"'@g' \
	-e 's@{{NAME}}@'"${NAME}"'@g' \
	"$SRC_FILE" >"$DEST_FILE"
