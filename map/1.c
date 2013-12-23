#include <sys/stat.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <pwd.h>
#include <grp.h>
#include <sys/mman.h>
#include <stdio.h>
#include <sys/msg.h>
#include <signal.h>
#include <errno.h>

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
size_t f_size=0;//f_size need for map
struct dirent **read_dir;
DIR *dir;

int take_num();
void my_error(char *mas);
DIR *open_dir();
char* form_path(char* path, char* entry);
void  get_info(struct file_data_struct **file_data, struct stat *buf, char* file_name, int i);
char* get_perm(mode_t st_mode);
void* open_file();
void send_signal();

int main(int argc, char **argv){
        struct file_data_struct **file_data;
        struct stat *buf;
        // f_num - number of file in directiry
        // fd - file descriptor
        int fd, f_num;
        void *for_map, *h_for_map;
        buf = (struct stat*)malloc(sizeof(struct stat));
        if( argc!=2)
                my_error("Incorrect work excl\n");
        // Open dir
        read_dir = (struct dirent**)malloc(sizeof(struct dirent*));
        if ((dir = opendir(argv[1])) == NULL)
                 my_error ("Can't open dir!\n");
        //Get a number of files
        f_num = take_num();
        file_data = (struct file_data_struct **) malloc (f_num * sizeof (struct file_data_struct*));
        //Get information
        char* path;
        int i=0;
        for (i; i < f_num; i++){
               if ((strcmp ((read_dir[i])->d_name, ".") != 0) && (strcmp((read_dir[i])->d_name, ".." ) != 0)){
                        path = form_path(argv[1], (read_dir[i]) -> d_name);
                        if (lstat(path, buf) < 0)
                                my_error("error in stat!\n");
                        get_info(file_data, buf, (read_dir[i])->d_name, i);
                }
                else i--;
        }
        //print all info in file
        for ( i=0; i < f_num; i++ )
                f_size += (file_data[i])->str_len;
        f_size += sizeof(size_t);
        f_size += sizeof(int);
        for_map = open_file();
        h_for_map = for_map;
        *((size_t* )h_for_map) = f_size;
        (h_for_map) += sizeof(size_t);
        * ((int * ) h_for_map ) = f_num;
        (h_for_map) += sizeof(int);
        for ( i=0 ; i < f_num ; i++) {
                * ((struct file_data_struct * ) h_for_map )= * (file_data[i]) ;
                strcpy ( ((struct file_data_struct * ) h_for_map ) -> nug, (file_data[i])-> nug);
                h_for_map += (file_data[i])->str_len ;

        }
        //close dir
        if ( closedir(dir) != 0)
                my_error ("ERROR in close dir!\n");
        if ( munmap (for_map, f_size + sizeof (int) ) < 0 )
                my_error ("Error in munap!\n");

        free(path);
        free(buf);
        // For correct work programm 2
        send_signal();

        return 0;
}
void send_signal(){
        pid_t ppid;
        ppid = getppid();
        kill (ppid, SIGUSR1);
        return;
}
void* open_file(){
        int fd;
        void* for_map;
        if( (fd = open (FILENAME, O_RDWR | O_CREAT | O_TRUNC , 0666 )) < 0)
                my_error ("ERROR can't open file out.txt!\n");
        if ( ftruncate ( fd , f_size ) < 0 )
                my_error ("ERROR can't change file size!\n");
        for_map = mmap ( NULL , f_size + 2 * sizeof(int) , PROT_READ | PROT_WRITE , MAP_SHARED , fd , 0 );
        if ( for_map == MAP_FAILED )
                my_error ("Mapping failed!\n");
        if ( close (fd) < 0 )
                my_error ("ERROR can't close file!\n");
        return for_map;
}

