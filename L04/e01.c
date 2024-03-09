#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

int main() {
    int n = 1;
    while(n!=0){
        fprintf(stdin, "Inserisci il numero intero:\n");
        fscanf(stdin, "%d ", n);
        if((n%2)==0) pari(n);
        else dispari(n);
    }
    return 0;
}

int pari(int n) {
    fprintf(stdout, "%d\n", n);
    return 0;
}

int dispari(int n) {
    fprintf(stderr, "%d\n", n);
    return 0;
}

// per la seconda parte dove chiede di fare ridirezione su file input, output e err, posso compilare il file.c con gcc file.c -o a.o e poi eseguo il file con:
// ./a.o < inFile.txt per la ridirezione di stdin
// ./a.o > pariFile.txt per la ridirezione di stdout
// ./a.o 2> dispariFile.txt per la ridirezione di stderr