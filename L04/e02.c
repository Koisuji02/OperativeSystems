#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define M 10

int main() {
    int i,j, n1, n2;
    char vet[2];
    pid_t pid;
    // apro il file
    FILE* fin = fopen("input.txt", "r");
    if (fin == NULL) {
        fprintf(stdin, "Errore apertura file!");
        exit(EXIT_FAILURE);
    }
    
    // realizzo il bubble sort
    for(i = 0; i < M-1; i++){
        for(j = 0; j < M-1-i; j++){
            // se sono nel figlio
            if((pid = fork())==0){
                // sincronizzazione tra figli per evitare che facciano casino sul file
                sleep(j+1);
                // sposto il cursore del file di j posizioni dall'inizio del file
                lseek(fin, j * sizeof(int), SEEK_SET);
                // leggo i 2 elementi dal file che si salvano nel buffer vet
                read(fin, vet, 2);
                // confronto i 2 interi letti
                n1 = (int)vet[0];
                n2 = (int)vet[1];
                // se devo effettuare lo scambio, mi
                if(n1 > n2){
                    vet[0] = (char)n2;
                    vet[1] = (char)n1;
                    lseek(fin, j * sizeof(int), SEEK_SET);
                    write(fin, vet, 2);   
                }
            }
        }
    }
    while(wait((int*)0) != pid);
    fclose(fin);
    return 0;
}