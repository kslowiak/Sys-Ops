#include <stdio.h>

int main(int argc, char *argv[])
{
	int i;
        printf("liczba argumentow wywolania: %d\n",argc);
        for(i=0;i<argc;i++)
                printf("argument nr %d = %s\n",i,argv[i]);
return 0;
}
