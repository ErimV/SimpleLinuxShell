#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

char inp[81],*word[16],*ptrinp;
int i=0;
pid_t pid;

void prompt(){
	char cwd[1024];
    getcwd(cwd, sizeof(cwd));
   	char *username=getlogin();
   	printf("\033[0;33m");
	printf("%s/: %s ==> ",cwd,username);
	printf("\033[0m");
}

void user_input(){
	fgets(inp,81,stdin);
	if ((strlen(inp)>0)&&(inp[strlen(inp)-1]=='\n')) inp[strlen(inp)-1]='\0';
	ptrinp=strtok(inp," ");
	while(ptrinp!=NULL){
		word[i]=ptrinp;
		i++;
    	ptrinp=strtok(NULL," ");
	}
}

int main()
{
	while(1){
		prompt();
		user_input();
		if(strcmp("exit",inp)==0){
			printf("exit\n");
			break;
		}
		pid = fork();
		if(-1 == pid){
			printf("Failed to create a child!\n");
		}
		else if(0 == pid){
			//printf("child\n");
			execvp(word[0],word);
		}
		else{
			//printf("parent\n");
			if(NULL == word[i]) waitpid(pid, NULL, 0);
		}
	}
}
