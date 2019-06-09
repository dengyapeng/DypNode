# echoserver
## 将本地git仓库同步至Github远程仓库
### 创建本地git仓库
+ mkdir test
+ git init 
### 将文件存入仓库
+ git add .  将当前目录所有文件存入临时区
+ git add filename  将指定文件存入临时区
+ git commit -m "第一次提交"    //提交到仓库
### 将本地仓库关联到Github上
+ git remote add origin url_of_your_newrepository

+ git push -u origin master  上传代码到远程仓库

  
___

## 第三章 套接字编程介绍

1. 套接字地址结构

   IPv4 套接字地址结构(**又称网际套接地址机构**)

   ~~~c
   struct in_addr{
       in_addr_t s_addr; //存储32位ipv4地址
   };
   struct sockaddr_in{
       uint8_t 	 sin_len;
       sa_family_t  sin_family;/*套接字地址结构的地址族 如AF_INET*/
       in_port_t 	 sin_port;//16-bit TCP或UDP 端口号 总是以网络字节序(2进制)来储存
       struct 		 in_addr sin_addr;//存储32位ipv4地址的结构
       char 		 sin_zero[8];//unused
   };
   ~~~

2. 通用套接字地址结构

   当作为一个参数传递进任何套接字函数时，套接字结构**总是以引用形式(也就是以指向该结构的指针)来传递**。

   然而以这样的指针作为参数之一的**任何套接字函数**必须处理来自所支持的**任何协议族的套接字地址结构**。

   因此定义了一个通用的套接字地址结构。

   ~~~c
   struct sockaddr{
       uint8_t sa_len;
       sa_family_t sa_family;//地址族 AF_XXX
       char sa_data[14];//protocol-specific address
   };
   ~~~

   套接字函数被定义为以指向某个**通用套接字地址结构的一个指针**作为参数

   如bind函数：

   ~~~c
   int bind(int ,struct sockaddr*,socklen_t);
   
   struct sockaddr_in serv;
   bind(sockfd,(struct sockaddr*)&serv,sizeof(serv));/*必须将指向特定于协议的套接字地址的指针进行强制类型转换，变成指向某个通用套接字地址结构的指针*/
   ~~~

3. IPV6套接字结构同IPV4类似，略

## 值-结果参数

1. 当往一个套接字函数传递一个套接字地址结构时，该结构总是以引用方式来传递，也就是说传递的是指向该结构的一个指针。该结构的长度也作为一个参数来传递，不过其传递方式取决于该结构的传递方式：是**从进程到内核**，还是**从内核到进程**。

   * 从进程到内核传递套接字结构的3个函数：bind,connect,和sendto。

     这些函数中一个参数是指向某个套接字结构的指针，另一个参数是该结构的整数大小。

     例如：

      ```c
     struct sockaddr_in serv;
     connect(sockfd,(struct sockaddr*)&serv,sizeof(serv));
      ```

     既然指针和指针所指内容的大小都传递给了内核，于是内核知道到底需要从进程负责多少数据进来。

   * 从内核到进程传递套接字结构的4个函数：accept,recvfrom,getsockname和getpeername。

     这些函数中的一个参数是指向某个套接字地址结构的指针，另一个参数指向**表示该结构大小的整数变量的指针**。

     例如：

     ``` c
     struct sockaddr_un cli;
     socklen_t len;
     
     len = sizeof(cli);//值-结果参数标志
     getpeername(unixfd, (struct sockaddr*)&cli,&len)//传递的是len的引用
     ```

     

     总结：

     当函数被调用时，结构大小是一个值，它告诉内核该结构的大小，这样内核写该数据时不至于越界；

     当该函数返回时，结构大小是一个结果，它告诉进程 内核在该结构中究竟存储了多少信息。

     这种类型的参数称为值-结果参数。

     

## 字节排序函数

* 内存中存储字节的两种方式：一种是将低序字节存储在起始地址，这称为小端(little-endian)字节序；另一种方法是将高序列字节存储在起始地址，这称为大端(big-endian)字节序。

