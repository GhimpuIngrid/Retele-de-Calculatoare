#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME "MyTest_FIFO"

    char s[300], aux[300], rez[400];
    int num, fd, fd1, fd2, fdcs, fdsc, v[300], k=0, i, j;
    pid_t pid;

int main() //int argc, char const *argv[]
{


    printf("%s\n", "Ma conectez la server... \n");
    //fd = open(FIFO_NAME, O_WRONLY | O_NDELAY);
    fd = open(FIFO_NAME, O_WRONLY);

    pid=getpid();
    //printf("%d\n", (int)pid);

    while(pid)
    {
        v[k] = pid % 10;
        k++;
        pid /= 10;
    }

    for(i=0, j=k-1; j>=0; i++, j--)
        {
            s[i]=v[j] + '0';
        }

    s[i]='\n';

    write(fd, s, strlen(s));


    sleep(2);

    printf("PID: %s\n", s);

    //snprintf(aux, 300, "%s%s", "FIFO", s);

    strcpy(aux, "FIFO");

    strcpy(aux+4, s);

    aux[i+4]='\0';

    //printf("%s: nume FIFO de scriere\n", aux1);

    //snprintf(aux2, 300, "%s%s", "FIFO2", s);

    //aux2[i+5] = '\0';

    printf("%s: nume FIFO\n", aux);
    
    char c;

    close(fd);
    fd1=open(aux, O_WRONLY);
    if(fd1 == -1)
        perror("eroare fd1\n");
    fd2=open(aux, O_RDONLY);
    if(fd2 == -1)
        perror("eroare fd2\n");

    snprintf(rez, 400, "%s%s", aux, "sc");
    fdsc = open(rez, O_RDONLY);

    snprintf(rez, 400, "%s%s", aux, "cs");
    fdcs = open(rez, O_WRONLY);

    printf("%s\n", "Am FIFO personal");
    printf("%s\n", aux);


    read(fdsc, s, 300);
    printf("%s\n", s);
    printf("%s\n", "Puteti introduce comenzi: \n");

    while(1){

    		gets(s);
    		num = write(fd1, s, strlen(s));

            if(strcmp(s, "quit") == 0)
                return 0;

            for(int i = 0; i < 300; i++)
                s[i] = '\0';

    		printf("Ati introdus o comanda de %d litere\n", num);
            //printf("%d %d", fdsc, fdcs);

            if(read(fdsc, s, 300) != -1)
            {
                //if(k == 1)
                    printf("%s\n", s);
                write(fdcs, " ", 1);
            }

            else perror("EROARE!");

            s[0] = '\0';
    }



}