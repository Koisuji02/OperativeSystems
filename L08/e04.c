#include<stdio.h>
#include<unistd.h>
#include<assert.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

sem_t semA, semB, semC, semD, semE, semF, semG, semH, semI;
pthread_t tidA, tidB, tidC, tidD, tidE, tidF, tidG, tidH, tidI;

// Processo A
void* process_A(void* arg){
    // ciclico
    while(1) {
        sem_wait(&semA);
        printf("A\n");
        sleep(2);
        // vado a B, C e D
        sem_post(&semB);
        sem_post(&semC);
        sem_post(&semD);
    }
}

// Processo B
void* process_B(void* arg){
    // ciclico
    while(1) {
        sem_wait(&semB);
        printf("B\n");
        sleep(2);
        // vado a I
        sem_post(&semI);
    }
}

// Processo C
void* process_C(void* arg){
    // ciclico
    while(1) {
        sem_wait(&semC);
        printf("C\n");
        sleep(2);
        // vado a E e F
        sem_post(&semE);
        sem_post(&semF);
    }
}

// Processo D
void* process_D(void* arg){
    // ciclico
    while(1) {
        sem_wait(&semD);
        printf("D\n");
        sleep(2);
        // vado a H
        sem_post(&semH);
    }
}

// Processo E
void* process_E(void* arg){
    // ciclico
    while(1) {
        sem_wait(&semE);
        printf("E\n");
        sleep(2);
        // vado a G
        sem_post(&semG);
    }
}

// Processo F
void* process_F(void* arg){
    // ciclico
    while(1) {
        sem_wait(&semF);
        printf("F\n");
        sleep(2);
        // vado a G
        sem_post(&semG);
    }
}

// Processo G
void* process_G(void* arg){
    // ciclico
    while(1) {
        // aspetto E e F
        sem_wait(&semG);
        sem_wait(&semG);
        printf("G\n");
        sleep(2);
        // vado a I
        sem_post(&semI);
    }
}

// Processo H
void* process_H(void* arg){
    // ciclico
    while(1) {
        sem_wait(&semH);
        printf("H\n");
        sleep(2);
        // vado a I
        sem_post(&semI);
    }
}

// Processo I
void* process_I(void* arg){
    // ciclico
    while(1) {
        // aspetto B, G e H
        sem_wait(&semI);
        sem_wait(&semI);
        sem_wait(&semI);
        printf("I\n");
        sleep(2);
        // vado a A
        sem_post(&semA);
    }
}

// main come prima, ma tolgo il ciclo sui thread, non richiesto
int main(int argc, char **argv){

  // inizializzo i semafori POSIX
  sem_init(&semA, 0, 1);
  sem_init(&semB, 0, 0);
  sem_init(&semC, 0, 0);
  sem_init(&semD, 0, 0);
  sem_init(&semE, 0, 0);
  sem_init(&semF, 0, 0);
  sem_init(&semG, 0, 0);
  sem_init(&semH, 0, 0);
  sem_init(&semI, 0, 0);

  // creo i vari thread a cui passo il t_id e la funzione che devono svolgere, senza però farne un ciclo (1 sola volta richiesto)
  pthread_create(&tidA, NULL, process_A, NULL);
  pthread_create(&tidB, NULL, process_B, NULL);
  pthread_create(&tidC, NULL, process_C, NULL);
  pthread_create(&tidD, NULL, process_D, NULL);
  pthread_create(&tidE, NULL, process_E, NULL);
  pthread_create(&tidF, NULL, process_F, NULL);
  pthread_create(&tidG, NULL, process_G, NULL);
  pthread_create(&tidH, NULL, process_H, NULL);
  process_I(NULL);
  
  // distruggo tutti i semafori
  sem_destroy(&semA);
  sem_destroy(&semB);
  sem_destroy(&semC);
  sem_destroy(&semD);
  sem_destroy(&semE);
  sem_destroy(&semF);
  sem_destroy(&semG);
  sem_destroy(&semH);
  sem_destroy(&semI);
  
  return 0;
}