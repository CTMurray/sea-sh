// Camaal Murray & Rabeeb Anwar
// CS 452 
// 1/26/2017 || Lab week 2
// Program: sea-sh Implementation 
/*****************************************************************************************
* Program goals are:
* display a promt to the user 
* spawn a child process to execute commands with execvp or execlp
* output the "user CPU time used" for each individual child process spawned by the shell
* output the number of "involuntary context switches" experienced by each individual child 
  process spawned by the shell
* repeat until the user advises to "quit"
******************************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>//sigaction
#include <sys/types.h>//stat
#include <sys/wait.h>
#include <dirent.h>//Dir
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>


#define MAX_LEN 1024
#define MAX_TOKENS 32 

//char mycwd[512];
//char home[512];

void split(char *str, char **tokens);
//char *tokens[MAX_TOKENS + 1];

int main()
{
	//line stored
    	char cmd[MAX_LEN + 1];
	//line parsed
	char *tokens[MAX_TOKENS + 1];
	
	//resourse usage
	struct rusage usage;

	int status;
	
    //custom cmd prompt
    char *cprompt = getenv("USER");
	//if "USER" is not configured/set us "my"
    if(cprompt == NULL)
        cprompt = "my";

    while(1) {
        
		//display prompt
        printf("[%s@sea-sh]$ ", cprompt);
		//read command
        if(fgets(cmd, sizeof(cmd), stdin) == NULL) break;

        //remove newline and null terminator
        if(cmd[strlen(cmd) - 1] == '\n')
            cmd[strlen(cmd) - 1] = '\0';
		//parse command
		split(cmd, tokens);
		//exits shell
		if(strcmp(tokens[0], "quit") == 0)
			exit(0);	
	
    	//Fork process	
    	pid_t pid = fork();
    	int status;
   
    	// Error forking
    	if (pid < 0) {
        	char *error = strerror(errno);//print error if fork fails
        	printf("fork: %s\n", error);
        	return 1;
    	}

		//Parent process
    	else if(pid > 0) {
		//wait for the child to finish
        	waitpid(-1, &status, 0);	
   	}    	
	//Child process
 	else if(execvp(*tokens, tokens)){
		
		//Error must have occurred
       	char *error = strerror(errno);
		printf("fork: %s\n", error);
		exit(1);
       	//fprintf(stderr, "execvp in child failed: \"%s\" not found.\n", *tokens);
    	}
		//display useer cpu time used and involunarty context
		getrusage(RUSAGE_CHILDREN, &usage);
		printf("user CPU time used: %f (microseconds)\n", (double)usage.ru_utime.tv_usec);
		printf("involuntary context switches: %ld\n",usage.ru_nivcsw);
    }   
    return 0;
}

//Split into tokens based on spaces
void split(char *cmd, char **tokens)
{
    int i;
    for( i = 0; i < MAX_TOKENS; i++) {
        tokens[i] = strsep(&cmd, " ");
        if(tokens[i] == NULL){	
	 break;
	}
    }
}


