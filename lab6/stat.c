#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc,char *argv[])
{
struct stat filstat;

if(argc!=2){
	fprintf(stderr,"Nieprawidlowa liczba argumentow. Poprawna skladnia: %s nazwa_pliku\n",argv[0]);
	return 2;
}

if(stat(argv[1], &filstat) != -1)
{
	printf("Plik: %s\n",argv[1]);
	printf("typ i prawa dostepu:\t%o\nnumer wezla i-node:\t%d\nliczba dowiazan:\t%d\n",filstat.st_mode,filstat.st_ino,filstat.st_nlink);
	printf("ID uzytkownika:\t%d\nID grupy:\t%d\nRozmiar pliku:\t%d\n",filstat.st_uid,filstat.st_gid,filstat.st_size);
	printf("Czas ostatniego dostepu do pliku:\t%s",ctime(&filstat.st_atime));
	printf("Czas ostatniej modyfikacji pliku:\t%s",ctime(&filstat.st_mtime));
	printf("Czas ostatniej zmiany stanu pliku:\t%s",ctime(&filstat.st_ctime));
	return 0;
}
else
	perror(argv[0]);
	return 1;
}
