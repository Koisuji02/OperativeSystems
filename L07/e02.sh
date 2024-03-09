#!/bin/bash

#$1 = nome del file, se numero argomenti passati da riga di comando != 1 -> errore
if [ $# -ne 1 ]; then
  exit 1
fi

max_length=0

# leggo ogni linea del file e trovo quella con lunghezza massima
while read -r line; do
    # prendo la lunghezza della singola riga con #line e la metto in length
    length=${#line}
    # se la lunghezza locale della riga è maggiore della massima finora, la metto come massima
    if [ $length -gt $max_length ]; then
        max_length = $length
    fi
# finito, reindirizzo input dal file di input
done < $1

# numero righe del file di input
num_righe=$(wc -l < $1)

# visualizzo i risultati
echo "Num caratteri riga più lunga: $maxlength"
echo "Num righe nel file: $num_righe"

exit 0