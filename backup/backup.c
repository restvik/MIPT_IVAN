#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <math.h>


void  copy_file(const char* src_file,const char* dst_file);
void handle_dir(char* src_path, char* dst_path, char* entry);
void handle_file(char* src_path, char* dst_path, char* entry);
void gz(const char* s);



char* form_path(char* path, char* entry)
{
        int new_path_size = 2 + strlen(path) + strlen(entry);
        char* new_path = malloc(new_path_size);
        if (new_path == NULL)
        {
                printf("In function form_path: malloc() failed:\nerror with malloc path for %s in %s", entry, path);
                return NULL;
        }
        if (sprintf(new_path, "%s/%s", path, entry) != new_path_size - 1)
        {
                printf("In function form_path: error with making path for %s in %s\n", entry, path);
                perror("sprintf() failed:");
                return NULL;
        }
        return new_path;
}


void dir_walk(char* src_path,char* dst_path)
{
        if (strstr(dst_path, src_path) !=NULL)
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


void handle_file(char* src_path, char* dst_path, char* entry)
{
        char* src_file = form_path(src_path, entry);
        if  (src_file == NULL)
        {
                printf("form_path(src) failed\n");
                exit(-1);
        }
        char* dst_file = form_path(dst_path, entry);
        if (dst_file == NULL)
        {
                printf("form_path(dst) failed\n");
                exit(-1);
        }
        copy_file(src_file,dst_file);

        free(src_file);
        free(dst_file);
}


void handle_dir(char* src_path, char* dst_path, char* entry)
{
        int errno;
        char* src_dir = form_path(src_path, entry);
        if  (src_dir == NULL)
        {
                printf("In function handle_dir: form_path(src) failed\n");
                exit(-1);
        }
        char* dst_dir = form_path(dst_path, entry);
        if (dst_dir == NULL)
        {
                printf("In function handle_dir: form_path(dst) failed\n");
                exit(-1);
        }
        if ((mkdir(dst_dir, 0777) == -1) && (errno != EEXIST))
        {
                printf("In function handle_dir: can't create a directory %s\n", dst_dir);
                printf("mkdir() failedi\n");
                exit(-1);
        }
        dir_walk(src_dir, dst_dir);
}
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
        int ofd = open(dst_file, O_CREAT | O_WRONLY | O_TRUNC, stat.st_mode);
        if (ofd == -1)
        {
                printf("In function copy_file: can't open %s\n", dst_file);
                exit(-1);
        }

        char buf[4096];
        ssize_t read_bytes;
        while ((read_bytes = read(ifd, buf, sizeof(buf))) > 0)
        {
                if (write(ofd, buf, read_bytes) != read_bytes)
                {
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

        if (argc != 3)
        {
                printf("Incorrect INPUT!\n", argv[0]);
                printf("For correct use of programm you should write ./1 <copy dir> <dest dir>\n");
                exit(-1);
        }

        char *s1 = realpath(argv[1], NULL);
        if (s1 == NULL)
        {
                printf("realpath failed\n");
                free(s1);
                exit(-1);
        }
        char *s2 = realpath(argv[2], NULL);
        if (s2 == NULL)
        {
                printf("realpath failed\n");
                free(s2);
                exit(-1);
        }

        dir_walk(s1,s2);

        free(s1);
        free(s2);

        return 0;
}

                         
