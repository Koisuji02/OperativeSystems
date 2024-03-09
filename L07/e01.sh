#!/bin/bash

# se il numero di parametri ricevuto sulla linea di comando != 3 -> errore
if [ $# -ne 3 ]; then
  exit 1
fi

# cerco ricorsivamente nella cartella la funzione nei vari file; dei risultati ordino in base al campo 1 e poi il 2 (campi separati da ':') e metto ciò che trovo come output sul file di output
grep -H -r "$2 *(.*)" $1 | sort -t ':' -k 1 -k 2rn > $3

exit 0