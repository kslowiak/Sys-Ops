#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

int errno;

int main(int argc, char *argv[])
{
	int i;
	struct stat statbuff;
	char *ptr;
	ushort	type;

	for(i=1;i<argc;i++) {
		if(stat(argv[i],&statbuff)<0){
			switch(errno){
				case EACCES: fprintf(stderr,"Blad: brak praw dostepu do pliku\n");
				case ENOENT: fprintf(stderr,"Blad: plik o podanej nazwie nie istnieje\n");
			}
			return 1;
		}
		printf("%s: ",argv[i]);
		type=statbuff.st_mode & S_IFMT;
		switch(type) {
		case S_IFDIR:	ptr="katalog"; break;
		case S_IFCHR:	ptr="specjalny, znakowy"; break;
		case S_IFBLK:	ptr="specjalny, blokowy"; break;
		case S_IFREG:	ptr="zwykly"; break;
		case S_IFLNK:	ptr="dowiazanie symboliczne"; break;	
		case S_IFSOCK:	ptr="gniazdo"; break;			
		case S_IFIFO:	ptr="kolejka fifo"; break;
		default:	ptr="typ nie znany"; break;
		}
		printf("%s\n",ptr);
	}
	return 0;
}

