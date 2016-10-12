#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
int main(int argc,char *argv[],char *envp[]) { // arguments of main
	pid_t p[10];
        FILE* fd;
        fd = fopen("/home/work/ff.txt","r"); //open file
        int i = 0; 
        char name[10][20];
	int time[10];
	while(!feof(fd)) //reading from file
                {
		fscanf(fd,"%d",&time[i]);
		fscanf(fd,"%s",name[i]);
                i++;
                };
        fclose(fd);// closing file
	i--; 
	printf("%d \n",i);
	int k,num; // num is number of each child
	for (k = 1;k < i;k++) p[k] = 0; 
	p[0] = fork();
	num = 0;
	for (k = 1;k < i;k++)
		 if (p[k-1] != 0) 
			{
			 p[k] = fork(); //birth of (k+1)-th child 
			num = k;
			};
	
	if (p[i-1] > 0)	// parent
	 {
		int sys;
		for (k = 0;k < i;k++) waitpid(p[k],&sys,0);
	}
	 else //children
	{
		sleep(time[num]);
		printf("number %d,my prog is %s \n",num + 1,name[num]);
		execle(name[num],name[num],0,envp);	
	} 
	return 0;
	}

