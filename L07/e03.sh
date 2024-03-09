#!/bin/bash

#$1 = directory, se numero argomenti passati da riga di comando != 1 -> errore
if [ $# -ne 1 ]; then
  exit 1
fi

for f in ls -r $1; do
    # estraggo il nome del file/directory
    base=$(basename "$f")
    # converto tutte le lettere maiuscole in minuscole
    new=$(echo "$base" | tr '[A-Z]' '[a-z]')
    # rinomino il file/directory
    mv "$f" "$1/$new"
done

exit 0