* 网际协议使用大端字节序来传送这些多字节整数

     ### 主机字节序和网络字节序之间的相互转换(涉及到4个函数)

     ```c
     uint16_t htons(uint16_t host16bitvalue)
     uint32_t htonl(uint32_t host32bitvalue)			//均返回：网络字节序的值
     uint16_t ntohs(uint16_t net16bitvalue)
     uint32_t ntohl(uint32_t net32bitvalue)			//均返回：主机字节序的值
     /* h代表host n代表network s代表short l代表long*/
     /* s视为一个16位的值(如tcp/udp端口号) l视为一个32位的值(如Ipv4地址) */
     ```

     
## 字节操纵函数

```c
#include <strings.h>

void bzero(void *dest,size_t nbytes);//把目标字节串中指定数目的字节置为0
void bcopy(const void *src,void *dest,size_t nbytes);//将指定数目的字节从源字节串移到目标字节串
int  bcmp(void *ptrl,const void *ptr2,size_t nbytes);//比较两个任意的字节串，若相同则返回值为0，否则返回值为非0
```



## 两组地址转换函数

它们在ASCII字符串(人门钟爱的点分十进制串)与网络字节序的二进制(存放在套接字地址结构中的值)之间转换网际地址。



``` c
/*这一组只适用与Ipv4地址*/
#include <arpa/inet.h>

int inet_aton(const char *strptr,struct in_addr *addrptr);//返回：若字符串有效则为1，否则为0
char *inet_ntoa(struct in_addr inaddr);//返回：指向一个点分十进制数串的指针
```

第一个函数inet_aton将strptr所指C字符串转换成一个32位的网络字节序二进制值，通过指针addrptr存储。

第二个函数inet_ntoa将一个32位的网络字节序二进制Ipv4地址转换成相应的点分十进制数串，由于这个函数返回值所指向的字符串驻留在静态内存中。意味着这个函数是不可重入的。



___

下面这两个函数是Ipv6出现的新函数，对于Ipv4和Ipv6地址都适用。

函数名中的p代表ASCII字符串，n代表套接字地址结构中的二进制值。

```c
#include <arpa/inet.h>

int inet_pton(int family,const *strptr, void *addrptr);//返回： 若成功则为1，若输入不是有效的表达式格式则为0，若出错则为-1

const char *inet_ntop (int family,const void *addrptr,char *strptr,size_t len);
//返回 若成功则为指向结果的指针，若出错则为NULL
//len参数是目标储存单元的大小，以免该函数溢出其调用者的缓冲区。IPV6 46 ipv4 16
```

这两个函数的family参数既可以是AF_INET,也可以是AF_INET。















___
# 第四章 基本TCP套接字编程

**1.socket 函数**

为了执行网络I/O，一个进程必须做的第一件事就是调用socket函数，指定期望的通信协议类型

```c
#include<sys/socket.h>
int socket(int family, int type,int protocol);//返回：若成功则为非负描述符，若出错则为-1
```

socket函数在成功时返回一个小的非负整数值，它与文件描述符类似，我们把它称为套接字描述符，简称sockfd。为了得到这个套接字描述符，我们只是指定了协议族和套接字类型。我们**并没有指定本地协议地址或远程协议地址**。

family参数指明协议族，type参数指明套接字类型，protocol参数应设为某个协议类型常值，或设为0，以选择所给定family和type组合的系统默认值。

|  family  |      说明      |
| :------: | :------------: |
| AF_INET  |    Ipv4协议    |
| AF_INET6 |    Ipv6协议    |
| AF_LOCAL |   Unix域协议   |
| AF_ROUTE | 路由套接字协议 |
|  AF_KEY  |   密钥套接字   |

|      type      |      说明      |
| :------------: | :------------: |
|  SOCK_STREAM   |  字节流套接字  |
|   SOCK_DGRAM   |  数据报套接字  |
| SOCK_SEQPACKEY | 有序分组套接字 |
|    SOCK_RAW    |   原始套接字   |

