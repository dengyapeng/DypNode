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

git clone会自动绑定远程库
### 查看远程库
git remote -v

### 删除远程库
git remote remove origin

### 添加远程库
git remote add  origin url_of_your_newrepository
___
## UNP 笔记

[第三章 套接字编程介绍](Node/c3.md)

[第四章 基本TCP套接字编程](Node/c4.md)

[第六章 I/O复用 select和poll函数](Node/c6.md)


___
## Linux 常用命令
    .tar文件的解压 tar -zcvf etc.tar /root/myApp  将当前目录的etc.tar文件解压到/root/myApp路径下


___ 
## 操作系统笔记
[第四章 进程及进程管理](Node/systemC4.md)

[第五章 资源分配与调度](Node/systemC5.md)

[第六章 主存管理](Node/systemC6.md)

[第七章 设备管理](Node/systemC7.md)
