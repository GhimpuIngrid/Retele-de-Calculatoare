#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/socket.h>
#include <time.h>

#define FIFO_NAME "MyTest_FIFO"
//#define _GNU_SOURCE

    struct client {int pid, fd1, fd2, fdsc, fdcs, status; 
                    char fifo[300], nume[300];
                    char time[300];}v[300];

    char s[1000], snrclienti[300], aux[300], rez[400], nume[1000], aux1[1000];
    int num, fd, kid, nrclienti, p, nrcl, k, i, d, fdn;
    int c1[2], c2[2], c3[2], c4[2], c5[2], c6[2];
    int kid1, kid2, kid3, kid4, kid5;
    char login[] = "login : ", getlog[] = "get-logged-users", getproc[] = "get-proc-info : ", logout[] = "logout", quit[] = "quit";
    time_t rawtime;
    struct tm *info;


int main() //int argc, char const *argv[]
{

    mknod(FIFO_NAME, S_IFIFO | 0666, 0); //fac fisierul de comunicare

    printf("%s\n", "Astept clienti... \n");
    //fd = open(FIFO_NAME, O_RDONLY | O_NDELAY);
    fd = open(FIFO_NAME, O_RDONLY);


    pipe2(c1, O_NDELAY);
    socketpair(AF_UNIX, SOCK_STREAM, 0, c2);
    socketpair(AF_UNIX, SOCK_STREAM, 0, c3);
    socketpair(AF_UNIX, SOCK_STREAM, 0, c4);
    socketpair(AF_UNIX, SOCK_STREAM, 0, c5);
    socketpair(AF_UNIX, SOCK_STREAM, 0, c6);
    
    /*pipe(c2); //copil login
    pipe(c3); //copil get logged users
    pipe(c4); //copil 
    pipe(c5); //copil
    pipe(c6); //copil logout*/

    //pipe(c1);

    kid=fork();

    if(kid == 0)
    {
        //close(c1[0]);

        
        nrclienti=0;
        close(c1[0]);
        close(c2[0]);
        close(c3[0]);
        close(c4[0]);
        close(c5[0]);
        close(c6[0]);
        close(c2[1]);
        close(c3[1]);
        close(c4[1]);
        close(c5[1]);
        close(c6[1]);

        while(1)
        {
            num=read(fd, s, 300);

            if(num != -1)
            {
                s[num]='\0';


                for(int i=0; s[i]; i++)
                {
                    p=4;
                    //snprintf(aux, 300, "%s%s", "FIFO1", s);

                    //printf("%s\n", aux);

                    strcpy(aux, "FIFO");

                    while(s[i] != '\n')
                    {
                        aux[p]=s[i];
                        p++;
                        i++;
                    }
                    aux[p]='\0';

                    mknod(aux, S_IFIFO | 0666, 0);

                    snprintf(rez, 400, "%s%s", aux, "sc");// de la server la client

                    mknod(rez, S_IFIFO | 0666, 0);

                    snprintf(rez, 400, "%s%s", aux, "cs"); //de la client la server

                    mknod(rez, S_IFIFO | 0666, 0);

                    //write(c1[1], aux, strlen(aux));

                    //snprintf(aux2, 300, "%s%s", "FIFO2", s);

                    //printf("%s\n", aux2);

                    //mknod(aux2, S_IFIFO | 0666, 0);

                    nrclienti++;

                    /*itoa(nrclienti, snrclienti, 10);

                    strcpy(rez, aux);
                    rez[p++]=' ';
            
                    strcpy(rez+p, snrclienti);
                    p=strlen(rez);

                    rez[p]=' ';

                    strcpy(rez+p, aux+4);*/
                    //printf("%s\n", rez);

                    snprintf(rez, 400, "%s %d %s", aux, nrclienti, aux+4);
                    
                    //open(c1[1]);
                    write(c1[1], rez, strlen(rez));
                }
            }
            else perror("eroare citire c1\n");
        }
    }

    else{

        kid1=fork();

        if(kid1 == 0)
        {
            close(c1[0]);
            close(c3[0]);
            close(c4[0]);
            close(c5[0]);
            close(c6[0]);
            close(c1[1]);
            close(c2[1]);
            close(c3[1]);
            close(c4[1]);
            close(c5[1]);
            close(c6[1]);

            while(1)
            {

                read(c2[0], s, 300);

                //strcpy(rez, s+8);

                p=0;
                int nrcl=0;
                int i;

                for(i=8; s[i]!=' '; i++) // sa nu uit sa precizez clientului ca nu are voie sa se logheze cu nume cu spatii
                {
                    rez[p] = s[i];
                    p++;
                }

                rez[p]='\0';
                ////////////////////////////////// e bine
                //write( c2[0], rez, strlen(rez));
                i++;

                for(; s[i] >= 48 && s[i] <= 57; i++)
                {
                    nrcl = nrcl*10+(s[i]-'0');
                }

                if((fdn=open("Nume.txt", O_RDONLY)) == -1)
                    perror("Eroare la deschiderea nume");

                read(fdn, nume, 1000);
                //write( c2[0], nume, strlen(nume));
                ///////////////////////////////////e bine

                char *t=strstr(nume, rez);
                int ok=0;

                while(t)
                {
                    if(t[strlen(rez)] == '\n')
                    {
                        ok=1;
                        break;
                    }
                    t = strstr(t + 1, rez);

                }
                //if(t) ok = 1;

                if(ok == 0)
                    write(c2[0], "Nu aveti aceasta permisiune\n", strlen("Nu aveti aceasta permisiune\n"));

                else {

                    /*v[nrcl].status = 1;
                    strcpy(v[nrcl].nume, rez);
                    time( &rawtime );
                    info = localtime( &rawtime );
                    v[nrcl].time = asctime(info);*/

                    write(c2[0], "V-ati logat cu succes\n", strlen("V-ati logat cu succes\n"));
                    
                    //snprintf(rez, 300, "%d\n", 1);
                    //write(c2[0], rez, strlen(rez));
                }
            }



        }
        
        else
        {
            /*kid2=fork();

            if(kid2 == 0)
            {
                close(c1[0]);
                close(c2[0]);
                close(c4[0]);
                close(c5[0]);
                close(c6[0]);
                close(c1[1]);
                close(c2[1]);
                close(c3[1]);
                close(c4[1]);
                close(c5[1]);
                close(c6[1]);

                read(c3[0], s, strlen(s));

                for(int i=1; i<=nrcl; i++)
                {
                    if(v[i].status == 1)
                    {

                    }
                }



                write(c3[0], "Am primit comanda getlog\n", strlen("Am primit comanda getlog\n"));

            }
           
           else*/
           {
                kid3=fork();

                if(kid3 == 0)
                {
                    close(c1[0]);
                    close(c2[0]);
                    close(c3[0]);
                    close(c5[0]);
                    close(c6[0]);
                    close(c1[1]);
                    close(c2[1]);
                    close(c3[1]);
                    close(c4[1]);
                    close(c5[1]);
                    close(c6[1]);


                    while(1)
                         {

                            read(c4[0], s, 300);

                            int i, pid=0;

                            for(i=0; s[i] >= '0' && s[i] <= '9'; i++)
                                pid=pid*10+(s[i]-'0');

                            sprintf(rez, "/proc/%d/status", pid);
                            int fdproc = open(rez, O_RDONLY);

                            read(fdproc, rez, 1000);
                            

                            //name Name:
                            //state State:
                            //ppid PPid:
                            //uid Uid:
                            //vmsize 23 VmSize:

                            char *t = 0;
                            char *p = 0;

                            //while(strstr(rez, "Name:"));

                            t = strstr(rez, "Name:");
                            p = strstr(t, "\n");
                            strncpy(nume, t, p - t + 1);

                            //write(c4[0], nume, 1000);

                            t = strstr(rez, "State:");
                            p = strstr(t, "\n");
                            strncat(nume, t, p - t + 1);

                            t = strstr(rez, "PPid:");
                            p = strstr(t, "\n");
                            strncat(nume, t, p - t + 1);

                            t = strstr(rez, "Uid:");
                            p = strstr(t, "\n");
                            strncat(nume, t, p - t + 1);

                            t = strstr(rez, "VmSize:");
                            p = strstr(t, "\n");
                            strncat(nume, t, p - t + 1);
                            write(c4[0], nume, strlen(nume));

                            for(i = 0; i < 1000; i++)
                                nume[i] = '\0';
                        }

                }
                
                else
                {
                    kid4=fork();

                    if(kid4 == 0)
                    {
                        close(c1[0]);
                        close(c2[0]);
                        close(c3[0]);
                        close(c4[0]);
                        close(c6[0]);
                        close(c1[1]);
                        close(c2[1]);
                        close(c3[1]);
                        close(c4[1]);
                        close(c5[1]);
                        close(c6[1]);

                        read(c5[0], s, 300);

                        for(int i=0; s[i] >= 48 && s[i] <= 57; i++)
                        {
                            nrcl = nrcl*10+(s[i]-'0');
                        }

                        v[nrcl].status = 0;

                        write(c5[0], "Ati fost delogat\n", strlen("Ati fost delogat\n"));

                    }

                    else
                    {
                        kid5=fork();

                        if(kid5 == 0)
                        {

                            close(c1[0]);
                            close(c2[0]);
                            close(c3[0]);
                            close(c4[0]);
                            close(c5[0]);
                            close(c1[1]);
                            close(c2[1]);
                            close(c3[1]);
                            close(c4[1]);
                            close(c5[1]);
                            close(c6[1]);

                            read(c6[0], s, strlen(s));

                            write(c6[0], "Am primit comanda quit\n", strlen("Am primit comanda quit\n"));

                        }

                        else
                        {
                            close(c1[1]);
                            //printf("%s\n", "Am ajuns in parinte");

                            close(c2[0]);
                            //close(c3[0]);
                            close(c4[0]);
                            close(c5[0]);
                            close(c6[0]);

                            while(1)
                            {

                                if(read(c1[0], s, 400) != -1)
                                {
                                    //printf("%s\n", s);

                                    nrcl++;

                                    for(i=0; s[i] != ' '; i++)
                                    {
                                        if(s[i] > '9')
                                            v[nrcl].fifo[i] = s[i];

                                        else if(s[i] >= '0' && s[i] <= '9')
                                        {
                                            v[nrcl].fifo[i] = s[i];
                                            v[nrcl].pid = v[nrcl].pid * 10 + (s[i] - '0');
                                        }
                                    }
                                
                                    //d=i+1;

                                    /*for(i=d; s[i] != ' '; i++)
                                    {
                                   
                                        v[nrcl].fifo2[i] = s[i];
                                    }*/

                                    s[0]='\0';

                                    //printf("%s %s\n", v[nrcl].fifo1, v[nrcl].fifo2);


                                    //printf("%s\n", "Am unde sa scriu");


                                    v[nrcl].fd1 = open(v[nrcl].fifo, O_RDONLY | O_NDELAY);
                                        if(v[nrcl].fd1 == -1)
                                            perror("eroare la deschiderea fd1\n");

                                
                                    v[nrcl].fd2 = open(v[nrcl].fifo, O_WRONLY | O_NDELAY);
                                        if(v[nrcl].fd2 == -1)
                                            perror("eroare la deschiderea fd2\n"); 

                                    snprintf(rez, 400, "%s%s", v[nrcl].fifo, "sc");
                                    v[nrcl].fdsc = open(rez, O_WRONLY);
                                    //printf("%s\n", rez);

                                    snprintf(rez, 400, "%s%s", v[nrcl].fifo, "cs");
                                    v[nrcl].fdcs = open(rez, O_RDONLY);
                                    //printf("%s\n", rez);


                                    printf("%s\n", v[nrcl].fifo);


                                    //v[nrcl].fd2 = open(v[nrcl].fifo2, O_WRONLY | O_NDELAY);

                                    int err = write(v[nrcl].fdsc, "V-ati conectat la server!\n", strlen("V-ati conectat la server!\n"));
                                        if(err == -1)
                                            perror("eroare la conectare");

                                    printf("%s\n", "Am deschis fisierele de comunicatie");

                                }

                                for(i=1; i <= nrcl; i++)
                               {
                                 
                                    //printf("%s\n", "verific comenzi de la clienti");
                                    for(int j=0; j<1000; j++)
                                        s[j]='\0';

                                    //printf("%s\n", "Am golit stringul");

                                    //num = read(v[i].fd1, s, 300);

                                    if(v[i].status >= 0 && read(v[i].fd1, s, 300) > 0)
                                    {
                                        printf("%s\n", " Citim comenzile clientilor: \n");

                                        //printf("%s %s\n", "aaa", v[i].fifo);

                                        printf("Clientul nr %d mi-a dat comanda %s\n", i, s);
                /////////////////////////////////////////////////////////////////////////////////////
                                        if(strcmp(s, getlog) == 0)
                                        {

                                            if(v[i].status == 0)
                                                strcpy(s, "Trebuie sa va logati pentru a avea aceasta permisiune");

                                            else
                                            {
                                                kid2=fork();

                                                if(kid2 == 0)
                                                {
                                                    close(c1[0]);
                                                    close(c2[0]);
                                                    close(c4[0]);
                                                    close(c5[0]);
                                                    close(c6[0]);
                                                    close(c1[1]);
                                                    close(c2[1]);
                                                    close(c3[1]);
                                                    close(c4[1]);
                                                    close(c5[1]);
                                                    close(c6[1]);

                                                    for(int i=1; i<=nrcl; i++)
                                                    {
                                                        if(v[i].status == 1)
                                                        {
                                                            gethostname(aux, 300);
                                                            snprintf(rez, 400, "%s %s %s\n", v[i].nume, aux, v[i].time);
                                                            strcat(aux1, rez);
                                                        }
                                                    }

                                                    write(c3[0], aux1, strlen(aux1));
                                                    return 0;
                                                }

                                                if(kid2){

                                                    //printf("sunt aici");

                                                    //while(1);
                                                    //close(c3[0]);
                                                    //printf("astept");
                                                    if(read(c3[1], s, 1000) == -1)
                                                        perror("eroare read c3\n");

                                                    //printf("am asteptat");
                                                    //printf("%s\n", s);
/*
                                                                printf("ala");
                                                                printf("%d %d", v[i].fdsc, v[i].fdcs);
                                                                
                                                                sleep(1);
                                                                if(write(v[i].fdsc, s, strlen(s)) == -1) //anunt cl ca am primit comanda
                                                                    printf("EROARE");

                                                                //sleep(1);
                                                                if(read(v[i].fdcs, s, 1) == -1) //nu trec mai departe pana nu primesc confirmare de la client
                                                                    printf("EROARE2");
                                                                printf("%s\n", "Merge ok");

                                                                continue;*/
                                                }
                                            }
                                            //snprintf(rez, 400, "%s %d\n", s, i);

                                            //printf("%s\n", "Orice");

                                            //write(c3[1], rez, strlen(rez));

                                            //read(c3[1] ,s, 300);
                                            //printf("%s\n", s);
                                        }

                                        else if(strncmp(s, "get-proc-info : ", 14) == 0)
                                        {
                                            if(v[i].status == 0)
                                                strcpy(s, "Trebuie sa va logati pentru a avea aceasta permisiune");

                                            else
                                            {
                                                p=0;
                                                for(int j=16; s[j] >= '0' && s[j] <= '9'; j++)
                                                {
                                                    rez[p]=s[j];
                                                    p++;
                                                }

                                                rez[p]='\0';

                                                write(c4[1], rez, strlen(rez));

                                                //printf("%s\n", rez);
                                                read(c4[1], s, 300);
                                                //printf("%s\n", s);
                                            }

                                            //snprintf(rez, 400, "%s %d\n", s, i);
                                            //write(c4[1], rez, strlen(rez));

                                            //read(c4[1] ,s, 300);
                                            //printf("%s\n", s);
                                        }

                                        else if(strcmp(s, logout) == 0)
                                        {
                                            if(v[i].status == 1)
                                            {
                                                //snprintf(rez, 400, "%d\n", i);
                                                //write(c5[1], rez, strlen(rez));

                                                //read(c5[1] ,s, 300);
                                                //printf("%s\n", s);

                                                v[i].status = 0;
                                                strcpy(s, "V-ati delogat cu succes");
                                            }

                                            else strcpy(s, "Nu sunteti logat");
                                        }

                                        else if(strcmp(s, quit) == 0)
                                        {
                                            //snprintf(rez, 400, "%s %d\n", s, i);
                                            //write(c6[1], rez, strlen(rez));

                                            //read(c6[1] ,s, 300);
                                            //printf("%s\n", s);
                                            v[i].status = -1;
                                            //continue;
                                        }

                                        else if(strncmp(s, login, 8) == 0)
                                        {
                                            
                                            if(v[i].status == 1)
                                                strcpy(s, "Sunteti deja logat\n");

                                            else{
                                                //printf("%s\n", "bines");
                                                snprintf(rez, 400, "%s %d\n", s, i);
                                                //printf("%s\n", rez);
                                                if(write(c2[1], rez, strlen(rez)) < 0)
                                                    perror("err");
                                                //printf("%s\n", "am scris");

                                                for(int k = 0; k < 300; k++)
                                                    s[k] = '\0';

                                                read(c2[1], s, 300);

                                                if(strcmp(s, "V-ati logat cu succes\n") == 0)
                                                {
                                                    v[i].status = 1;
                                                    p=0;
                                                    int k;
                                                    for(k=8; rez[k]!=' '; k++)
                                                    {
                                                        v[i].nume[p]=rez[k];
                                                        p++;
                                                    }
                                                    v[i].nume[p]='\0';

                                                    //strcpy(v[i].nume, rez);
                                                    time( &rawtime );
                                                    info = localtime( &rawtime );
                                                    strcpy(v[i].time, asctime(info));
                                                    //v[i].time = asctime(info);
                                                }
                                                //printf("%s\n", s);
                                                //printf("am iesit");
                                            }
                                            //printf("%d\n", v[i].status);
                                        }

                                        else //write(v[i].fdsc, "Comanda incorecta!\n", strlen("Comanda incorecta!\n"));
                                            strcpy(s, "Comanda incorecta!");
                                    /////////////////////////////////////////////////////////////////
                                        k=1;
                                        if(v[i].status >= 0)
                                        {
                                            //printf("Numar client : %d, numar total : %d", i, nrcl);
                                            //printf("%d %d", v[i].fdsc, v[i].fdcs);

                                            sleep(1);
                                            if(write(v[i].fdsc, s, strlen(s)) == -1) //anunt cl ca am primit comanda
                                                printf("EROARE");

                                            //sleep(1);
                                            if(read(v[i].fdcs, s, 1) == -1) //nu trec mai departe pana nu primesc confirmare de la client
                                                printf("EROARE2");
                                            //printf("%s\n", "Merge ok");
                                        }
                                    }
                                    //s[0]='\0';

                                } 
                            }
                        }
                    }
                }
           } 
        }    
    }


    /*while(1){

        printf("%s\n", "Inca nu avem niciun client \n");
        num = read(fd, s, 300);
        printf("%s\n", "Ne-a contactat primul client \n");
        s[num] = '\0';

        printf("%s\n", s);

    }*/


    return 0;
        
}