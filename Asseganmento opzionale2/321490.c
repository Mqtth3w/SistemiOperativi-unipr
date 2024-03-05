// Matteo Gianvenuti 321490 

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


int main(int argc, char *argv[]){

	if( (argc < 3) || (argc > 5) ){
		printf("il numero deglia argomenti deve essre >= 2 e <= 4, devono essere nomi di comandi UNIX privi di argomenti\n");
		exit(1);
	}

	char read[5] =";read";

	int i, pid, status, comandi[argc];
	for(i = 1; i < argc; i++){
		
		if( (pid = fork()) < 0){
			printf("errore nella fork\n");
			exit(2);
		}
		else if(pid == 0){
		
			printf("PID=%d esegue comando %s\n", getpid(), argv[i]);
			strcat(argv[i], read);
			int result = execl("/usr/bin/xterm", "xterm", "-e", argv[i], NULL ); //char *args[] = {"/usr/bin/xterm", "xterm", "-hold", "-e", argv[i], NULL};
			perror("execl"); printf("result=%d\n", result); exit(0);
		}
		else{
			comandi[i]=pid; //salva nella pos del comando i chi l'ha eseguito
		}
	}
	
	while( (pid=waitpid(-1, &status, 0)) > 0){
		
		if(WIFEXITED(status)){
			int j = 1;
			while(comandi[j] != pid){
				j++;
			}
			if(j <= (argc-1) ){ // previene il caso in cui il pid trovato non è di uno dei figli creati con fork
				printf("PID=%d terminato normalmente con status=%d\n", pid, WEXITSTATUS(status));

				if( (pid = fork()) < 0){
					printf("errore nella fork\n");
					exit(3);
				}
				else if(pid == 0){
					
					printf("figlio PID=%d riesegue comando %s\n", getpid(), argv[j]);
					strcat(argv[j], read);
					int result2 = execl("/usr/bin/xterm", "xterm", "-e", argv[j], NULL );
					perror("execl"); printf("result2=%d\n", result2); exit(4);
				}
				else{
					comandi[j] = pid;		// se si comennta questa riga i comandi vengono rieseguiti una sola volta		
				}
			}
		}
		else if(WIFSIGNALED(status)){
			printf("PID=%d e' stato terminato da un segnale con status=%d\n", pid, WEXITSTATUS(status));
		}
		else{
			printf("PID=%d e' stato terminato in modo anomalo con status=%d\n", pid, WEXITSTATUS(status));
			perror("terminazione anomala");
		}
	}
	exit(0);
}
