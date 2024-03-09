#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_STR 100
#define LEN_STR 100

void allocateMemory(char ***str);
void readFile(FILE *fp, int *n, char ***str);
void executeSystem(char **str, int n);
void doExec(char **str, int n);
void myExecute(char **cmd);

int main(int argc, char *argv[]){
    FILE *fin;
    char *filename, **str;
    int n = 0;
    // se ho un numero di argomenti passati da riga di comando diverso da 2, allora errore
    if(argc != 2){
        printf("Errore: numero di argomenti errato!\n");
        return(1);
    }
    // prendo il nome del file da argv[1]
    filename= argv[1];
    // apro il file
    fin = fopen(filename, "r");
    if(fin == NULL){
        printf("Errore: impossibile aprire il file!\n");
        return(1);
    }
    // alloco la memoria per il vettore di stringhe
    allocateMemory(&str);
    // leggo il file
    readFile(fin, &n, &str);
    // faccio System
    executeSystem(str, n);
    // faccio Exec
    doExec(str, n);

    return 0;
}


void allocateMemory(char ***str) {
    // alloco il vettore di stringhe
    str = (char)malloc(MAX_STR * sizeof(char));
    if (*str == NULL) {
        fprintf(stderr, "Errore nell'allocazione di memoria per il vettore di stringhe.\n");
        exit(EXIT_FAILURE);
    }
    // alloco le singole stringhe
    for (int i = 0; i < MAX_STR; i++) {
        (str)[i] = (char)malloc(LEN_STR * sizeof(char));
        if ((*str)[i] == NULL) {
            fprintf(stderr, "Errore nell'allocazione di memoria per la stringa %d.\n", i);
            exit(EXIT_FAILURE);
        }
    }
}
void readFile(FILE *fp, int *n, char ***str){
    // lettura singole righe da file
    while (fgets((*str)[*n], LEN_STR, fp) != NULL) {
        (*n)++;
    }
}

void executeSystem(char **str, int n){
    // eseguo i comandi con la system
    for(int i=0; i<n; i++){
        printf("Executing system... %s\n", str[i]);
        if(system(str[i])!= -1){
            printf("System executed\n\n");
        }
    }
}

void doExec(char **str, int n){
    char **cmd, *token,  delimiters[] = " \n";
    allocateMemory(&cmd);
    token = (char*) malloc(LEN_STR);
    // uso strtok per prendere i singoli campi dei comandi per darli alla exec
    for(int i=0; i<n; i++){
        token = strtok(str[i], delimiters);
        int indice = 0;
        // qui uso strtok con NULL per tenere l'indirizzo preso prima ed avanzare da lì
        while (token != NULL) {
            cmd[indice] = strdup(token);
            token = strtok(NULL, delimiters);
            indice++;
        }
        cmd[indice] = NULL;
        myExecute(cmd);
        printf("Exec done\n\n");

    }
    
}

void myExecute(char **cmd){
    // faccio la fork
    int pid = fork();
    // se sono nel figlio faccio la exec
    if(pid == 0){
        printf("Running execvp... %s \n", cmd[0]);
        // campi della exec passati dal vettore di stringhe cmd
        execvp(cmd[0], cmd);
    }else{ // se sono nel padre, faccio solo la sleep
        sleep(3);
    }
}