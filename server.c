/*
    Server process
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h> // exit
#include <unistd.h> // sleep
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/* structure of client's message : numbers a and b; id - identifier of client (for server) */
struct mymsgbuf {
   		long mtype;
 		int a,b; 
		int id;
    	} data;// global array of client' s data

int num = 0; // number of working threads

pthread_t thread_id[2]; // array of identifiers of threads working with client's data
int ind = 0; // index of thread_id
pthread_mutex_t lock; 

key_t key = 600; // IPC_key


void* my_thread(void* arg) {
	//pthread_mutex_lock(&lock);
	
	struct mymsgbuf out = *(struct mymsgbuf*)arg;
	int k = out.mtype;

	//pthread_mutex_unlock(&lock);
	/* actions of thread */
	sleep(out.a + out.b);
	out.a += out.b;

	out.mtype = out.id + 1; // changing direction: information for sender, that data with index k are ready to send	
	int msqid; // IPC descriptor   
  
	/* try to get an access to shared memory according to key */
	if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
			printf("Cant get msqid \n");
			exit(-1);
		}
	msgsnd(msqid,(struct msgbuf *) &out,16,0); // sending data to client	
	
  	printf("Result = %d is sent to client %d. \n",out.a,out.id);

    	thread_id[k] = 0;
	num--; //current thread ends	
    	return NULL;
}

//main function

int main() {
 
	/* creating message queue according to key */
	
	int msqid; // IPC descriptor   
  
	/* try to get an access to shared memory according to key */
	         if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
			printf("Cant get msqid \n");
			exit(-1);
       		}
        int i; 
	thread_id[0] = 0;
	thread_id[1] = 0;	
	pthread_mutex_init(&lock, NULL);

	/* receiving messages from clients */
	while(1) {
			if (msgrcv(msqid,(struct msgbuf*) &data,16,1,0) < 0) // receive with mtype = max + 1
				{
					printf("cant receive message from queue \n");
					msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL); 
					exit(-1);
				}
			if (num < 2) {
				/* searching for free index */
				if (thread_id[0] == 0) ind = 0; else ind = 1;

				/* creating thread */
				data.mtype = ind; // save index to mtype
      				i = pthread_create(&thread_id[ind] ,(pthread_attr_t *)NULL ,my_thread ,&data);//this thread will work with index ind 
      				if (i) {
          				printf("Can`t create thread, returned value = %d \n" ,i);
          				exit(-1);
      				}
  		
    				printf("Client %d, your message is received!Please wait for the answer \n",data.id);
				num++; // number of working threads increments
			} else // sending error to client (id = 0)
				{
					data.mtype = data.id + 1;
					printf("Sorry,client %d,but server is busy \n",data.id);
					data.id = 0;
					msgsnd(msqid,(struct msgbuf *) &data,16,0);
				}
		}
	pthread_mutex_destroy(&lock); //destroy lock

    	return 0;
}






