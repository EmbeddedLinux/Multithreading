
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>


#define NUM_OF_THREAD 10

pthread_mutex_t MyMutex;
int buffer[10];
int count =0 ;

sem_t SemEmpty;
sem_t SemFull;

void *Producer(void *args)
{
   while(1){

      pthread_mutex_lock(&MyMutex);
      int x = rand() %100;      
      printf("Producing X = %d \n" , x);
      sem_wait(&SemEmpty);
      buffer[count] = x;
      count++;
      sem_post(&SemFull);
      sleep(1);
      pthread_mutex_unlock(&MyMutex);
   }
}

void *Consumer(void *args)
{
   while(1)
   {
      sem_wait(&SemFull);
      int y = buffer[count -1];
      printf("Conusimg Y = %d\n" , y);
      count--;
      sem_post(&SemEmpty);      
      sleep(1);
      pthread_mutex_unlock(&MyMutex);
   }

}

int main() 
{
   // printf() displays the string inside quotation
   srand(time(NULL));
   printf("Hello, World! \n");
   pthread_t t1[2];
   sem_init(&SemEmpty, 0 ,10);
   sem_init(&SemFull,0,0 );
   int i;
   pthread_mutex_init(&MyMutex, NULL);

   for (i =0 ; i < NUM_OF_THREAD; i++)
   {
      
      if (i == 0 ){
         if (pthread_create(t1 + i, NULL , &Producer , NULL) !=0 )
         {printf("Error in crearting thread = %d" , i); return 1;}
      }
      else{
         if (pthread_create(t1 + i, NULL , &Consumer , NULL) !=0 )
         {printf("Error in crearting thread = %d" , i); return 1;}
      }
   }
   for ( i =0 ;i < NUM_OF_THREAD; i++)
   {
      if (i == 0)
      {
      if (pthread_join(t1[0], NULL ) != 0)
      {printf("Error in Joinning thread"); return 3;}
      }
      else
      {
         if (pthread_join(t1[1], NULL ) != 0)
         {printf("Error in Joinning thread"); return 3;}
      }

   }

   pthread_mutex_destroy(&MyMutex);
   sem_destroy(&SemEmpty);
   sem_destroy(&SemFull);

 return 0;
}
