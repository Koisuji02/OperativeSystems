int atomicIncrement (int *var) {
  int tmp = *var;
  *var = tmp + 1;
  return (tmp);
}

// variabile globale per l'ordine di prenotazione per accesso alla SC
int ticketNumber;
// variabile globale per indicare il processo a cui spetta l'accesso alla SC
int turnNumber;

// inizializzare le 2 variabili
int init() {
    ticketNumber = 0;
    turnNumber = 0;
    return 0;
}

// Funzione di lock
void lock(pid_t pid) {
    // ottengo il turno dalla funzione atomicIncrement passando la variabile globale ticketNumber
    int turn = atomicIncrement(&ticketNumber);
    // aspetto fino al turno giusto
    while (turn != turnNumber);
    // Accesso alla SC
    printf("Processo con pid=%d è entrato nella SC.\n", (int)pid);
}

// Funzione di unlock
void unlock(pid_t pid){
    // incremento il turno
    atomicIncrement(&turnNumber);
    // Uscita dalla SC
    printf("Processo con pid=%d è uscito dalla SC.\n", (int)pid);
}