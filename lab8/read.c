#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>

int descr;

void endp(){
	close(descr);
	printf("The reader process ended.\n");
	exit(0);
}

int main(){
	char tab[100];
	int size;

	signal(SIGTERM,endp);
	signal(SIGINT,endp);

	

	descr=open("fifo",O_RDONLY);
	if(descr==-1){
		perror("error while opening file");
		exit(0);
	}
	

	printf("File fifo is open for reading\n");
	for(;;){
		bzero(tab,sizeof(tab));
		size=read(descr,tab,100);
		if(size==0) {
			printf("Writer process has been terminated\nDo you want to end the reader process (y/n)?\n");
			switch(fgetc(stdin)){
			case 't': close(descr); exit(0);
			default: fflush(stdin); sleep(1);
			}
		}
		else
		printf("%d characters readed\nThe text readed is:\n%s\n",size,tab);
	}
}
