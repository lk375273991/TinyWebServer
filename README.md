# TinyWebServer

## server01.c 
server01.c 展示了一个最基本的服务器端编程的简单例子。用于向浏览器发送 “Hello World”

使用方法： 

	gcc server01.c
	./a.out

在浏览器输入：

	localhost:8080
	
即可看到 Hello World 

## server02.c

server02.c 展示了在动态访问页面的请求下服务器是如何处理的。
支持HTML和JPG图片的读取。

使用如下：

	gcc server02.c -o server
	./server

然后分别在浏览器下输入:

	localhost/one.html
	localhost/two.html

即可看到结果