#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void binary(int *, int);

int main (int argc, char *argv[]) { // main uguale a prima
  int n, *vet;
  n = atoi (argv[1]);
  vet = (int *)malloc(n * sizeof(int));
  if (vet == NULL) {
    printf("Allocation Error.\n");
    exit(1);
  }
  printf("Binary Numbers:\n");
  binary(vet, n);
  free(vet);
  return 0;
}

void binary (int *vet, int n) {
  int i, j;
  pid_t pid1, pid2;
  // ciclo for al posto della funzione che si richiama
  for (i = 0; i <= n; i++) {
    // condizione di terminazione
    if (i == n) {
      for (j = 0; j < n; j++) {
        printf("%d", vet[j]);
      }
      printf("\n");
      exit (0);
    } else {
      // caso di non terminazione, quindi corpo della funzione
      pid1 = fork ();       
      if (pid1 == 0) {
        // figlio1, ovvero mette 0
        vet[i] = 0;
      } else {
        // padre
        pid2 = fork ();       
        if (pid2 == 0) {
          // figlio2, ovvero mette 1
          vet[i] = 1;
        } else { // padre che quindi fa le 2 wait
          wait ((int *) 0);
          wait ((int *) 0);
          exit (1);
        }
      }
    }
  }
  return 0;
}