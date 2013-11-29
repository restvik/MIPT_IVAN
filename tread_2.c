#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#include <time.h>


// This program sorts the matrix lines


int l;
int *a;
int lev;

struct struct_th{
        int c;
        int j;
        int k;
        int wl;
};
// Get  matrix

int openf(){

        char *s,*sh;
        int fd,n=0,bs;
        int flag=1;

        sh=(char*)malloc(sizeof(char));
        s=(char*)malloc(n*sizeof(char));

        (void)umask(0);

        if((fd=open("matrix.txt", O_RDONLY))<0){
                printf("Can't read file!\n");
                exit(-1);
        }


        //serch  lengt of each posichion for 1 elemnt of matrix
        while(flag){
                if(read(fd,sh,1)<0){
                        printf("Can't read file!\n");
                        exit(-1);
                }
                if(sh[0]==' ')
                        flag=0;
                else{
                        n++;
                        s=(char*)realloc(s,n*sizeof(char));
                        strcat(s,sh);
                }

        }

        //get size of matrix

        bs=atof(s);
        flag=1;
        free(s);
        s=(char*)malloc(0);
        n=0;

        while(flag){
                if(read(fd,sh,1)<0){
                        printf("Can't read file!\n");
                        exit(-1);
                }
                if(sh[0]=='\n')
                        flag=0;
                else{
                        n++;
                        s=(char*)realloc(s,n*sizeof(char));
                        strcat(s,sh);
                }
        }

        // makeing massiv a[]
        n=atof(s);
        free(s);
        sh=(char*)realloc(sh,bs*sizeof(char));
        a=(int*)malloc(n*n*sizeof(int));

        int i,j,k;

        for(i=0;i<n*n;i++){
                if(read(fd,sh,bs+1)<0){
                        printf("Can't read file!\n");
                        exit(-1);
                }

                a[i]=atof(sh);

        }

        free(sh);

                                 
        lev=n;
        return bs;
}

// bubble ssort
void  bsort(int wl,int j,int k,int c){

        for(j=0;j<lev;j++)
                for(k=j;k>0;k--)
                        if(a[wl*lev+k]<a[wl*lev+k-1]){
                                c=a[wl*lev+k];
                                a[wl*lev+k]=a[wl*lev+k-1];
                                a[wl*lev+k-1]=c;
                        }
}


// mission for thread
void* mythread(void *arguments)
{
        struct struct_th *arg;

        arg=(struct struct_th *)arguments;

        while((arg->wl)<lev){
                bsort(arg->wl,arg->j,arg->k,arg->c);
                (arg->wl)=l;
                l++;
        }

        return NULL;
}


//works of threads

void thwork(int m){

        int j,k,res;
        struct struct_th *sth;
        pthread_t *thid;

        thid=(pthread_t*)malloc((m)*sizeof(pthread_t));
        sth=(struct struct_th*)malloc((m)*sizeof(struct struct_th));

        thid[0]=pthread_self();

        for(k=0;k<m;k++){
                sth[k].k=0;
                sth[k].j=0;
                sth[k].wl=k;
        }

        int ft;
        ft=time(NULL);

        for(k=1;k<m;k++){
                res=pthread_create(&thid[k],NULL,&mythread,&sth[k]);
                if(res!=0){
                        printf("ERROR on thread creat, return value = %d\n",res);
                        exit(-1);
                }
        }

        mythread(&sth[0]);

        for(k=1;k<m;k++){
                pthread_join(thid[k],NULL);
        }

        ft=time(NULL)-ft;
        printf("time in seconds: %d\n",ft);

        free(sth);

}


int step(int i){
        int s=1,j;

        for(j=0;j<i;j++){
                s=s*10;
        }

        return s;
}



char* genstr(int bs,char *s,int a){
        int i,j=bs,b,n;

        b=a;

        for(i=0;i<bs;i++){
                n=step(j-1);
                s[i]='0'+b/n;
                b=b-(b/n)*n;
                j--;
        }

        return s;

}


void madef(int n, int bs){

        int fd;
        int i,j;
        char *s;


        s=(char*)malloc(bs*sizeof(char));

        (void)umask(0);

        if((fd=open("out.txt",O_WRONLY | O_CREAT | O_TRUNC, 0666))<0){
                printf("Can't open file!\n");
                exit(-1);
        }


        // write in file matrix
        for(i=0;i<n;i++){
                for(j=0;j<n;j++){
                        genstr(bs,s,a[i*n+j]);
                        if((write(fd,s,bs))<0){
                                printf("Can't write a symbol!\n");
                                exit(-1);
                        }
                        if(j!=(n-1)){
                                if((write(fd," ",1))<0){
                                        printf("Can'r write spase!\n");
                                        exit(-1);
                                }
                        }
                }
                if(write(fd,"\n",1)<0){
                        printf("Can't write in file!\n");
                        exit(-1);
                }
        }

        if(close(fd)<0){
                printf("Can't close file\n");
                exit(-1);
        }

        free(s);
}



int main(){
        int m,n;
        int bs;

        a=(int*)malloc(0*sizeof(int));

        printf("How many threads?!\n");
        scanf("%d",&m);

        l=m;

        bs=openf(a);

        thwork(m);

        madef(lev,bs);

        return 0;
}
