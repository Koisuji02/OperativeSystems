#!/bin/bash

#$1 = nome processo e $2 = intervallo di tempo, se numero argomenti passati da riga di comando != 2 -> errore
if [ $# -ne 2 ]; then
    exit 1
fi

# numero di volte in cui lo trovo a zombie (se arrivo a 5 termina)
counter=0

# ciclo infinito che si chiude una volta raggiunti 5 volte di zombie
while [ 1 -eq 1 ]; do

    # prendo la riga della ps con nome cercato
    line=$(ps -el | grep $1)

    # se l'output della grep è 0, ovvero ho trovato il processo cercato
    if [ $? -eq 0 ]; then 
        # prendo il pid del processo cercato (con la cut prendo il secondo campo della linea, separato da spazio)
        pid=$(echo $line | cut -d ' ' -f 2)
        # prendo lo stato del processo cercato
        stato=$(echo $line | cut -d ' ' -f 11)

        # se il processo si trova nello stato zombie, aumento il counter di 1
        if [ $stato == "Z" ]; then
            counter=$(($counter+1))
        fi

        # se sono arrivato a 5
        if [ $counter -eq 5 ]; then
            # uccido il processo
            kill -9 $pid
            exit 0
        fi
    fi
done