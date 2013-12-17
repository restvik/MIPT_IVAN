#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

time_t start_time;//time when we start use our programm


void copy_file(const char* src_file,const char* dst_file);
void handle_dir(char* src_path, char* dst_path, char* entry);
void handle_file(char* src_path, char* dst_path, char* entry);
void gz(const char* s);

// formation of full path of new file system object
char* form_path(char* path, char* entry)
{
        int new_path_size = 2 + strlen(path) + strlen(entry);
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

void dir_walk(char* src_path,char* dst_path)
{
        // check of correct input information
        // strstr(const char *path_1,const char *path_2) -  finds the first occurrence path_2 in path_1
        if (strstr(dst_path, src_path)!=NULL)
                {
                printf("In function dir_walk: gone in cycles. Please verify the input <src> and <dst>.");
                exit(-1);
        }

        DIR* dir = opendir(src_path);
        if (dir == NULL)
        {
                printf("In function dir_walk: ERROR with opening %s\n", src_path);
                exit(-1);
        }

        // find out the information about our file system object and raises the appropriate function
        struct dirent *d;
        while ((d = readdir(dir)) != NULL)
        {
                if ((strcmp(d->d_name, ".") != 0) && (strcmp(d->d_name, "..") != 0))
                {
                        if (d->d_type & DT_REG)
                                 handle_file(src_path, dst_path, d->d_name);
                        if (d->d_type & DT_DIR)
                                 handle_dir(src_path, dst_path, d->d_name);
                }
        }
        if(closedir(dir)<0){
                printf("in FUNCTION DIR_WALK: ERROR with close\n");
                exit(-1);
        }
}
// Function for work with regular files
void handle_file(char* src_path, char* dst_path, char* entry)
{
        // AP: а что это вы все файлы подряд копируете? а проверка на модификацию?
        // Проверка на модификацию я поставил в копирование файла, т.к в любом случае нам придется в функции copy_file воспользоваться системным вызовом fstst()
        char* src_file = form_path(src_path, entry);
        char* dst_file = form_path(dst_path, entry);

        copy_file(src_file,dst_file);

        free(src_file);
        free(dst_file);
}
//Function for work with directoris
void handle_dir(char* src_path, char* dst_path, char* entry)
{
        int errno;
        char* src_dir = form_path(src_path, entry);
        char* dst_dir = form_path(dst_path, entry);

        if ((mkdir(dst_dir, 0777) == -1) && (errno != EEXIST))
        {
                printf("In function handle_dir: can't create a directory %s\n", dst_dir);
                printf("mkdir() failedi\n");
                exit(-1);
        }
        dir_walk(src_dir, dst_dir);
}
//Function for copy regulars file
void copy_file(const char* src_file,const char* dst_file)
{
        int ifd = open(src_file, O_RDONLY);
        if (ifd == -1)
        {
                printf("In function copy_file: can't open %s\n", src_file);
                exit(-1);
        }
        struct stat stat;
        if (fstat(ifd, &stat) == -1)
        {
                printf("In function copy_file: can't stat %s\n", src_file);
                exit(-1);
        }
        // Check the timr of last file modificatiom
        if (start_time < (stat.st_mtime) ){
                printf("file was modificat backup can't work more!\n");
                exit(-1);
        }

        int ofd = open(dst_file, O_CREAT | O_WRONLY | O_TRUNC, stat.st_mode);
        if (ofd == -1)
        {
                printf("In function copy_file: can't open %s\n", dst_file);
                exit(-1);
        }
        char buf[4096];
        ssize_t read_bytes;
        int flag=1;
        // AP: а где проверки на возможность записи меньшего количества байт?
        while(flag){
                if((read_bytes = read(ifd, buf, sizeof(buf))) < 0)
                        printf("In function copy_file: can't read %s\n", src_file);
                else
                        if(read_bytes != sizeof(buf))
                                flag = 0;
                if(write(ofd, buf, read_bytes) != read_bytes){
                        printf("In function copy_file: can't write %s\n", dst_file);
                        exit(-1);
                }
        }
        if(close(ofd)<0){
                printf("error in close(ofd)\n");
                exit(-1);
        }
        if(close(ifd)<0){
                printf("error in close(ifd)\n");
                exit(-1);
        }
        gz(dst_file);
}
//function for ompression of regular file
void  gz(const  char *s){
        int nid;

        nid=fork();

        if (nid==0){
                execl("/bin/gzip","/bin/gzip",s,NULL);
                printf("ERROR in execl\n");
                exit(-1);
        }
        if(nid<0){
                printf("Can't fork\n");
                exit(-1);
        }
}
int main(int argc, char *argv[])
{
        // I don't need on memory where I will store time so I wrote NULL in time()
        start_time=time(NULL);
        // Check the correct data input
        if (argc != 3)
        {
                printf("Incorrect INPUT!\n", argv[0]);
                printf("For correct use of programm you should write ./1 <src dir> <dest dir>\n");
                exit(-1);
        }
        //Take the realpath of src dir
        char *s1 = realpath(argv[1], NULL);
        if (s1 == NULL)
        {
                printf("realpath failed\n");
                free(s1);
                exit(-1);
        }
        //take the realpath of dst dir
        char *s2 = realpath(argv[2], NULL);
        if (s2 == NULL)
        {
                printf("realpath failed\n");
                free(s2);
                exit(-1);
        }
        //start scun dir
        dir_walk(s1,s2);
        free(s1);
        free(s2);
        return 0;
}

