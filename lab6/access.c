#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
if(chmod(argv[1],S_IROTH | S_IWUSR)<0)
	printf("Error\n");
else
	printf("Ustawiono prawa dostepu %o do pliku %s\n",S_IROTH | S_IWUSR,argv[1]);
return 0;
}
