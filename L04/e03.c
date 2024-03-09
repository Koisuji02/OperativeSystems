#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

// il flag mi serve per sincronizzare alternativamente padre e figlio, quindi alternandoli con flag = 1 (figlio) e flag = 0 (padre)
int flag = 0;

// gestore del segnale in modo da inizializzare flag a 1 per il figlio
void sig_handler(int signo) {
    if (signo == SIGUSR1)
        flag = 1;
}

int main() {
    pid_t pid;
    // Imposta il gestore del segnale per il padre
    signal(SIGUSR1, sig_handler);

    // faccio fork e creo figlio
    pid = fork();
    // se fork errata, allora errore (classica gestione di fork errata)
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    // se invece fork ha funzionato e sono nel figlio
    else if (pid == 0) {
        while (1) {
            son();
        }
    } else { // se invece sono nel padre
        while (1) {
            father(pid);
        }
    }
    return 0;
}

void son() {
    if (flag == 1) {
                printf("Child Woke-up\n");
                flag = 0; // flag viene rimesso a 0 per stampare alternativamente e sincronizzare il padre e il figlio
                // sleep aggiunta per evitare corse critiche
                sleep(1);
                // invio segnale al padre (uso getppid perchè prendo il pid del padre che mi ha generato)
                kill(getppid(), SIGUSR1);
            }
}

void father(pid_t pid) {
    if (flag == 0) {
        printf("Father Woke-up\n");
        flag = 1;
        sleep(1);
        kill(pid, SIGUSR1);
    }
}