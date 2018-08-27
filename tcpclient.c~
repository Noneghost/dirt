/*************************************************************************
	> File Name: tcpserver.c
	> Author: zc  
	> Mail: zcpython_crawler@yeah.net 
	> Created Time: 2018年08月26日 星期日 16时29分14秒
 ************************************************************************/

#include <stdio.h>
#include <sqlite3.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

struct CMD{
    int id ;
    int mode ;
    char user[16];
    char passwd[16];
    int flag;    // 0 : 不存在   1 ： 存在   2: 密码错误
    char word[1024];
	char history[1024];
	char time[20];
};
struct CMD cmd;
int create_tcp_socket(char *ip_addr  )
{
    int server = socket(AF_INET , SOCK_STREAM , 0);
    int opt = 1;
    setsockopt(server , SOL_SOCKET,SO_REUSEADDR ,(char *)&opt , sizeof(opt));
    if (opt < 0 )
    {
        perror("socket :");
        return -1;
    }
    struct sockaddr_in server_addr ;
    server_addr.sin_family          = AF_INET ; 
    server_addr.sin_addr.s_addr     = inet_addr(ip_addr);
    
    return server ; 
}
void login()
{ 
    static int i=0;
    printf("please enter your user and passwd :\n");
    char user[16];
    char passwd[16];
    while(1)
    {
        printf("user:");
        fgets(user,sizeof(user),stdin);
        if(strlen(user)<=1)
        {
            printf("user can't null \n");
	    memset(user,0,sizeof(user));
            continue;
        }
        user[strlen(user)-1] = '\0';
        printf("user is :%s \n",user);
        printf("passwd:");
        fgets(passwd,sizeof(passwd),stdin);
        if(strlen(passwd)<=6)
        {
            printf("passwd can't less than six numbers \n");
	    memset(passwd,0,sizeof(passwd));
            continue;
        }
        passwd[strlen(passwd)-1] = '\0';
        printf("passwd is %s \n",passwd);
        cmd.mode = 1;
        strcat(cmd.user,user);
        strcat(cmd.passwd,passwd);
	 
        return ;
    }
}
int cmd_manage(int client)
{
    if(cmd.flag == 0)// 进行登陆检测
    {
        login();
        send(client ,(char *)&cmd , sizeof(cmd),0);
        recv(client,(char*)&cmd,sizeof(cmd),0);
        if(cmd.flag == 1)
            printf("登陆成功 \n");
        else if(cmd.flag == 0)
            {		printf("密码错误\n");
		 memset(cmd.user,0,sizeof(cmd.user));
   		 memset(cmd.passwd,0,sizeof(cmd.passwd));
		}
        else if(cmd.flag ==4)
            printf("注册成功 \n");
    }
    if(cmd.flag)
    {
    char buf[1024];
	time_t search_time; 
	struct tm *local_timer;
       while(1)
        {
	if(!strncmp(cmd.word,"HISTORY",strlen("HISTORY"))) // 检测历史记录
		{
		
		printf("%s\n",cmd.history);
        }
		
            cmd.mode = 2;    //登陆成功
            printf("please enter word : ");
            fgets(cmd.word,sizeof(cmd.word),stdin);
            cmd.word[strlen(cmd.word)-1] = '\0';
          	search_time = time(NULL);
			local_timer = localtime(&search_time);
	 		sprintf(cmd.time,"%d-%d-%d %d:%d:%d",local_timer->tm_year+1900,local_timer->tm_mon+1,local_timer->tm_mday, local_timer->tm_hour,local_timer->tm_min,local_timer->tm_sec);    
	
			
			send(client ,(char *)&cmd,sizeof(cmd),0);
            if(!strncmp(cmd.word,"QUIT",strlen("QUIT")))
            {
                printf("client closed \n");
                close(client);
                exit(0);
            }
    	
            memset(&cmd,0,sizeof(cmd));
            recv(client ,(struct CMD *)&cmd,sizeof(cmd),0);
            printf("%s\n",cmd.word);
     
            return 0;
        }
    }
    
    //printf("cmd .flag = %d \n",cmd.flag);
}
void R(int client ,char *server_ip_addr, int server_port)
{
    char buf[1024] ; 
    struct sockaddr_in server_addr ; 
    server_addr.sin_family          = AF_INET;
    server_addr.sin_port            = htons(server_port);
    server_addr.sin_addr.s_addr     = inet_addr(server_ip_addr);
    int ret = connect(client , (struct sockaddr*)&server_addr , sizeof(server_addr));
    if( ret < 0 )
    {
        perror("connect  faile: ");
        return ;
    }
    
    while(1)
    {
		printf("*********************************************\n");
		printf("                   dirt                      \n");
		printf("      查找历史： HISTORY      退出： QUIT     \n");
		printf("*********************************************\n");
        cmd_manage(client);

    }
}
int main(int argc,char *argv[])
{
    int client = create_tcp_socket("127.0.0.1");
    R(client,"127.0.0.1",50000);

    return 0;
}
