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
#include <signal.h>
#include <sys/wait.h>
struct CMD{
    int id;
    int mode ;
    char user[16];
    char passwd[16];
    int flag ;  //  1 : user 存在   0 ： 不存在 2 : 密码错误
    char word[1024];
	char history[1024];
	char time[20];
};
struct CMD cmd;
int create_tcp_socket(char *ip_addr , int port )
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
    server_addr.sin_port            = htons(port);
    server_addr.sin_addr.s_addr     = inet_addr(ip_addr);
    
    int ret = bind(server , (struct sockaddr*)&server_addr,sizeof(server_addr));
    if( ret < 0 )
    {
        perror(" bind  fail:");
        return -1;
    }
    listen(server , 5 );
    return server ; 
}
void handler(int igno)
{
    while(waitpid(-1,NULL,WNOHANG)>0);
}

static int callback(void *data ,int argc ,char **argv ,char **azColName)
{
    fprintf(stderr,"%s \n" , (const char*)data);
    int i ;
    for(i = 0 ; i <argc ;i++)
    {
        printf("%s = %s \n",azColName[i] , argv[i] ? argv[i] : "NULL");
        if(argv[i] =="zc")
           {cmd.flag = 2;
            return 0; 
           }
    }
    printf("\n");
    return 0;
}
void sql_handler(char *sql,int flag)
{
    sqlite3 *db;
    char * zerromsg = 0;
    int rc ;
    const char * data = "callback function called ";
    rc = sqlite3_open("dirt.db",&db);
    if( rc )
    {
        fprintf(stderr , "can't open database :%s \n ", sqlite3_errmsg(db));
        exit(0);
    }
    else
    {
        fprintf(stderr , " opened database successfly \n");
    }
    
	if(flag == 1)
	{    
			rc = sqlite3_exec(db , sql , callback , (void *)data ,&zerromsg);
		    cmd.flag = 4;   // sigin success 
		    if(rc != SQLITE_OK)
		    {
		        fprintf(stderr,"sql error :%s \n",zerromsg);
		        sqlite3_free(zerromsg);
		    }
		    else
		    {
		        fprintf(stdout , "table created successfully \n");
		    }

	}
	else if(flag ==2 )//历史   未设置 history超过 buf大小  清楚多余的历史记录
	{
		 char **dbResult ; 
		int nline , nColumn;
		char sql_bak[100];
		sprintf(sql_bak,"select word,time from history where username = '%s';",cmd.user);

		 rc = sqlite3_get_table(db,sql_bak,&dbResult,&nline,&nColumn,&zerromsg);
		
		 int sum = nline*nColumn + nColumn;
		 int i ;
		char buf[40];
 		if(rc == SQLITE_OK)
		{
			for( i = 2; i <= sum ; i=i+2)
		        {
		           
		             sprintf(buf,"HISTOY : %s %s\n  ",dbResult[i],dbResult[i+1]);
		             strcat(cmd.history,buf);   
				
		       
		        }
		
	  printf("cmd.history : %s \n",cmd.history);	   
		}
		
	}
	else
	{
		 char **dbResult ; 
		 int nline , nColumn;

		 rc = sqlite3_get_table(db,"select * from user;",&dbResult,&nline,&nColumn,&zerromsg);
		 int sum ;
		sum = nline*nColumn + nColumn;
		 int i ;
		 if(rc == SQLITE_OK)
		    {
		        for( i = 4 ; i <= sum ; i=i+3)
		        {
				printf("i = %d  %s \n",i,dbResult[i]);
		            if(!strncmp(dbResult[i] , cmd.user,strlen(cmd.user)))
		            {
		                if(!strncmp(dbResult[i+1] ,cmd.passwd ,strlen(cmd.passwd)))
				{
					printf("帐号 ： %s 密码： %s\n",cmd.user,cmd.passwd);
		                    cmd.flag = 1;// login success 
		                    printf("xxxxx  : %s\n",dbResult[i]);
		                    sqlite3_close(db);
				 return ;
		                }
		                else
		                {
		                    cmd.flag = 0;  // passwd is error
		                    return ;
		                }
		            }
		        }
		    }
		
	        rc = sqlite3_exec(db , sql , callback , (void *)data ,&zerromsg);
		cmd.flag = 4;   // sigin success 
		if(rc != SQLITE_OK)
		 {
		        fprintf(stderr,"sql error :%s \n",zerromsg);
		        sqlite3_free(zerromsg);
		 }
		    else
		    {
		        fprintf(stdout , "table created successfully \n");
		    }
	}	
	   	

        sqlite3_close(db);
        return ;
}
void cmd_manmage(int client)
{	int flag =0;
    char sql[1024];
    if(cmd.mode == 1) // 注册
    {
        memset(sql,0,sizeof(sql));
        sprintf(sql, "insert into user values(%d,'%s','%s');",cmd.id ,cmd.user,cmd.passwd);
        sql_handler(sql,0);
        
        send(client , (char *)&cmd ,sizeof(cmd) ,0);
    }
    else if(cmd.mode == 2)
    {
        printf("time  %s \n",cmd.time);
      
        FILE * fd = fopen("dict.txt","r");
        if(fd <0)
        {
            perror("fopen");
            return ;
        }
        char buf[1024];
        if(strlen(cmd.word)==0)
        {
            strcat(cmd.word,"null\n");
            send(client,(char*)&cmd,sizeof(cmd),0);
            return ;
        }
        if(!strncmp((cmd.word),"QUIT",strlen("QUIT"))) // 推出
            {
                printf("client closed \n");
                close(client);
                return ;
            }
	else if(!strncmp((cmd.word),"HISTORY",strlen("HISTORY")))//历史
	{	
		memset(sql,0,sizeof(sql));
		memset(&cmd.history,0,sizeof(cmd.history));
	
		sql_handler(sql,2);
 		send(client,(char*)&cmd,sizeof(cmd),0);
		memset(&cmd.history,0,sizeof(cmd.history));
			flag = 1;
			return ;
		
	}
		memset(sql,0,sizeof(sql));
		sprintf(sql, "insert into history values('%s','%s','%s');",cmd.word,cmd.time,cmd.user);
		sql_handler(sql,1);
		
        while(fgets(buf,sizeof(buf),fd)!= NULL)
            { 
			
              if(!strncmp(cmd.word,strtok(buf," "),strlen(cmd.word)))
                {
                    
                    strcat(cmd.word ,strtok(NULL,"\n"));
                    strcat(cmd.word,"\n");
                    printf("%s ",cmd.word);
                    send(client,(char*)&cmd,sizeof(cmd),0);
		flag = 1;
                }
				
				
				

                memset(buf,0,sizeof(buf));
            }
	if(flag ==0)  // 不存在
	{	
			strcat(cmd.word," no exist");
	   		 send(client,(char*)&cmd,sizeof(cmd),0);
				
	}
				


    }
    return ;
}
void R(int server )
{
    struct sockaddr_in client_addr ; 
    int client_addr_len = sizeof(client_addr); 
    char buf[1024];
    signal(SIGCHLD,handler);
    while(1)
    {
    
        int client = accept(server , (struct sockaddr *)&client_addr ,&client_addr_len );
        if((fork()== 0))
        {
        printf("connect ip is %s port is %d \n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
        while(client > 0 )
        {
            recv(client ,(struct CMD *)&cmd , sizeof(cmd), 0);
            cmd_manmage(client); 
         //   printf("rec : id = %d  username :  %s   passwd : %s\n ",cmd.id , cmd.user ,cmd.passwd );

            memset(&cmd , 0,sizeof(cmd));
            

        }
        }
    }
}
int main(int argc,char *argv[])
{
    int server = create_tcp_socket("127.0.0.1",50000 );
    R(server );
    return 0;
}
