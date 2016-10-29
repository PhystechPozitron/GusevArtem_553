// client 

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char* argv[],char* envp[]) {
	
	int msqid; // IPC descriptor  
	key_t key = 600; // IPC_key 

	int i = 0, res; 

	// structure of client's message : numbers a and b; id - identifier of client (for server) 
	struct mymsgbuf {
		long mtype;
		int a,b;  // variables a and b
		int id; // 
	} mybuf;

	/* if ((key = ftok(pathname, 0)) < 0) {
		printf("Cant generate key ");
		exit(-1);
	} */

	// try to get an access to shared memory according to key
	if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
		printf("Cant get msqid \n");
		exit(-1);
	}
		// client's actions
		mybuf.a = atoi(argv[1]);
		mybuf.b = atoi(argv[2]);
		mybuf.mtype = 1;
		mybuf.id = atoi(argv[3]); //integer number > 0

		// sending a message
		if (msgsnd(msqid,(struct msgbuf *) &mybuf, 16, 0) < 0) {
			printf("Cant send message to queue \n");
			msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
			exit(-1);
		}
		printf("Numbers are sent to server. \n");
	//waiting for the answer	
		if  (msgrcv(msqid,(struct msgbuf *) &mybuf,16 ,mybuf.id + 1, 0) < 0)
			{
				printf("Cant receive message from queue \n");
				msgctl(msqid, IPC_RMID, (struct msqid_ds*)NULL);
				exit(-1);
			}
		if (mybuf.id > 0) {
			res = mybuf.a; // 
			printf(" Thank you! result is %d \n",res);
			} else printf("The server is busy... okey, I ll ask it later( \n");
		return 0;
} 
