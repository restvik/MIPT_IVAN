#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <pthread.h>

#define KEYFILE "mat.txt"
#define OUTFILE "out.txt"
#define BY 1
#define STOPWORK 2
#define DOWORK 4
#define MAXLEN 15
//mas - place where we will store our matrix
// mat_size - size of matrix
//buf_size - show me how much space i use for one element of matrix
//line_ind - indicator which show for us what next line will sort
//cln_num - number of clientd
//ff - finish flag which told us when we must recive massage BY and when just told client FINISH his work
int *mat, mat_size, buf_size, line_id, msqid, cln_num,ff=0;
//struct for threads
struct  STRUCT_TH{
        int num_work_line;//line wich thread will sort
        int cln_id;
};
struct mymsgbuf{
        long mtype;
        int mat_part[MAXLEN+1];
} mybuf;

int step(int i);
char* genstr(char *char_num, int ent_num);
// open_file - function whuch get in matrix file all information about matrix.
void  open_file(char *file_name){
        char *buf_str = malloc(1), *help_buf_str = malloc(1);
        int fd, b_str_size = 0;

        // Opent file for get matrix
        (void)umask(0);
        if((fd = open(file_name, O_RDONLY)) < 0){
                printf("Can't open file!\n");
                exit(-1);
        }
        //serch  lengt of each posichion for one elemnt of matrix
        int flag = 1;
        while(flag){
                //I read one symboluntil I can't fins a space for(I do it because I don't no how much space I use for one element of matrix)
                if(read(fd, help_buf_str, 1) < 0){
                        printf("Can't read file!\n");
                        exit(-1);
                }
                if(help_buf_str[0] == ' ')
                        flag = 0;
                else{
                        b_str_size++;
                        buf_str = (char*)realloc(buf_str, b_str_size*sizeof(char));
                        strcat(buf_str, help_buf_str);
                }

        }
        //get size of matrix
        buf_size = atoi(buf_str);
        flag = 1;
        free(buf_str);
        buf_str = (char*)malloc(0);
        b_str_size = 0;

        while(flag){
                if(read(fd, help_buf_str, 1)<0){
                        printf("Can't read file!\n");
                        exit(-1);
                }
                if(help_buf_str[0] == '\n')
                        flag = 0;
                else{
                        b_str_size++;
                        buf_str = (char*)realloc(buf_str, b_str_size*sizeof(char));
                        strcat(buf_str, help_buf_str);
                }
        }
        // makeing massiv mas[]
        mat_size = atoi(buf_str);
        free(buf_str);
        help_buf_str = (char*)realloc(help_buf_str, buf_size*sizeof(char));
        mat=(int*)malloc(mat_size*mat_size*sizeof(int));

        int i, read_size;
        for(i=0; i < mat_size*mat_size; i++){
                if( (read_size = read(fd, help_buf_str, buf_size+1)) < 0){
                        printf("Can't read file!\n");
                        exit(-1);
                }
                else
                        if(read_size != buf_size+1){
                                printf("Incorrect read() work!\n");
                                exit(-1);
                        }
                mat[i]=atoi(help_buf_str);
        }
        free(help_buf_str);
}
void madef(){
        int fd, i, j;
        char *buf_str=malloc(buf_size);
        (void)umask(0);
        if((fd = open(OUTFILE , O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0){
                printf("Can't open file out.txt!\n");
                exit(-1);
        }
        // write in file matrix
        for(i=0; i < mat_size; i++){
                for(j=0; j < mat_size; j++){
                        genstr(buf_str, mat[i*mat_size+j]);
                        if( (write(fd, buf_str, buf_size)) != buf_size){
                                printf("Can't write in file!\n");
                                exit(-1);
                        }
                        if(j != (mat_size-1)){
                                if((write(fd, " ", 1)) < 0){
                                        printf("Can'r write spase!\n");
                                        exit(-1);
                                }
                        }
                }
                if(write(fd, "\n", 1)<0){
                        printf("Can't write in file!\n");
                        exit(-1);
                }
        }
        if(close(fd) < 0){
                printf("Can't close file\n");
                exit(-1);
        }
        free(buf_str);
}
//Programm which take a number and reten 10^number
int step(int num){
        int stepi = 1, j;
        for(j = 0; j < num; j++){
                stepi=stepi*10;
        }
        return stepi;
}
//Programm which interprets nomber from int to char
char* genstr(char *char_num, int ent_num){
        int i, j=buf_size, help_num = ent_num, stepi;
        for(i=0; i<buf_size; i++){
                stepi = step(j-1);
                char_num[i] = '0' + help_num/stepi;
                help_num = help_num - (help_num / stepi)*stepi;
                j--;
        }
        return char_num;
}
// generation matrix for send message
//Functn for one thread
void* mythread(void *arguments)
{
        int i;
        struct STRUCT_TH *arg = (struct STRUCT_TH *)arguments;
        struct mymsgbuf{
                long mtype;
                int mat_part[MAXLEN+1];
        } mybuf;


       while((arg->num_work_line) < mat_size){
                // Send message to client
                mybuf.mtype = arg->cln_id + cln_num;
                for(i = 0; i < mat_size; i++)
                        mybuf.mat_part[i + 1] = mat[arg->num_work_line * mat_size + i];
                mybuf.mat_part[0] = DOWORK;
                if( (msgsnd(msqid, &mybuf, (MAXLEN+1)*sizeof(int), 0)) < 0){
                         printf("ERROR can't send message in tread #%i!\n", arg->cln_id);
                         exit(-1);
                }
                // receive message from client
                if ( (msgrcv(msqid, &mybuf,(MAXLEN+1)*sizeof(int), arg->cln_id, 0)) < 0){
                        printf("ERROR can't receive message in thread #%i!\n", arg->cln_id);
                        exit(-1);
                }
                for(i = 0; i < mat_size; i++)
                        mat[arg->num_work_line * mat_size+i]=mybuf.mat_part[i+1];
                (arg->num_work_line) = line_id;
                line_id++;
        };
        mybuf.mtype = arg->cln_id + cln_num;
        if(ff != cln_num)
                mybuf.mat_part[0] = STOPWORK;
        else
                mybuf.mat_part[0] = BY;
        ff++;
        if( (msgsnd(msqid, &mybuf, (MAXLEN+1)*sizeof(int), 0)) < 0){
                         printf("ERROR can't send FINAL  message in #%i!\n", arg->cln_id);
                         exit(-1);
        }
        return NULL;
}
//organising nom_cln threads
void thwork(int cln_num){
        int j,k,res;
        key_t key, sem_key;
        struct STRUCT_TH *sth;
        pthread_t *thid;

        thid=(pthread_t*)malloc((cln_num)*sizeof(pthread_t));
        sth=(struct STRUCT_TH*)malloc((cln_num)*sizeof(struct STRUCT_TH));

        key = ftok(KEYFILE, 0);
        if(key < 0){
                printf("ERROR in ftok!\n");
                exit(-1);
        }
        msqid = msgget(key, 0666 | IPC_CREAT);
        if(msqid < 0){
                printf("ERROR in msgget!\n");
                exit(-1);
        }
        thid[0] = pthread_self();
        for(k = 0; k < cln_num; k++){
                sth[k].num_work_line = k;
                sth[k].cln_id=k+1;
        }
        for(k=1; k < cln_num; k++){
                res=pthread_create(&thid[k], NULL, &mythread, &sth[k]);
                if(res != 0){
                        printf("ERROR on thread creat, return value = %d\n",res);
                        exit(-1);
                }
        }

        mythread(&sth[0]);
        for(k = 1; k < cln_num; k++){
                pthread_join(thid[k], NULL);
        }
        free(sth);
        free(thid);
}
int main(int argc,char **argv){
        if(argc != 3){
                printf("ERROR in call server!\n");
                exit(-1);
        }
        cln_num = atoi(argv[2]);
        line_id = cln_num;
        open_file(argv[1]);
        thwork(cln_num);
        madef();

        return 0;
}
       
