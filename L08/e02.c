#include<stdio.h>
#include<unistd.h>
#include<assert.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

// ciascun processo viene creato e distrutto ad ogni iterazione del ciclo principale
// processi non ciclici
// semafori tramite pipe

// tipo pipe usato come int*
typedef int* pipe_t;

// creazione dei semafori
pipe_t semA, semBCD, semEF, semG, semH, semI;

// Creazione del semaforo (pipe)
pipe_t pipe_create(int n) {
    int i;
    // creo il semaforo (pipe), allocandolo e ne faccio la pipe(S) per creare la pipe
    pipe_t S = (pipe_t)malloc(2*sizeof(int));
    pipe((int)S);
    // faccio le signal (write del token sulla pipe)
    for(i=0; i<n; i++) pipe_signal(S);
    return S;
}

// Distruzione del semaforo
void pipe_destroy(pipe_t S){
    close(S[0]);
    close(S[1]);
    free(S);
}

// WAIT = read bloccante di un token (carattere) dalla pipe
void pipe_wait(pipe_t S){
    char token;
    read(S[0], (void*) &token, 1);
}

// SIGNAL = write non bloccante di un token dalla pipe
void pipe_signal(pipe_t S){
    char token = '*';
    write(S[1], (void*) &token, 1);
}

// La funzione spawn viene usata per generare processi figli concorrenti, che possono lavorare senza aspettarsi tra loro, passando ad ogni processo figlio la funzione che dovrà svolgere
void spawn(void (*f)(void)){
    // faccio fork per creare il processo figlio
    pid_t pid = fork();
    // se sono nel figlio faccio la funzione richiesta ed esco
    if(pid == 0){
        f();
        exit(0);
    }
}

// Processo A
void process_A(){
    int i;
    pipe_wait(semA);
    printf("A\n");
    sleep(2);
    // vado a B, C o D
    for(i=0; i<3; i++) pipe_signal(semBCD);
}

// Processo B
void process_B(){
    pipe_wait(semBCD);
    printf("B\n");
    sleep(2);
    // vado a I
    pipe_signal(semI);
}

// Processo C
void process_C(){
    int i;
    pipe_wait(semBCD);
    printf("C\n");
    sleep(2);
    // vado a E o F
    for(i=0; i<2; i++) pipe_signal(semEF);
}

// Processo D
void process_D(){
    pipe_wait(semBCD);
    printf("D\n");
    sleep(2);
    // vado a H
    pipe_signal(semH);
}

// Processo E
void process_E(){
    pipe_wait(semEF);
    printf("E\n");
    sleep(2);
    // vado a G
    pipe_signal(semG);
}

// Processo F
void process_F(){
    pipe_wait(semEF);
    printf("F\n");
    sleep(2);
    // vado a G
    pipe_signal(semG);
}

// Processo G
void process_G(){
    // aspetto da E e F
    pipe_wait(semG);
    pipe_wait(semG);
    printf("G\n");
    sleep(2);
    // vado a I
    pipe_signal(semI);
}

// Processo H
void process_H(){
    pipe_wait(semH);
    printf("H\n");
    sleep(2);
    // vado a I
    pipe_signal(semI);
}

// Processo I
void process_I(){
    // aspetto da B, G e H
    pipe_wait(semI);
    pipe_wait(semI);
    pipe_wait(semI);
    printf("I\n");
    sleep(2);
    // vado a A
    pipe_signal(semA);
}

int main(int argc, char **argv){

    // creazione dei semafori richiesti (1 per i figli sotto ogni processo)
    semA = pipe_create(1);
    semBCD = pipe_create(0);
    semEF = pipe_create(0);
    semG = pipe_create(0);
    semH = pipe_create(0);
    semI = pipe_create(0);

    while(1){
        // chiamo le varie spawn per ogni processo
        spawn(process_A);
        spawn(process_B);
        spawn(process_C);
        spawn(process_D);
        spawn(process_E);
        spawn(process_F);
        spawn(process_G);
        spawn(process_H);
        process_I();
    }
    
    // distruzione di tutti i semafori
    pipe_destroy(semA);
    pipe_destroy(semBCD);
    pipe_destroy(semEF);
    pipe_destroy(semG);
    pipe_destroy(semH);
    pipe_destroy(semI);

    return 0;
}
