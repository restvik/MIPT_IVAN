#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define din 30 // dinomenator for ganaration random nombers


// getting  buffer size

int gbs(){
        int bs=0;
        int m=din;

        while(m>1){
                bs++;
                m=m/10;
        }

        return bs;
}

//generate a matrix

void matgen(int n,int *a){

        int i=0,j=0;

        srand(time(NULL));

        for(i=0;i<n;i++)
                for(j=0;j<n;j++)
                        (a[i*n+j])=(rand()%din);
}

//get step i
int step(int i){
        int s=1,j;

        for(j=0;j<i;j++){
                s=s*10;
        }

        return s;
}

// genaration str
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

// puttng matrix in file

void madef(int n,int *a,char *s2){

        int fd;
        int i,j;
        char *s;
        int bs;

        bs=gbs();

        s=(char*)malloc(bs*sizeof(char));

        (void)umask(0);

        if((fd=open("matrix.txt",O_WRONLY | O_CREAT | O_TRUNC, 0666))<0){
                printf("Can't open file!\n");
                exit(-1);
        }


        // write in file bs

        genstr(bs,s,bs);
        if((write(fd,s,bs))<0){
                printf("Can't write a symbol!\n");
                exit(-1);
        }

        if((write(fd," ",1))<0){
                printf("Can't write  spase!\n");
                exit(-1);
        }

        // write size of matrix
        if((write(fd,s2,strlen(s2)))<0){
                printf("Can't write n!\n");
                exit(-1);
        }

        if((write(fd,"\n",1))<0){
                printf("Can't write  enter!\n");
                exit(-1);
        }


        // write in file matrix
        for(i=0;i<n;i++){
                for(j=0;j<n;j++){
                        genstr(bs,s,a[i*n+j]);
                        if((write(fd,s,bs))<0){
                                printf("Can't write element of matrix!\n");
                                exit(-1);
                        }
                        if(j!=(n-1))
                                if((write(fd," ",1))<0){
                                        printf("Can't write element of matrix!\n ");
                                        exit(-1);
                                }
                }
                if(write(fd,"\n",1)<0){
                        printf("Can't write in file!\n");
                        exit(-1);
                }
        }

        free(s);

        if(close(fd)<0){
                printf("Can't close file\n");
                exit(-1);
        }


}

int main(int argc, char** argv){
        int n,i,j;
        int *a;

        if(argc<2){
                printf("You not write a size of matrix\n");
                exit(-1);
        }
        else
                if(argc==2)
                        n=atof(argv[1]);
                else{
                        printf("A lot of arguments\n");
                        exit(-1);
                }

        a=(int*)malloc(n*n*sizeof(int));

        matgen(n,a);
        madef(n,a,argv[1]);


        free(a);

        return 0;
}
                        
