#include <sys/types.h>
#include <pthread.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int a = 0;

pthread_mutex_t lock;

struct range {
                    int from;
                    int to;
         };

void* th1(void* _arg)
{
	struct range out = *((struct range *)_arg); // pass arg to "out"
	int k;
	int sum = 0;
	for(k = out.from; k <= out.to; k++) sum += k; // math
	a += sum; 
	return NULL;
}

int main(int argc,char* argv[],char* envp[]) {	
	int res,k,num;

	num = atoi(argv[argc - 1]); // number of threads

	pthread_t * th = malloc(num*sizeof(pthread_t));// init of arrays and mutex
	struct range * in = malloc(num*sizeof(struct range)); 
	pthread_mutex_init(&lock,NULL);

	for(k = 0; k < num; k++) // record to " in" and creating threads
	{
		in[k].from = atoi(argv[2*k + 1]);
		in[k].to = atoi(argv[2*k + 2]) ;
        	res = pthread_create(&th[k],NULL,th1,&in[k]);
	};
	
	for(k = 0; k < num; k++) pthread_join(th[k],NULL); // wait
	
	printf("end result %d \n ",a);
	pthread_mutex_destroy(&lock);
	
	return 0;
}

