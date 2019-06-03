#include "head.h"
void str_echo(int sockfd)
{
	ssize_t n;
	char buf[MAXLINE];
again:
	while((n=read(sockfd,buf,MAXLINE))>0)
	{
		write(sockfd,buf,n);
		if(n<0)
		goto again;
		else if(n<0)
		printf("str_echo");
	}
}
int main(int argc,char **argv)
{
	int listenfd,connfd;// 创建一个监听文件描述符，一个已连接文件描述符
	pid_t childpid;//设置一个pid变量
	socklen_t clilen;
	struct sockaddr_in cliaddr,servaddr;

	listenfd = socket(AF_INET,SOCK_STREAM,0);
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;
	servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
	servaddr.sin_port=htons(SERV_PORT);
	bind(listenfd,(SA*)&servaddr,sizeof(servaddr));
	listen(listenfd,LISTENQ);
	for(;;)
	{
		clilen = sizeof(cliaddr);
		connfd=accept(listenfd,(SA*)&cliaddr,&clilen);
		if((childpid=fork())==0)
		{
			close(listenfd);
			str_echo(connfd);
			exit(0);
		}
		close(connfd);
	}
}
