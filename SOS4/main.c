#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define COME_MIN_TIME 1
#define COME_MAX_TIME 3
#define CUT_MIN_TIME 2
#define CUT_MAX_TIME 5

int Cli;                        //identifikator soketa
struct sockaddr_in for_addr;    //strukt for soket
int customerCount;

int CheckSpace()
{
    int Data = 1;
    send(Cli,&Data,sizeof(Data),0);
    printf("CheckSpace\n");
    recv(Cli,&Data,sizeof(Data),0);
    return Data;        //or 0 or 1
}
int PostCust()            //sdelat semafor costumer post
{
    int Data = 2;
    printf("PostCust\n");
    send(Cli,&Data,sizeof(Data),0);
    recv(Cli,&Data,sizeof(Data),0);
    return Data;

}
int WaitBur()                        //sdelat sem_wait for barber
{
    int Data = 3;
    send(Cli,&Data,sizeof(Data),0);
    printf("Waitbur\n");
    recv(Cli,&Data,sizeof(Data),0); /*pause programma poka not take control of the semafor */
    return Data;
}

void *customer(void* unused)
{
   if (CheckSpace())
   {
    printf("created customer\n");
    PostCust();
    WaitBur();
   }
   else{
    printf("go out\n");
  }
  return NULL;
}
int main(int argc, char * argv[])
{
    srand(time(NULL));
//      setbuf(stdout, NULL); // получаем из аргов значения
//      if (argc != 2)
//      {
//        return 1;
//      }

//    customerCount = atoi(argv[1]);
    customerCount = 10;
      printf("Customer count: %i\n", customerCount);
    Cli = socket(AF_INET,SOCK_STREAM,0);    //create soket
    for_addr.sin_family = AF_INET;        //режим "интернет"
    for_addr.sin_addr.s_addr = inet_addr("192.168.181.224");    //указывем ip сервера
    for_addr.sin_port  = htons(8010);
    connect(Cli, (const struct sockaddr*)&for_addr, sizeof (for_addr) );    //коннектимся
    pthread_t threadCustomers[customerCount];
    for (int i = 0; i < customerCount; i++)
      {
        pthread_create(&(threadCustomers[i]), NULL, customer, NULL);
        sleep(rand() % (COME_MAX_TIME - COME_MIN_TIME) + COME_MIN_TIME);
      }
    return 0;                //возврат 0, если true

}