|   protocol   |     说明     |
| :----------: | :----------: |
| IPPROTO_TCP  | TCP传输协议  |
| IPPROTO_UDP  | UDP传输协议  |
| IPPROTO_SCTP | SCTP传输协议 |

**2.connect函数**

TCP客户用connect函数来建立与TCP服务器的连接。

```c
#include<sys/socket.h>
int connect(int sockfd,const struct sockaddr *servaddr,socklen_t addrlen);
//返回：若成功则为0，若出错则为-1
```

sockfd是由socket函数返回的套接字描述符，第二个，第三个参数分别是一个指向套接字地址结构的指针和该结构的大小。**套接字地址结构必须含有服务器的IP地址和端口号**。

客户在调用函数connect前不必非得调用函数，因为如果需要的话，内核会确定源IP地址，并选择一个临时端口作为端口号。

若为TCP套接字，调用connect 函数将激发TCP的三路握手过程，而且仅在连接建立成功或出错时才返回。

其中的出错返回可能有以下几种情况:

 * 若TCP客户没有收到SYN分节的响应，则返回ETIMEDOUT错误。
* 若对客户的SYN的响应是RST(表示复位),则表明该服务器主机在我们所指定的端口上没有进程在等待与之连接，收到RST马上返回ECONNREFUSED错误。这是一种硬错误
* 若客户发出的SYN在中间的某个路由器上引发了一个"destination unreachable"目的地不可达ICMP错误。客户主机内核保存该消息后，并按第一种情况中所述的时间间隔继续发送SYN。若在某个规定时间后仍然未收到响应，则把保存的消息作为EHOSTUNREACH或ENETUNREACH错误返回给进程。

**3.bind函数**

bind函数把一个本地协议地址赋予一个套接字。对于网际网协议，协议地址是32位的IPv4地址或128位的IPv6地址与16位的TCP或UDP端口号的组合。

```c
#include<sys/socket.h>
int bind(int sockfd,const struct sockaddr*myaddr,socklen_t addrlen);
//返回：若成功则为0，若出错则为-1

//example
struct sockaddr_in servaddr;
listenfd = socket(AF_INET,SOCK_STREAM,0);
bzero(&servaddr,sizeof(servaddr));
servaddr.sin_family = AF_INET;
servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
servaddr.sin_port = htons(13);
bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
```

第二个参数是一个指向特定于协议的地址结构的指针，第三个参数是该地址结构的长度。对于TCP，调用bind函数可以指定一个端口号或指定一个IP地址，也可以两者都指定，还可以都不指定。

* 服务器在启动时通常会绑定它们的众所周知端口。若一个TCP客户或服务器未曾调用bind绑定一个端口，当调用connect或listen时，内核就要为相应的套接字选择一个临时端口。

* 进程可以把一个特定的IP地址绑定到它的套接字上，不过这个**IP地址必须属于其所在主机的网络接口之一**。

  对于TCP客户端，这就为在该套接字上**发送的IP数据报指派了源IP地址**。

  对于TCP服务器，这就限定该套接字**只接收那些目的地为这个IP地址的客户连接**。

  TCP客户端通常不把IP地址绑定到它的套接字上。当连接套接字时，**内核**将**根据所用外出网络接口**来**选择源IP地址**。

  若TCP服务器没有把IP地址绑定到它的套接字上，内核就把客户发送的SYN的目的IP地址作为服务器的源IP地址。

![](C:\Users\dell\Desktop\捕获g.PNG)

___



对于IPv4来说，通配地址由常量INADDR_ANY来指定，其值一般为0。他告知内核去选择IP地址

```c
struct sockaddr_in servaddr;
servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
```

  对于IPv6来说

```c
struct sockaddr_in6 serv;
serv.sin6_addr = in6addr_any;
```

注意如果让内核来为套接字选择一个临时端口号，那么必须注意，函数bind并不返回所选择的值。实际上由于bind函数的第二个参数有const限定词，它无法返回所选之值为了得到内核所选择的这个临时端口值，必须调用getsockname来返回协议地址。

