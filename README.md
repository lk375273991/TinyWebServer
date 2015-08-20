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

## sercer03.c
server03.c 增加了对PHP的支持，相应的需要了解CGI协议。
在Ubuntu下，使用如下命令安装PHP-FPM：

	sudo apt-get install php5-fpm
	
然后需要修改php5-fpm的配置文件,即/etc/php5/fpm/pool.d/www.conf文件,在第33行，将 

	listen	= /var/run/php5-fpm.sock 
	
改为

	listen = 127.0.0.1:9000
	
然后执行命令

	sudo php5-fpm
	
来运行php5-fpm。


在server03.c文件中需要将第292行中的

	strcpy(msg, "/home/2009/Desktop/");
	
改为所在的路径。

使用如下命令编译并执行后:

	gcc server03.c fastcgi.c -o server03
	./servero3
	
在浏览器中输入
	
	127.0.0.1:8080/test.php?name=TinyWebServer
	
即可看到结果。
