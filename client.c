#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>

//Functions Declarations
void myfunc (void* ptr);
void getMsg();
void printMsg();

//Semaphore mutex lock
sem_t mutex;

//Shared memory pointers
char *shm, *s;

//Array for messages between Client-Server
char a[100];

int main()
{
    //For Shared memory
    int shmid,i,size=30;
    key_t key;
    
    //For thread passing
    char *wel = "\n ------- Chat-Room Server<->Client --------\n";
    
    //Key for data segment created by server, must be same as that of server
    key = 1000; 
    
    // For Thread
    int t;
    
    //Forking
    int b = fork();
    int processID = getpid();
    int parentID = getppid();
    
    //Thread variable
    pthread_t thread2;
    
    //Initializing semaphore indicated by sem
    sem_init(&mutex,0,1);
    
    
    //Locate data segment using key and get its id
    if ((shmid = shmget(key, size, IPC_CREAT | 0666)) < 0) {	
        perror("shmget"); 
        exit(1);
    }
    //Attach pointer with memory block
    if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    
    	printf("\n");
    	
    	//Calling thread
    	t=pthread_create(&thread2, NULL, (void*) &myfunc , (void*) wel);
    	//Joining thread
   	pthread_join(thread2, NULL);	
   	
   	//Printing Child and Parent Processes IDs
    	printf("\nProcess ID = %d\n",processID);  
    	printf("Parent ID = %d\n\n",parentID);
	
    if(b==0) //Child Process
    {
    
    
	*shm = '!';
	while(*shm!='*')
	{
		while(*shm!='@'&&*shm!='*')
			sleep(1); 
		if(*shm!='*'){
		
			printMsg();

			getMsg();
			
			if(a[0]!='*'){
				s=shm+1;
				for (i=0;i<strlen(a);i++)
					 *s++ = a[i];
				*s = '\0';
				*shm = '#';
				      }
			else
				*shm = '*';
			     }
		else
			printf("Connection Closed by Server");
	}
    }
    
	else if(b>0) //Parent Process
	{
	   //Detach shared memory
	   shmid = shmdt(shm);
	   
	   //Wait for child process to finish
	   waitpid(b,0,0);
	}
	
	else
	{
		printf("Fork Error");
	}
    	
    	//Destroying semaphore indicated by sem
	sem_destroy(&mutex);
	
    exit(0);
}

void myfunc (void *ptr) //Thread Function
{
			char *a = (char*) ptr;
			printf("%s", a);
			pthread_exit(0);
}

void getMsg()
{			
			//Semaphore Locking
			sem_wait(&mutex);
			
			printf("Enter Message : ");
			fgets(a, 100, stdin);
			
			//Filing
			FILE * fp;
			fp = fopen ("write.txt", "a");
			fprintf(fp, "%s", "Client : ");
   			fprintf(fp, "%s", a);
   			fprintf(fp, "%s", "\n");
   			fclose(fp);
   			//Filing Close
   		
   			//Semaphore Unlocking
			sem_post(&mutex);
}

void printMsg()
{
			//Semaphore Locking
			sem_wait(&mutex);
			
			printf("Server : ");
			for (s = shm+1; *s != '\0'; s++)
		    	putchar(*s);
			putchar('\n');
			
			//Semaphore Unlocking
			sem_post(&mutex);
}


