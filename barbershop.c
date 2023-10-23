%%writefile barbershop.c

//bibliotecas
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // utilizado para sleep()

// quantidade de cadeiras da barbearia
#define NUM_CHAIRS 4
#define NUM_CLIENTS 10

// semáforos
sem_t customers;    // cliente: entrou na loja
sem_t barber;       // barbeiro: acordado ou dormindo
sem_t customerDone; // cliente: corte finalizado ou não
sem_t barberDone;   // barbeiro: livre ou cortando cabelo
sem_t mutex;        // protege as variáveis

// variáveis
int clients = 0; // clientes aguardando
int totalClients = 0; // clientes já atendidos

// rotina do cliente
void *customer_routine(void *arg) {

  int id = *((int *)arg); // identificador do cliente atual
  sem_wait(&mutex); // protege as variações em "clients"

  if (clients < NUM_CHAIRS) {
    clients++; // cliente pode entrar, atualiza o número de clientes na barbearia

    printf("Cliente %d entrou na barbearia, %d clientes aguardando\n", id, clients);

    sem_post(&mutex);     // libera o mutex

    sem_post(&customers); // avisa chegada de cliente
    sem_wait(&barber);    // espera o sinal do barbeiro

    // getHairCut();

    sem_post(&customerDone); // avisa o cliente que o corte foi finalizado
    sem_wait(&barberDone); // aguarda o banco estar vazio

  } else {
    printf("Barbearia Hélio's Barber Shop cheia, cliente %d foi embora\n", id);
    sem_post(&mutex); // libera o mutex sem atualizar o numero de clientes
  }
   pthread_exit(NULL);
}

// rotina do barbeiro
void *barber_routine(void *agr) {

  // deixando o barbeiro em espera ativa
  while (1) {
    sem_wait(&mutex);
    if(clients==0){
      printf("Sem clientes, Barbeiro Hélio está dormindo\n");
    }
    sem_post(&mutex);

    sem_wait(&customers); // aguarda chegada de cliente
    sem_post(&barber); // sinaliza que está pronto

    sem_wait(&mutex);
      clients--;
    sem_post(&mutex);

    if(clients >= 0){
      printf("Barbeiro Hélio cortando cabelo, %d clientes aguardando\n", clients);
      // cutHair();
      sleep(rand() % 3 + 3); //simula o tempo de corte

      printf("Barbeiro Hélio terminou o corte\n");
      totalClients++;

      sem_wait(&customerDone);
      sem_post(&barberDone);
    }
    else
      break;
  }
  pthread_exit(NULL);
}

// função principal
int main() {

  int i, clientsThread;
  pthread_t barber_t, customers_t;

  // inicialização dos semáforos
  sem_init(&mutex, 0, 1); // inicializando o mutex desbloqueado
  sem_init(&customers, 0, 0);
  sem_init(&barber, 0, 0);
  sem_init(&customerDone, 0, 0);
  sem_init(&barberDone, 0, 0);

  // cria a thread do barbeiro
  pthread_create(&barber_t, NULL, barber_routine, NULL);

  // cria as threads dos clientes (aguardando entre uma e outra)
  for (i = 0; i < NUM_CLIENTS; i++) {
    clientsThread = i + 1;
    pthread_create(&customers_t, NULL, customer_routine, (void *) &clientsThread);
    sleep(rand() % 3 +1); //Comente essa linha e descomente a linha abaixo para forçar a soneca do barbeiro
    // sleep(5);
  }

  // aguarda a finalização das threads dos clientes
  while(clients>=0){
    sem_post(&customers);
    sem_post(&customerDone);
    sleep(2);
  }

  // mensagens de finalização
  printf("Hoje o Barbeiro Hélio atendeu %d clientes\n", totalClients);
  printf("Ele terminou o serviço do dia e está indo pra casa!\n");

  // finaliza a thread do barbeiro
  pthread_cancel(barber_t);
  pthread_join(barber_t, NULL);

  // destruição dos semáforos (free)
  sem_destroy(&mutex);
  sem_destroy(&customers);
  sem_destroy(&barber);
  sem_destroy(&customerDone);
  sem_destroy(&barberDone);

  return 0;
}
