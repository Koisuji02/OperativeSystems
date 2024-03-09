#!/bin/bash

# non passato da riga di comando -> prendilo da tastiera
if [ $# -eq 0 ]; then
  echo "Inserisci il nome del file:"
  read filename
else
  # passato da riga di comando
  filename=$1
fi

# 2 vettori (uno per le stringhe e uno per le loro frequenze assolute)
declare -a stringhe
declare -a frequenze

# Leggi le stringhe dal file e aggiorna i vettori
for word in $(cat "$filename"); do

  flag=false

  # Cerca la parola nei vettori (doppia parentesi per aritmetica in Bash)
  for ((i=0; i<${#stringhe[@]}; i++)); do
    #!se la trovo, metto flag = true e incremento di 1 il numero di frequenze di quella stringa (ed esco dal ciclo)
    if [ "${stringhe[i]}" == "$word" ]; then
      flag=true
      frequenze[i]=$((frequenze[i]+1))
      break
    fi
  done

  #!se flag = false, ovvero non ho trovato la parola, devo aggiungerla alle stringhe e imposto la sua frequenza ad 1
  if [ "$flag" == false ]; then
    stringhe+=("$word")
    frequenze+=("1")
  fi
done

# Visualizza le frequenze
for ((i=0; i<${#stringhe[@]}; i++)); do
  echo "${stringhe[i]} ${frequenze[i]}"
done

exit 0