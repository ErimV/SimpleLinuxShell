#include <stdio.h>
#include <unistd.h>

void prompt(){
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    char *username=getlogin();
    printf("\033[0;33m");
    printf("%s/: %s ==>\n ",cwd,username);
    printf("\033[0m");
}



int main()
{
    //while(1){
	prompt();
    //}
}
