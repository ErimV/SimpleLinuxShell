#include <stdio.h>
#include <unistd.h>
#include <string.h>

void prompt(){
	char cwd[1024];
    getcwd(cwd, sizeof(cwd));
   	char *username=getlogin();
   	printf("\033[0;33m");
	printf("%s/: %s ==> ",cwd,username);
	printf("\033[0m");
}

void user_input(){
	char inp[81],*word,*prgname,*param;
	int count=0;
	fgets(inp,81,stdin);
	char *ptrinp=strdup(inp);
	while( (word = strsep(&ptrinp," ")) != NULL ){
       	count++;
       	if(count==1) prgname=word;
       	else param=word;
	}
}

int main()
{
	//while(1){
		prompt();
		user_input();
	//}
}
