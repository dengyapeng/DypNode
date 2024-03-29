# 学习笔记
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

### 将远程库更新同步到本地仓库
1.把远程库更新到本地 *git fetch origin master*

2.比较远程更新和本地版本库的差异 *git log master.. origin/master*

3.合并远程库 *git merge origin/master*

### 本地库删除文件后同步到远程库(同样移除文件)
1.预览将要删除的文件 *git rm -r -n --cached 文件/文件夹名称*

2.确定无误后删除文件 *git rm -r --cached 文件/文件夹名称*

3.提交到本地并推送到远程服务器 *git commit -m "提交说明";git push origin master*

git commit 后可能会报如下错误
> *** Please tell me who you are.

Run

  git config --global user.email "you@example.com"
  git config --global user.name "Your Name"

to set your account's default identity.
它提示你应先设置用户邮箱和用户名 设置一个全局的邮箱和用户名一步到位，以后不用再添加
```c
    dyp@dyp:~/Myzip$ git config --global user.email "1369020579@qq.com"
    dyp@dyp:~/Myzip$ git config --global user.user.name "dyp"
```
### 远程仓库(GIthub)修改之后，应先将本地仓库更新
否则会报如下错误
 ! [rejected]        master -> master (fetch first)

1.先pull远程仓库 *git pull origin*

2.然后和本地仓库合并 *git merge origin master*

3.最后可以将本地仓库更新内容提交给Github  *git push -u origin master* 


### github readme添加图片
  首先找到repo中图片，点击图片可获得路径，如：
    https://github.com/dengyapeng/Huffmanzip/blob/master/Image/decompress1.png
    需将blob改为raw。。。
    以Markdown语法如：
    ![](.../raw/...)方式添加
___
## UNP 笔记

[第三章 套接字编程介绍](Node/c3.md)

[第四章 基本TCP套接字编程](Node/c4.md)

[第六章 I/O复用 select和poll函数](Node/c6.md)

___
## Linux 常用命令
    .tar文件的解压 tar -zcvf etc.tar /root/myApp  将当前目录的etc.tar文件解压到/root/myApp路径下
    虚拟机 从本地下载文件命令#yum install lrzsz  
___
## 操作系统笔记
[第四章 进程及进程管理](Node/systemC4.md)

[第五章 资源分配与调度](Node/systemC5.md)

[第六章 主存管理](Node/systemC6.md)

[第七章 设备管理](Node/systemC7.md)

[第八章 文件系统](Node/systemC8.md)

[问答题梳理](Node/问答题梳理.md)


## 《后台开发：核心技术与应用实践》后台开发基础知识总结 可以作为复习提纲

___
### c++知识体系梳理