int take_num(){
        int f_num=0;
        while ((read_dir[f_num] = readdir(dir)) != NULL) {
                if ((strcmp ((read_dir[f_num]) -> d_name, ".") != 0) && (strcmp((read_dir[f_num]) -> d_name, ".." ) != 0)){
                        f_num ++;
                        read_dir = (struct dirent**) realloc (read_dir, (f_num + 1) * sizeof(struct dirent*));
                }
        }
        return f_num;
}
void my_error(char* mas)
{
        write(2, mas, strlen(mas) + 1);
        exit(-1);
        return;
}
char* form_path(char* path, char* entry)
{
        int new_path_size = 1 + strlen(path) + strlen(entry);
        char* new_path = malloc(new_path_size);
        if (new_path == NULL)
        {
                printf("In function form_path: malloc() failed:\nerror with malloc path for %s in %s", entry, path);
                return NULL;
        }
        strcat(strcpy(new_path, path), "/");
        strcat(new_path, entry);

        return new_path;
}
void get_info(struct file_data_struct **file_data, struct stat *buf, char *file_name, int i){
        struct passwd* user_data = getpwuid(buf -> st_uid);
        struct group* group_data = getgrgid(buf -> st_gid);
        char *user, *group, *perm, *str;
        size_t file_data_size;

        if (user_data == NULL)
                 my_error("error while getting information about user \n");
        if (group_data == NULL)
                 my_error("error while getting information about group \n");
        user = (char*)malloc(sizeof(char) * (strlen(user_data -> pw_name) + 11));
        strcpy(user, "USER NAME:");
        strcat(user, user_data -> pw_name);
        strcat(user, "\n");
        group = (char*)malloc(sizeof(char) * (strlen(group_data -> gr_name) + 12));
        strcpy(group, "GROUP NAME:");
        strcat (group, group_data -> gr_name);
        //Make a string where we will store names of file user and group
        str = (char*)malloc((strlen(group) + strlen(user) + strlen(file_name) + 12) * sizeof(char));
        strcpy(str, "FILE NAME:");
        strcat(str, file_name);
        strcat(str, "\n");
        strcat(str, user);
        strcat(str, group);

        file_data_size = (sizeof(struct file_data_struct) + strlen(str)* sizeof(char));
        file_data[i] = (struct file_data_struct *) malloc (file_data_size);
        (file_data[i]) -> str_len = file_data_size;


        strcpy( (file_data[i]) -> nug, str);
        strcpy( (file_data[i]) -> cr_date, ctime (&(buf -> st_ctime)) );
        strcpy( (file_data[i]) -> l_ch_date, ctime (&(buf -> st_mtime)) );
        switch ( (buf -> st_mode) & S_IFMT) {
                case S_IFBLK:  strcpy ( (file_data[i])->file_type , "block file\n" );     break;
                case S_IFCHR:  strcpy ( (file_data[i])->file_type , "character file\n" ); break;
                case S_IFDIR:  strcpy ( (file_data[i])->file_type , "dir\n" );        break;
                case S_IFIFO:  strcpy ( (file_data[i])->file_type , "FIFO\n" );        break;
                case S_IFLNK:  strcpy ( (file_data[i])->file_type , "link file\n" );          break;
                case S_IFREG:  strcpy ( (file_data[i])->file_type , "reg file\n" );     break;
                case S_IFSOCK: strcpy ( (file_data[i])->file_type , "socket\n" );           break;
                default:       strcpy ( (file_data[i])->file_type , "unknown\n" );          break;
        }
        (file_data[i])->file_size = buf -> st_size;
        perm = get_perm(buf -> st_mode);
        strcpy( (file_data[i])->perm, perm);
        free (user);
        free (group);
        free (perm);
        return;
}

char* get_perm(mode_t st_mode)
{
        char* perm;
        perm = (char*)malloc(PERM_SIZE * sizeof(char));
        mode_t owner, group, other;
        int i = 0;
        owner = st_mode & S_IRWXU;
        group = st_mode & S_IRWXG;
        other = st_mode & S_IRWXO;
        perm = (char*) malloc (PERM_SIZE * sizeof(char));
        for (i=0;i < PERM_SIZE; i++) perm[i] = '-';
                perm [PERM_SIZE - 1] = '\0';
        if ((owner & S_IRUSR)>0) perm [0] = 'r';
        if ((owner & S_IWUSR)>0) perm[1] = 'w';
        if ((owner & S_IXUSR)>0) perm[2] = 'x';
        if (group & S_IRGRP) perm[3] = 'r';
        if (group & S_IWGRP) perm[4] = 'w';
        if (group & S_IXGRP) perm[5] = 'x';
        if (other & S_IROTH) perm[6] = 'r';
        if (other & S_IWOTH) perm[7] = 'w';
        if (other & S_IXOTH) perm[8] = 'x';

        return perm;
}
