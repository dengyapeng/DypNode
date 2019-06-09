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

2.**connect函数**
