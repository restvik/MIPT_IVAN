#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <string.h>

#define LOGFILE "log.txt"
#define KEYFILE "mat.txt"
#define MAXLEN 15
#define BY 1
#define STOPWORK 2
#define WORKDONE 3

int num_cl;//client nomber
int cln_num;//number of clients at all

void sem_a(int sid);
void sem_d(int sid);
int open_file();
int make_sem();
void del_sem(int sid);
void write_log (char* mess, int sid, int fid);
void  bsort(int mas[]);

void make_queue(){
        int msqid, fid, sid;
        key_t key;
        struct mymsgbuf{
                 long mtype;
                 int mat_part[MAXLEN+1];
        }mybuf;
        fid = open_file();
        sid = make_sem();
        if (num_cl == 1)
                sem_a(sid);
//      Generation of IPC key
        if((key = ftok(KEYFILE,0)) < 0){
                printf("Can't generate key\n");
                exit(-1);
        }
        //Get access for key
        if((msqid = msgget(key, 0666 | IPC_CREAT)) < 0){
                printf("Can't get msqid\n");
                exit(-1);
        }
        int flag=1;
        // Client start receive and send massages
        while(flag){
                //rsv massage
                if ( (msgrcv(msqid, &mybuf,(MAXLEN+1)*sizeof(int), num_cl + cln_num, 0)) < 0){
                        printf("can't receive message in CLIENT!\n");
                        exit(-1);
                }
                write_log("CLN RSV\n", sid, fid);
                //analyse of massage
                if(mybuf.mat_part[0] == STOPWORK)
                        flag = 0;
                else{
                        //last massage in queue dekite all ipc
                        if(mybuf.mat_part[0] == BY){
                                if ((msgrcv(msqid, &mybuf,(MAXLEN+1)*sizeof(int),3*cln_num, 0)) < 0){
                                          printf("can't receive message in LAST CLIENT!\n");
                                          exit(-1);
                                }
                                if (semctl(sid, 0, IPC_RMID, 0)<0){
                                          printf("ERROR in delite semophore\n");
                                          exit(-1);
                                }
                                if( msgctl(msqid, IPC_RMID, NULL) < 0){
                                          printf("ERROR in delite queue\n");
                                          exit(-1);
                                }
                                if( close(fid) < 0){
                                          printf("ERROR in close file\n");
                                          exit(-1);
                                }

                        }
                        //if it's not last massage we just close client
                        else{
                                mybuf.mtype = num_cl;
                                mybuf.mat_part[0] = WORKDONE;
                                bsort(mybuf.mat_part);
                                if( (msgsnd(msqid, &mybuf, (MAXLEN+1)*sizeof(int), 0)) < 0){
                                printf("can't send  message in CLIENT!\n");
                                exit(-1);
                                }
                                write_log("CLN SND\n", sid, fid);
                        }
                }
        }
        write_log("I'm FINISH\n", sid, fid);
}
void  bsort(int mas[]){
        int j,k,c;
        for(j = 1; j <= MAXLEN; j++)
                for(k = j; k > 1; k--)
                        if( mas[k] < mas[k-1]){
                                c = mas[k];
                                mas[k] = mas[k-1];
                                mas[k-1] = c;
                        }
}

int  open_file(){
        int fd;
        (void)umask(0);
        if((fd = open(LOGFILE,  O_RDWR | O_CREAT | O_TRUNC | O_APPEND, 0666)) < 0){
                printf("Can't open file!\n");
                exit(-1);
        }
        return fd;
}

int make_sem(){
        key_t skey;
        int sid;

        skey = ftok (KEYFILE , 1);
        if (skey < 0){
                printf("ERROR in ftok()\n");
                exit(-1);
        }
        sid = semget (skey , 1 , IPC_CREAT | 0666);
        if (sid < 0){
                printf("ERROR in smget\n");
                exit(-1);
        }
        return sid;
}
void write_log (char* mess, int sid, int fid) {
        sem_d (sid);

        if (write (fid, mess, strlen (mess)) < 0){
                printf("ERROR in write() work!\n");
                exit(-1);
        }

        sem_a (sid);
}


void sem_a(int sid) {
        struct sembuf mybuf;
        mybuf.sem_op = 1;
        mybuf.sem_flg = 0;
        mybuf.sem_num = 0;
        if (semop (sid, &mybuf, 1) < 0){
                printf("ERROR in work semop in A\n");
                exit(-1);
        }
}
void sem_d(int sid) {
        struct sembuf mybuf;
        mybuf.sem_op = -1;
        mybuf.sem_flg = 0;
        mybuf.sem_num = 0;
        if (semop (sid, &mybuf, 1) < 0){
                printf("ERROR in work semop in D\n");
                exit(-1);
        }
}
int main(int argc,char  ** argv){
        if(argc!=3){
                printf("ERROR in call one of the client\n");
                exit(-1);
        }
        cln_num=atoi(argv[2]);
        num_cl=atoi(argv[1]);
        make_queue();

        return 0;
}
         
