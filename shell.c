#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

char *input[10];
int ps_switch;
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
	fgets(inp,81,stdin);//Kullanicidan girdi alma
	char *ptrinp = strdup(inp);
	//Alinan girdiyi ayiklama
	while((word = strsep(&ptrinp," ")) != NULL ){
       		count++;
       		word[strcspn(word, "\n")] = 0;
       		input[count] = word;
	}
}

void exec_command(char** input){
	
	int stat;
	//Yeni bir proses olusturma
	int pid = fork();
	
	if (pid == -1) {
		printf("\033[0;31m"); //Renk belirleme
        	printf("Forklama basarisiz.\n");//Proses olusturma basarisiz olunca hata verir
        	printf("\033[0m"); //Rengi default hale getirme
		//Proses olusturma durdurulur
        	return;
    } 
    else if (pid == 0) {
	//Olusturulan prosese yeni bir kod yukleme
        if (execvp(input[0], input) < 0) {
        	printf("\033[0;31m"); //Renk belirleme
        	printf("Hata: Komut icra edilemiyor.\n");//Eksik veya hatali komut yazildiginda hata verir
        	printf("\033[0m"); //Rengi default hale getirme
        }
    	exit(EXIT_FAILURE);
    } 
    else {
    	//pid tutma
    	pids[count] = pid;
    	count++;
	//Ebeveyn prosesin yavru prosesi beklemesini saglayan komut
    	waitpid(pid, &stat, WUNTRACED);
	//Prosesler tamamlandiktan sonra fonksiyondan cikilir
        return;
    }
}

void exit_shell(){
	//Kabuk programindan cikma komutu
	exit(0);
}

void change_dir(char* target){
	//Built-in cd komutu
	if (chdir(target) != 0){
		perror("Hata");//Yanlis bir dosya yolu girildiginde hata verir
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
	//input[] listesindeki elemanlari NULL yapma komutu
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
	//Baslangicta kullaniciya mesaj verilir
	entry_msg();
	
	while(1){
		prompt();
		user_input(input);
		ps_switch = classify(input[0]);
		
		//Built-in komutu kontrolu icin switch case kullanildi
		switch (ps_switch) {
		case 1:		//Built-in cd
			change_dir(input[1]);
			break;
		case 2:		//Built-in exit
			printf("\033[0;33m"); //Renk belirleme
			printf("Exiting...\n");
			printf("\033[0m"); //Rengi default hale getirme
			exit_shell();
			break;
		case 3:		//Built-in showpid
			show_pid();
			break;
		default:	//Komut calistirma
			exec_command(input);
		}
		memset_input(input);
	}
}
