#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int descr;

void endp(){
	close(descr);
	printf("The writer process is terminated.\n");
	exit(0);
}

void b_p(){
	printf("The reader process has been terminated. The pipe is broken. I am dying as well.\n");
	close(descr);
	exit(0);
}

int main(){
	char tab[100];
	int size;

	signal(SIGTERM,endp);
	signal(SIGINT,endp);
	signal(SIGPIPE,b_p);

	descr=open("fifo",O_WRONLY);
	if(descr==-1){
		perror("error opening the file");
		exit(0);
	}
	printf("File fifo is open for writing\n");
	for(;;){
		printf("Type the text to be sent to the pipe fifo: ");
		scanf("%s",tab);
		size=write(descr,tab,strlen(tab));
		printf("%d written.\n",size);
	}
}
