#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include<string.h>

#define MaxItems 5 // Maximum items a producer can produce or a consumer can consume
#define BufferSize 5 // Size of the buffer


struct paket{
  unsigned int seq_no;
  unsigned int type;
  unsigned int payload_len;
  char payload[100];
};

sem_t empty;
sem_t full;
int in = 0;
int out = 0;
//int buffer[BufferSize];
pthread_mutex_t mutex;



struct paket pkt[BufferSize];

char *message="This message need to be send";
int len = 30/5;

void *producer(void *pno)
{   
    //int item;
    
    
    
    //printf("producer");
        //struct packet* pkt;
        
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        
        //write packet 
        pkt[in].seq_no = in;
        pkt[in].type=1;
        pkt[in].payload_len=7;
        char m[100];
       
        //printf("\n len: %d, message:%s\n",len,message+len);
        memcpy(pkt[in].payload,message,len);
        message=message+len;
        
        printf("Producer %d: **MESSAGE : %s **at %d\n", *((int *)pno),pkt[in].payload,in);
        in = (in+1)%BufferSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    
}

void *consumer(void *cno)
{   
        //printf("consumer****");
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        //int item = buffer[out];
        struct paket *p = &pkt[out];
        //p->seq_no=pkt[out].seq_no;
       //p->type=pkt[out].type;
        //p->payload_len=pkt[out].payload_len;
        //p->payload=pkt[out].payload;
        //memcpy(p->payload,pkt[out].payload,15);
        printf("Consumer %d: Remove Item %u from %d\n",*((int *)cno),p->seq_no, out);
        printf("**Message from packet : %s\n",p->payload);
        out = (out+1)%BufferSize;
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    
}

int main()
{   

    pthread_t pro[5],con[5];
    pthread_mutex_init(&mutex, NULL);
    
    sem_init(&empty,0,BufferSize);
    sem_init(&full,0,0);

    int a[5] = {1,2,3,4,5}; //Just used for numbering the producer and consumer

    for(int i = 0; i < 5; i++) {
        pthread_create(&pro[i], NULL, (void *)producer, (void *)&a[i]);
    }
    for(int i = 0; i < 5; i++) {
        pthread_create(&con[i], NULL, (void *)consumer, (void *)&a[i]);
    }

    for(int i = 0; i < 5; i++) {
        pthread_join(pro[i], NULL);
    }
    for(int i = 0; i < 5; i++) {
        pthread_join(con[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
    
}