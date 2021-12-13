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

void exit_shell(){
	exit(0);
}

void change_dir(char* target){
	if (chdir(target) != 0){
		perror("Hata");
	}
}

int classify(char* command){
	if (!(strcmp(command, "cd"))) return 1;
	else if (!(strcmp(command, "exit"))) return 2;
	else if (!(strcmp(command, "showpid"))) return 3;
	else return 0;
}

void memset_input(char** input){
	int count = 0;
	while(input[count] != NULL){
		count++;
	}
	while(count + 1){
		input[count] = NULL;
		count--;
	}
}

int main()
{	
	char *girdi[100];
	int ps_switch;
	
	while(1){
		prompt();
		user_input(girdi);
		ps_switch = classify(girdi[0]);
		
		switch (ps_switch) {
		case 1:
			change_dir(girdi[1]);
			break;
		case 2:
			printf("Exiting...");
			exit_shell();
			break;
		default:
			exec_command(girdi);
		}
		
		//exit_shell();
		//exec_command(girdi);
		//change_dir(girdi[1]);
		memset_input(girdi);
	}
	
}