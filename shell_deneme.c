#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

void prompt(){
	char cwd[1024];
    	getcwd(cwd, sizeof(cwd));
   	char *username = getlogin();
   	printf("\033[0;33m");
	printf("%s/: %s > ",cwd,username);
	printf("\033[0m");
}

void user_input(char** input){
	char inp[81],*word;
	int count = -1;
	fgets(inp,81,stdin);
	char *ptrinp = strdup(inp);
	while((word = strsep(&ptrinp," ")) != NULL ){
       		count++;
       		word[strcspn(word, "\n")] = 0;
       		input[count] = word;
	}
}

void exec_command(char** input){
	
	int stat;
	pid_t pid = fork();
	
	if (pid == -1) {
        	printf("Forklama basarisiz.\n");
        	return;
    	} 
    	else if (pid == 0) {
        	if (execvp(input[0], input) < 0) {
        		printf("Hata: Komut icra edilemiyor.\n");
        	}
        	exit(EXIT_FAILURE);
    	} 
    	else {
        	waitpid(pid, &stat, WUNTRACED); 
        	return;
    	}
}


int main()
{	
	char *girdi[100];
	
	while(1){
		
		int count = 0;
		prompt();
		user_input(girdi);
		exec_command(girdi);
		while(girdi[count] != NULL){
			count++;
		}
		while(count + 1){
			girdi[count] = NULL;
			count--;
		}
		
	}
	
}