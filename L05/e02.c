#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define N 100

int main() {
    
    char buffer_a[N], buffer_b[N];
    pid_t pid; //pid del figlio
    int flag = 1, i = 0;

    // pipe
    int pipe_a[2], pipe_b[2];
    pipe(pipe_a);
    pipe(pipe_b);

    pid = fork();
    if (pid < 0){
        perror("Errore nella fork!");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0){ // sono nel figlio

        close(pipe_b[0]);
        close(pipe_a[1]);
        while(flag){
            // figlio fa la read bloccante in attesa del padre e stampa
            read(pipe_a[0], buffer_a, N);
            fprintf(stdout, "I'm the child (PID = %d)\n", pid);
            // condizione di terminazione imposta dal padre
            if (strcmp(buffer_a, "end\n") == 0) flag = 0;
            write(pipe_b[1], "ciao\n", N);
        }
        close(pipe_b[1]);
        close(pipe_a[0]);
        exit(0); // il figlio termina

    } else{

        close(pipe_b[1]);
        close(pipe_a[0]);
        while(flag){
            fprintf(stdout, "I'm the father(PID = %d)\n", getppid());
            // per far terminare il programma, dopo 5 giri scrivo end sulla pipe da cui legge il figlio
            if (i != 5) write(pipe_a[1], "ciao\n", N);
            else {
                write(pipe_a[1], "end\n", N);
                flag=0;
            }
            i++;
            read(pipe_b[0], buffer_b, N);
        }
        close(pipe_b[0]);
        close(pipe_a[1]);
        waitpid(pid, NULL, 0); // aspetta che il figlio termini

    }

    return 0;
}