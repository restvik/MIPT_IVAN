#include <stdio.h>
#include <pwd.h>
#include <grp.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/msg.h>
#include <signal.h>
#include <wait.h>

#define KEYFILE "manager.c"
#define FILENAME "out.txt"
#define DATE_SIZE 26
#define PERM_SIZE 10
#define TYPE_SIZE 15

struct file_data_struct {
        size_t str_len;//length of one file data
        char file_type[TYPE_SIZE];//size of type
        char perm[PERM_SIZE];//permission
        char cr_date[DATE_SIZE];//date of creation
        char l_ch_date[DATE_SIZE];//last change date o file
        off_t file_size;//size of file
        char nug[];//name user group
};

void my_error(char *mas);
void one(int signo);
void zero(int signo);
void get_size();
void* open_file();

int main(){
        //waitpid(0, NULL,0);
        // wait работал, я пытался организовать корректный обмен сообщениями, что бы передать размер файла таким образом
        // но к сажелению у меня за нескольок часов не получилось это сделать((  Я планировал двумя сигналами sigusr1 sigusr2
        // передать побитово число. Дело в том что мне на этой неделе не только задачи по праграммированию сдвать надо. 
        // Это я к тому, что если вы от меня хотели увидеть работу с сигналами. 
        // Я также скланяюсь к тому что сдесь лучше было бы организовать связь между процессами запущенными в разных 
        // в разных программах через очередь сооющений тк эти программы  не обязательно могут быть запущенны через менеджер
        int fd, i = 0, f_num;
        void *for_map,* h_for_map;
        size_t f_size = 0;

        if ((fd = open(FILENAME, O_RDWR | O_CREAT , 0666)) < 0)
                 my_error("can not open result file\n");
        // fila alrady maped in memory
        if ((for_map = mmap(NULL, sizeof(size_t), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED)
                 my_error("ERROR in map\n");
        h_for_map = for_map;
        f_size = *((size_t*)h_for_map);
        printf("%i\n",f_size);
        (h_for_map) += sizeof(size_t);
        f_num = *((int*)h_for_map);
        h_for_map += sizeof(int);
        if (close(fd) < 0)
                my_error("cannot close file \n");

        for ( i=0; i < f_num; i++) {
                printf ("%s\n",((struct file_data_struct *)h_for_map)->nug);
                printf ("Create date  %s",((struct file_data_struct *)h_for_map)->cr_date);
                printf ("Last mod date  %s",((struct file_data_struct *)h_for_map)->l_ch_date);
                printf ("Permissions  %s\n",((struct file_data_struct *)h_for_map)->perm);
                printf ("File type  %s",((struct file_data_struct *)h_for_map)->file_type );
                printf ("File size  %i",(int)((struct file_data_struct *)h_for_map)->file_size );
                h_for_map += ((struct file_data_struct *)h_for_map)->str_len ;
                printf("\n\n");
        }

        if ( munmap ( for_map , f_size  ) < 0 )
                my_error ("ERROR in munmap!\n");

        return 0;
}
void my_error(char* mas){
            write(2, mas, strlen(mas) + 1);
            exit(-1);
}