​	

**4.listen函数**

listen函数仅由TCP服务器调用，它做两件事。

(1) listen函数把一个未连接的套接字转换成一个被动套接字，指示内核应接受指向该套接字的连接请求。

(2) 本函数的第二个参数规定了内核应该为相应套接字排队的最大连接个数。

```c
#include<sys/socket.h>
int listen(int sockfd,int backlog);
//返回: 若成功则为0，若出错则为-1
```

理解backlog参数，我们必须认识到内核为**任何一个给定的监听套接字维护两个队列**

 *  未完成连接队列，这些套接字处于SYN_RCVD状态。 
 *  已完成连接队列，这些套接字处于ESTABLISHED状态。

当三路握手完成时，该项就从未完成连接队列移到已完成连接队列的队尾。当进程调用accept时，已完成连接队列中的队头项将返回给进程，或者如果该队列为空，那么进程将被投入睡眠，直到TCP在该队列中放入一项才唤醒它。

注意不要把backlog的值设为0，因为不同的实现对此有不同的解释。



**5.accept函数**

   accept函数由TCP服务器调用，用于从已完成连接队列队头返回下一个已完成的连接。如果已完成连接队列为空，那么进程将被投入睡眠。

```c
#include <sys/socket.h>
socklen_t len;
struct sockadddr_in cliaddr;
len=sizeof(cliaddr);
int accept(int sockfd, struct sockaddr *cliaddr,socklen_t *addrlen);
//返回 若成功则为非负描述符，若出错则为-1
```

参数cliaddr和addrlen用来返回已连接的客户的协议地址。addrlen是值-结果参数；调用前，我们将*addrlen值置为由cliaddr所指的套接字地址结构的长度。返回时，该整数值(len)即为由内核存放在该套接字结构内的确切字节数。(len值很可能改变)。

如果accept成功，将返回一个已连接套接字描述符。当服务器完成对某个给定客户的服务时，相应的已连接套接字就被关闭。如果不对客户端信息感兴趣，可以将后面两个参数都置为空。



**6.fork函数**

```c
#include<unisted.h>
pid_t fork(void);//返回：在子进程中为0，在父进程中为子进程ID，若出错则为-1
```

fork的难以理解之处在于调用它一次，它却返回两次。它在父进程中返回一次，返回值是子进程的进程ID号；

在子进程又返回一次，返回值为0。



**7.并发服务器**

采用fork实现的简易并发服务器

```c
pid_t pid;
int listenfd,connfd;
listenfd = socket(...);
bind(listenfd,...);
listen(listenfd,LISTENQ);

for( ; ; ){
    connfd = accept(listenfd,...);
    if(pid = fork()==0){
        close(listenfd);/* child closes listening socket */
        doit(connfd);   /* process the request */
        close(connfd);  /* done with this client */ //可以忽略
        exit(0);  		/* child terminates */ //exit会直接关闭connfd
    }
    close(connfd);		/* parent closes connected socket */
}
```

当一个连接建立时，accept返回，服务器接着调用fork,然后由子进程服务客户(通过已连接套接字connfd)，父进程则等待下一个连接(通过监听套接字listenfd)。既然新的客户由子进程提供服务，父进程就关闭已连接套接字。



**8.close函数**

用于关闭套接字，并终止TCP连接。

```c
#include <unistd.h>
int close(int sockfd);//返回： 若成功则为0，若出错则为-1；
```



**9.getsockname和getpeername函数**

getsockname返回与某个套接字关联的本地协议地址，gepeername返回与某个套接字关联的外地协议地址。

```c
#include <sys/socket.h>

int getsockname(int sockfd,struct sockaddr *localaddr,socklen_t *addrlen);

int getpeername(int sockfd, struct sockaddr *peeraddr,socklen_t *addrlen);
//均返回: 若成功则为0，若出错则为-1
//两个函数的最后一个参数都为值-结果参数
```

