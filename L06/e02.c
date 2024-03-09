#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define MAXCHAR 100

// gestore degli errori per evitare di fare ogni volta perror e exit insieme
void errorExit(const char *errorMessage) {
    perror(errorMessage);
    exit(EXIT_FAILURE);
}

void *singleThread(void *names){
    int *v, rows, i, j, tmp;
    // apro il file names[0] in lettura e il file names[1] in scrittura
    FILE* fin = fopen(names[0], "r"), fout = fopen(names[1], "w");
    if (fin == NULL) errorExit("Errore apertura file input!");
    if (fout == NULL) errorExit("Errore apertura file output!");
    
    // 1^ riga del file con il numero di righe (ovvero di interi) che uso per allocare il vettore di interi
    fscanf(fin, "%d", rows);
    v = (int*)malloc(rows*sizeof(int));
    // prendo dal file gli interi e li metto nel vettore
    for(i = 0; i < rows; i++){
        fscanf(fin, "%d\n", v[i]);
    }

    // faccio l'ordinamento del vettore
    for(i = 0; i < rows; i++) {
        for(j = 0; j < i; j++) {
            if(v[i] < v[j]) {
                tmp = v[i]; 
                v[i] = v[j];
                v[j] = tmp;
            }
        }
    }

    // stampa sul file di uscita
    for(i = 0; i < rows; i++) fprintf(fout, "%d\n", v[i]);

    // fclose e free
    fclose(fin);
    fclose(fout);
    free(v);

    return;
}


int main(int argc, char **argv){
    int n, i;
    // usato alla fine della join per capire la terminazione dei vari thread
    void *exitstatus;
    // creazione e allocazione delle stringhe (strA, strB e il vettore che userò per passarle al Thread)
    char *strA, *strB, *strDaPassare[2];
    strA = (char*)malloc(MAXCHAR*sizeof(char));
    strB = (char*)malloc(MAXCHAR*sizeof(char));
    for(i = 0; i < 2; i++) strDaPassare[i] = (char*)malloc(MAXCHAR*sizeof(char));

    // creazione del vettore contenente i threadID e allocazione del vettore
    pthread_t *threadID = (pthread_t *) malloc(n * sizeof(pthread_t));

    // n = numero file ingresso e uscita (e quindi numero dei thread da generare)
    n = atoi(argv[1]);
    // strA = nome di "n" file di input (es. strA1.txt, strA2.txt, ..., strAn.txt)
    strcpy(strA, (char *)argv[2]);
    // strB = nome di "n" file di output (es. strB1.txt, strB2.txt, ..., strBn.txt)
    strcpy(strB, (char *)argv[3]);
    for(i = 0; i < n; i++) {
        // nome file input da passare al thread
        sprintf(strDaPassare[0], "%s%d.txt", strA, i+1);
        // nome file output da passare al thread
        sprintf(strDaPassare[1], "%s%d.txt", strB, i+1);
        pthread_create(&threadID[i], NULL, singleThread, (void*)strDaPassare);
    }

    // si fanno terminare i threads con una join, salvando gli exit status
    for(i = 0;i < n; i++) {
        pthread_join(threadID[i] , &exitstatus);  
    }

    // free tutte le cose allocate
    free(strA);
    free(strB);
    for(i = 0; i < 2; i++) free(strDaPassare[i]);
    free(threadID);

    return 0;
}