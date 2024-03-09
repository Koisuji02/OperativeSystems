#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define MAXCHAR 100
// numero di interi in ogni file (più comodo)
#define N_FILE 5

// tipo per i parametri da passare al thread
struct threadArgs {
    int *v;
    char *name;
};
typedef struct threadArgs arg_t;

// gestore degli errori per evitare di fare ogni volta perror e exit insieme
void errorExit(const char *errorMessage) {
    perror(errorMessage);
    exit(EXIT_FAILURE);
}

void *singleThread(void *params){
    int i;
    arg_t *parametro = (arg_t*) params;
    
    FILE* fin = fopen(parametro->name, "r");
    if (fin == NULL) errorExit("Errore apertura file input!");

    // prendo dal file gli interi e li metto nel vettore
    for(i = 0; i < N_FILE; i++){
        fscanf(fin, "%d\n", parametro->v[i]);
    }

    fclose(fin);
    return;
}


int main(int argc, char **argv){

    // numero di file e vettore di interi totale e della matrice di vettori locali
    int n, i, j, *tot, **v, k = 0, tmp, m, p;
    // usato alla fine della join per capire la terminazione dei vari thread
    void *exitstatus;
    // creazione e allocazione delle stringhe (tmp conterrà strAn per il thread)
    char *strA, *strB, *tmp;
    strA = (char*)malloc(MAXCHAR*sizeof(char));
    strB = (char*)malloc(MAXCHAR*sizeof(char));
    tmp = (char*)malloc(MAXCHAR*sizeof(char));

    // creazione del vettore di struct da passare ai thread
    arg_t * parametri;

    // creazione del vettore contenente i threadID e allocazione del vettore
    pthread_t *threadID = (pthread_t *) malloc(n * sizeof(pthread_t));

    // n = numero file ingresso (e quindi numero dei thread da generare)
    n = atoi(argv[1]);
    // strA = nome di "n" file di input (es. strA1.txt, strA2.txt, ..., strAn.txt)
    strcpy(strA, (char *)argv[2]);
    // strB = nome dell'unico file di uscita
    strcpy(strB, (char *)argv[3]);

    // apertura file uscita
    FILE* fout = fopen(strB, "w");

    // allocazione del vettore di struct da passare al thread
    parametri = (arg_t*)malloc(n*sizeof(arg_t));

    // allocazione del vettore di interi finale
    tot = (int*)malloc(n*N_FILE*sizeof(int));
    // e locale
    v = (int**)malloc(n*sizeof(int));
    for(i = 0; i < n; i++) v[i] = (int*)malloc(N_FILE*sizeof(int));

    for(i = 0; i < n; i++) {
        // nome file input da passare al thread dentro tmp
        sprintf(tmp, "%s%d.txt", strA, i+1);
        strcpy(parametri[i].name, tmp);
        parametri[i].v = v[i];
        pthread_create(&threadID[i], NULL, singleThread, (void*)parametri[i]);
    }

    // si fanno terminare i threads con una join, salvando gli exit status
    for(i = 0; i < n; i++) {
        pthread_join(threadID[i] , &exitstatus);
        for(j = 0; j < N_FILE; j++) {
            // il vettore totale avanza mentre i vettori locali finiscono dopo N_FILE
            tot[k] = v[i][j];
            k++;
            // sort della sequenza fino ad ora
            m = k;
            while(m >= 0) {
                for(p = 0; p < m; p++){
                    if(tot[m] < tot[p]) {
                        tmp = tot[m]; 
                        tot[m] = tot[p];
                        tot[p] = tmp;
                    }
                }
            }
        }
    }

    // stampa di tot su file uscita strB
    for(i = 0; i < (n*N_FILE); i++) fprintf(fout, "%d\n", tot[i]);

    // free tutte le cose allocate
    fclose(fout);
    free(strA);
    free(strB);
    free(tmp);
    free(threadID);
    free(v);
    free(tot);
    free(parametri);

    return 0;
}