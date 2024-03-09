#include<stdio.h>
#include<unistd.h>
#include<assert.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

// ciascun thread viene creato e distrutto ad ogni iterazione del ciclo principale
// thread non ciclici
// semafori tramite POSIX

sem_t semA, semBCD, semEF, semG, semH, semI;
pthread_t tidA, tidB, tidC, tidD, tidE, tidF, tidG, tidH, tidI;

// Processo A
void* process_A(void* arg){
  int i;
  // faccio partire da A (1° nodo del grafo)
  sem_wait(&semA);
  printf("A\n");
  sleep(2);
  // vado a B, C o D
  for(i=0; i<3; i++) sem_post(&semBCD);
  return NULL;
}

// Processo B
void* process_B(void* arg){
  sem_wait(&semBCD);
  printf("B\n");
  sleep(2);
  // vado a I
  sem_post(&semI);
  return NULL;
}

// Processo C
void* process_C(void* arg){
  int i;
  sem_wait(&semBCD);
  printf("C\n");
  sleep(2);
  // vado a E o F
  for(i=0; i<2; i++) sem_post(&semEF);
  return NULL;
}

// Processo D
void* process_D(void* arg){
  sem_wait(&semBCD);
  printf("D\n");
  sleep(2);
  // vado a H
  sem_post(&semH);
  return NULL;
}

// Processo E
void* process_E(void* arg){
  sem_wait(&semEF);
  printf("E\n");
  sleep(2);
  // vado a G
  sem_post(&semG);
  return NULL;
}

// Processo F
void* process_F(void* arg){
  sem_wait(&semEF);
  printf("F\n");
  sleep(2);
  // vado a G
  sem_post(&semG);
  return NULL;
}

// Processo G
void* process_G(void* arg){
    // aspetto E e F
    sem_wait(&semG);
    sem_wait(&semG);
    printf("G\n");
    sleep(2);
    sem_post(&semI);
    return NULL;
}

// Processo H
void* process_H(void* arg){
  sem_wait(&semH);
  printf("H\n");
  sleep(2);
  // vado a I
  sem_post(&semI);
  return NULL;
}

// Processo I
void* process_I(void* arg){
  // aspetto B, G e H
  sem_wait(&semI);
  sem_wait(&semI);
  sem_wait(&semI);
  printf("I\n");
  sleep(2);
  // vado a A
  sem_post(&semA);
  return NULL;
}


int main(int argc, char **argv){
  
  // inizializzo i semafori POSIX
  sem_init(&semA, 0, 1);
  sem_init(&semBCD, 0, 0);
  sem_init(&semEF, 0, 0);
  sem_init(&semG, 0, 0);
  sem_init(&semH, 0, 0);
  sem_init(&semI, 0, 0);

  while(1){
    // creo i vari thread a cui passo il t_id e la funzione che devono svolgere
    pthread_create(&tidA, NULL, process_A, NULL);
    pthread_create(&tidB, NULL, process_B, NULL);
    pthread_create(&tidC, NULL, process_C, NULL);
    pthread_create(&tidD, NULL, process_D, NULL);
    pthread_create(&tidE, NULL, process_E, NULL);
    pthread_create(&tidF, NULL, process_F, NULL);
    pthread_create(&tidG, NULL, process_G, NULL);
    pthread_create(&tidH, NULL, process_H, NULL);
    process_I(NULL);
  }
  
  // distruggo tutti i semafori
  sem_destroy(&semA);
  sem_destroy(&semBCD);
  sem_destroy(&semEF);
  sem_destroy(&semG);
  sem_destroy(&semH);
  sem_destroy(&semI);
  
  return 0;
}
