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

## 套接字编程常用API

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

4. 

5. 

6. 

7. 

   

   

   

   
