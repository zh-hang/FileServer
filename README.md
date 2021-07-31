# FTPServer

这是一个文件服务器。使用C++ ThreadPool实现。

## 需求

### 服务端

- 支持客户端请求查询文件（文件名称，文件大小，更改日期），上传文件，下载文件，删除文件
- 支持多客户端并发向服务请求操作、
- 信令走TCP，数据传输支持TCP/UDP两种方式可选。 

### 客户端

- 查询文件列表
- 文件上传
- 文件下载
- 文件删除

### 待选需求

- 局域网内服务器发现（组播方式）
- 上传，下载支持断点上传和下载
- 支持用户安全机制：用户登录鉴权，权限控制。
- 数据传输走安全传输 SSL

## 对象模型

![](images/UML.png)

## 迭代1

### 模型

![](images/迭代一model.png)

### 需求

- 服务器和客户端的连接
- 指定文件的传输

## 迭代2

### 模型

![](images/迭代二model.png)

### 需求

- 利用TCP传输信令
- 传输文件列表
- 选择文件UDP传输

## 迭代3

### 模型

![](images/迭代三model.png)

### 需求

- 利用select实现并发文件下载
- 使用threadpool实现并发

## 迭代四

### 模型

系统模型和迭代三相同，这里主要讨论几个模块的数据流图。

#### 用户登录

![](images/login.png)

#### 文件下载

![](images/download.png)

#### 文件上传

![](images/upload.png)

### 需求

- 实现用户验证
- 实现文件上传

## 参考文档

- [C++11线程池实现](https://www.cnblogs.com/lzpong/p/6397997.html)
- [根据linux内核源码查找recv返回EBADF(errno 9)的原因](https://www.cnblogs.com/james1207/p/3400192.html)

