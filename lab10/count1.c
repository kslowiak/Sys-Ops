#include <stdio.h>
#include <unistd.h>

int main(){
int i;
int j;
for(i=1;i<200;i=i+2){

/*poczatek sekcji krytycznej */
        for(j=0;j<i;j++){
                printf("%d ",i);
                fflush(stdout);
        }
        sleep(1);
        printf("\n");
/* koniec sekcji krytycznej */
}
return 0;
}

