#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define N 100
// variabile globale flag per la sincronizzazione dei 2 processi consumatore e produttore
int flag = 0;
pid_t producer_pid, consumer_pid;

// gestore dei segnali per la sincronizzazione tra fratelli consumatore-produttore
void signal_handler(int signo, pid_t pid) {
    if(signo == SIGUSR1 && pid == producer_pid) flag = 1;
    else if (signo == SIGUSR2 && pid == consumer_pid) flag = 0;
    return;
}

int main() {

    char input_string[N], buffer[N];

    int pipe_fd[2];
    // Creazione della pipe
    pipe(pipe_fd);

    // impostazione del signal handler
    if (signal(SIGUSR1, signal_handler) == SIG_ERR) {
        printf("Signal Handler Error.\n");
        return 1;
    }
    if (signal(SIGUSR2, signal_handler) == SIG_ERR) {
        printf("Signal Handler Error.\n");
        return 1;
    }

    // Creazione del processo produttore
    producer_pid = fork();

    if (producer_pid < 0) exit(EXIT_FAILURE);

    if (producer_pid == 0) { // sono nel figlio produttore

        close(pipe_fd[0]); // Chiude l'estremità di lettura della pipe
        while (1) {

            while (flag == 0) {
                pause();  // Sospendi il processo finché non arriva un segnale
            }

            printf("Inserisci una stringa (o 'end' per terminare): ");
            fgets(input_string, N, stdin);

            // Scrive sulla pipe fino a quando non riceve "end"
            write(pipe_fd[1], input_string, strlen(input_string));
            if (strcmp(input_string, "end") == 0) {
                break;
            }
            
            kill(consumer_pid, SIGUSR2);

        }


        close(pipe_fd[1]); // Chiude l'estremità di scrittura della pipe
        exit(EXIT_SUCCESS);

    } else { // sono nel padre

        // Creazione del processo consumatore
        consumer_pid = fork();

        if (consumer_pid < 0) exit(EXIT_FAILURE);

        if (consumer_pid == 0) { // sono nel figlio consumatore

            close(pipe_fd[1]); // Chiude l'estremità di scrittura della pipe
            while (1) {

                while (flag == 1) {
                pause();  // Sospendi il processo finché non arriva un segnale
                
                }
                // Legge dalla pipe fino a quando non riceve "end"
                ssize_t bytes_read = read(pipe_fd[0], buffer, sizeof(buffer));
                if (bytes_read <= 0 || strcmp(buffer, "end\n") == 0) {
                    break;
                }

                // Converti la stringa in maiuscolo e la stampa
                for (int i = 0; i < bytes_read; i++) {
                    buffer[i] = toupper(buffer[i]);
                }
                printf("Consumatore: %s", buffer);

                kill(producer_pid, SIGUSR1);
            }

            close(pipe_fd[0]); // Chiude l'estremità di lettura della pipe
            exit(EXIT_SUCCESS);

        } else { // sono nel padre

            close(pipe_fd[0]); // Chiude l'estremità di lettura della pipe
            close(pipe_fd[1]); // Chiude l'estremità di scrittura della pipe

            // Attendi che entrambi i figli terminino
            waitpid(producer_pid, NULL, 0);
            waitpid(consumer_pid, NULL, 0);

        }
    }
    return 0;
}