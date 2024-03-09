#!/bin/bash

# se il numero di parametri ricevuto sulla linea di comando != 3 -> errore
if [ $# -ne 3 ]; then
  exit 1
fi

# per ogni file trovato nella directory cercata con la find, fare l'operazione dell'altro es
for f in `find $1 -type f`; do
  grep -H -n "$2 *(.*)" $f | sort -r -t ':' -k 1 -k 2n > $3  
done

exit 0