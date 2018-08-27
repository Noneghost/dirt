/*************************************************************************
	> File Name: main.c
	> Author: zc  
	> Mail: zcpython_crawler@yeah.net 
	> Created Time: 2018年08月26日 星期日 21时34分30秒
 ************************************************************************/

#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main(int argc,char *argv[])
{
    FILE* fd = fopen("dict.txt","r");
        if(fd<0)
    {
        perror("fopen");
        return 0;
    }
    char buf[1024];
    char word[20] = "able";
    while(fgets(buf,sizeof(buf),fd)!=NULL)
    {
      //printf("%s",buf);

     if ( !strncmp(word ,strtok(buf," "),sizeof(word))) 
      printf(" %s      %s\n",word , strtok(NULL,"\n") );
        
        memset(buf,0,sizeof(buf));
    
    }
    return 0;
}
