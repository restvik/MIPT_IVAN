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

#define FILENAME "out.txt"
#define DATE_SIZE 26
#define PERM_SIZE 10
#define TYPE_SIZE 15

int start_id =1; // indicator wich help programm 2 know when it must start work
size_t f_size=0;
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
void my_handler();
void* open_file();
int main(){
        //blocked programm 2
        while(start_id) (void)signal(SIGUSR1,my_handler);
        //Take a size of file and nomber of files
        int fd, i = 0, f_num;
        void *for_map,* h_for_map;

        if ((fd = open(FILENAME, O_RDWR | O_CREAT , 0666)) < 0)
                 my_error("can not open result file\n");
        if ((for_map = mmap(NULL, sizeof(size_t)+sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED)
                 my_error("ERROR in map\n");
        h_for_map = for_map;
        f_size = *((size_t*)h_for_map);
        (h_for_map) += sizeof(size_t);
        f_num = *((int*)h_for_map);
        h_for_map += sizeof(int);
        if (munmap ((void*)for_map, sizeof(size_t) + sizeof(int)) < 0 ) my_error ("error with munmap \n");
        //Take all data about files
        if ((for_map = (struct file_data_struct* )mmap(NULL, f_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0)) == MAP_FAILED)
                 my_error("mapping failed \n");
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

void my_handler(){
        start_id--;
        return;
}
      
