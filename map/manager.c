#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void call_1(char *dir);
void call_2();
void my_error(char *mas);

int main(int argc, char** argv){

        if (argc != 2){
                printf("ERROR!\n Incorrect INPUT data\n It mast look ./m <dir>\n");
                exit(-1);
        }

        char *r_path = realpath(argv[1], NULL);
        if (r_path == NULL)
        {
                printf("realpath failed\n");
                free(r_path);
                exit(-1);
        }

        int nid;
        nid = fork();
        if (nid < 0)
                my_error("Can't fork!\n");
        else
                if( nid  == 0)
                        call_1(r_path);
                else
                        call_2();
        return 0;
}
void call_1(char *dir){
                execl("./1", "./1", dir, NULL);
                my_error("ERROR in call 1.c\n");
}

void call_2(){
        execl("./2", "./2", NULL);
        my_error("ERROR in call 2.c\n");
}

void my_error(char *mas){
            write(2, mas, strlen(mas)+1);
            exit(-1);
}
