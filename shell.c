#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int pids[5];
int count=0;
 
void prompt(){
	char cwd[1024];
    	getcwd(cwd, sizeof(cwd)); //Dosya konumunu alma
   	char *username = getlogin(); //Kullanıcı bilgisini alma
   	printf("\033[0;33m"); //Renk belirleme
	printf("%s/: %s >> ",cwd,username); //Prompt yazdırma
	printf("\033[0m"); //Rengi default hale getirme
}

void entry_msg(){
	printf("\033[H\033[J"); //Shell'i temizleme
	printf("\033[0;33m"); //Renk belirleme
	printf("----50.Grup Isletim Sistemleri Proje Odevi----\n");
	printf("\033[0m"); //Rengi default hale getirme
}

void user_input(char** input){
	//Kullanıcı girişi alma ve kelimelere ayırma işlemi
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
	int pid = fork();
	
	if (pid == -1) {
		printf("\033[0;31m"); //Renk belirleme
        	printf("Forklama basarisiz.\n");
        	printf("\033[0m"); //Rengi default hale getirme
        	return;
    } 
    else if (pid == 0) {
        if (execvp(input[0], input) < 0) {
        	printf("\033[0;31m"); //Renk belirleme
        	printf("Hata: Komut icra edilemiyor.\n");
        	printf("\033[0m"); //Rengi default hale getirme
        }
    	exit(EXIT_FAILURE);
    } 
    else {
    	//pid tutma
    	pids[count] = pid;
    	count++;
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

void show_pid(){
	//showpid yazdırma
	for(int i=0;i<5;i++){
		printf("%d\n",pids[i]);
	}
}

int classify(char* command){
	//Girilen komut built-in komut mu kontrolu
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
	char *input[100];
	int ps_switch;
	entry_msg();
	
	while(1){
		prompt();
		user_input(input);
		ps_switch = classify(input[0]);
		
		switch (ps_switch) {
		case 1:
			change_dir(input[1]);
			break;
		case 2:
			printf("\033[0;33m"); //Renk belirleme
			printf("Exiting...\n");
			printf("\033[0m"); //Rengi default hale getirme
			exit_shell();
			break;
		case 3:
			show_pid();
			break;
		default:
			exec_command(input);
		}
		memset_input(input);
	}
}
