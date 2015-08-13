//
//  main.cpp
//  webserver
//
//  Created by lk375273991 on 15/8/12.
//  Copyright (c) 2015年 2009. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080         //服务器监听端口

void errorHandling( char * str)      //错误处理
{
    fputs(str, stderr);
    fputc('\n',stderr);
    exit(1);
}
int main(int argc, const char * argv[]) {
    // insert code here...
    
    int server_sock;           //服务器套接字描述符
    int client_sock;           //客户端套接字描述符
    
    char buf[1025];            //缓冲区
    
    struct sockaddr_in server_addr;          //服务器套接字地址信息
    struct sockaddr_in client_addr;          //客户端套接字地址信息
    socklen_t client_addr_size;              //客户端套接字地址信息变量的大小
    
    //发送给客户端的固定内容
    char status[]="HTTP/1.0 200 OK\r\n";
    char header[]="Server: A Tiny Webserver\r\nContent-Type: text/html\r\n\r\n";
    char body  []="<html><head><title>A Tiny Webserver</title></head><body><h2>Hello World</h2></body></html>";
    //char status[] = "HTTP/1.0 200 OK\r\n";
    //char header[] = "Server: A Simple Web Server\r\nContent-Type: text/html\r\n\r\n";
    //char body[] = "<html><head><title>A Simple Web Server</title></head><body><h2>Welcome!</h2><p>This is github!</p></body></html>";

    
    //创建服务器套接字描述符
    server_sock=socket(PF_INET, SOCK_STREAM, 0);
    if (-1==server_sock)
    {
        errorHandling("bulid socket errer");
    }
    
    //分配套接字IP和端口信息
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port        = htons(PORT);
    
    //绑定服务器套接字
    if (-1==bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)))
    {
        errorHandling("bind error!");
    }
    
    //监听服务器套接字
    if (-1==listen(server_sock, 10))
    {
        errorHandling("listern error!");
    }
    
    //接受服务器的请求,accept调用成功返回一个新的套接字描述符，这个描述符代表了与远程客户端的连接
    client_addr_size = sizeof(client_addr);
    client_sock      = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size);
    if (-1==client_sock)
    {
        errorHandling("accept error!");
    }
    
    //读取客户端请求
    read(client_sock, buf, sizeof(buf)-1);
    printf("%s",buf);
    
    //向客户端套接字发送信息
    write(client_sock, status,sizeof(status));
    write(client_sock, header, sizeof(header));
    write(client_sock, body, sizeof(body));
    
    //关闭套接字
    close(client_sock);
    close(server_sock);
    
    return 0;
}
