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
    
    //Key for data segment created by server
    key = 1000;
    
    // For Thread
    int t;
    
    //Forking
    int f = fork();
    int processID = getpid();
    int parentID = getppid();
    
    //Thread variable
    pthread_t thread1;
    
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
    	t=pthread_create(&thread1, NULL, (void*) &myfunc , (void*) wel);
    	//Joining thread
	pthread_join(thread1, NULL);
	
	//Printing Child and Parent Processes IDs
    	printf("\nProcess ID = %d\n",processID);  
    	printf("Parent ID = %d\n\n",parentID);
    	
    
    	if(f==0) //Child Process
    {
    	
	*shm = '!';
	while(*shm!='*')
	{		
		getMsg();
		
		if(a[0]!='*'){
			s = shm+1;
			for (i=0;i<strlen(a);i++)
				 *s++ = a[i];
			*s = '\0';
			*shm = '@';

			while(*shm!='#'&&*shm!='*')
				sleep(1);

			if(*shm!='*'){
				printMsg();
			}
			else
			printf("Connection Closed by client");
		}
		else
			*shm = '*';
	}
     }
     
     else if(f>0) //Parent process
        {
        //Detach shared memory
	shmid = shmdt(shm);
	
	//Wait for child process to finish
	waitpid(f,0,0);
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
		fprintf(fp, "%s", "Server : ");
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
		
		printf("Client : ");
		for (s = shm+1; *s != '\0'; s++)
		putchar(*s);
		putchar('\n');
		
		//Semaphore Unlocking
		sem_post(&mutex);
}






