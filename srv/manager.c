#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// function wich determines the number of digits
int main(int argc, char** argv){
        // n_cln - nomber of clients
        // i_size - how many symbols in nomber
        // n_str -  string where we  will store our nomber of clients in char
        int i, n_cln, i_size, nid;
        char *n_str=malloc(1), *n_str_help=malloc(1);

        if(argc != 3){
                printf("INPUT ERROR!\n EXAMPLE:\n ./manager.c  <name of file> <clients nomber>\n");
                exit(-1);
        }

        // Stsrt n clients
        n_cln = atoi(argv[2]);
        if( n_cln != 0){
                for(i=1; i <= n_cln; i++){
                        nid = fork();
                        if(nid < 0)
                        {
                                printf("ERROR in fork!\n");
                                exit(-1);
                        }
                        else
                                if(nid == 0){
                                        sprintf(n_str, "%i", i);
                                        execl("./c","./c", n_str, argv[2], NULL);
                                        printf("ERROR in execl!\n");
                                        exit(-1);
                                }
                }
        }
        else{
                printf("ERROR nobody can't sort matrix!\n");
                exit(-1);
        }
        // Start server
        nid=fork();
        if(nid < 0){
                printf("ERROR in fork!\n");
                exit(-1);
        }
        else
                if(nid == 0){
                        execl("./s","./s", argv[1], argv[2], NULL);
                        printf("ERROR in execl!\n");
                        exit(-1);
                }

        return 0;
}
