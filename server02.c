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

void requestHandling(void *sock);                  //浏览器请求处理
void sendData(void *sock,char * filename);         //向浏览器发送请求文件的内容
void catHtml (void *sock,char * filename);         //读取HTML内容
void catJpeg(void *sock,char *filename);           //读取图像文件内容
void sendError(void *sock);                        //错误处理
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
    
    while(1)
    {

        //接受服务器的请求,accept调用成功返回一个新的套接字描述符，这个描述符代表了与远程客户端的连接
        client_addr_size = sizeof(client_addr);
        client_sock      = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_size);
        if (-1==client_sock)
        {
            errorHandling("accept error!");
        }
        requestHandling((void *)&client_sock);
    }
    
    //关闭套接字
    //close(client_sock);
    close(server_sock);
    
    return 0;
}
void requestHandling(void * sock)
{
    int client_sock= *((int *) sock);
    char buf[1024];                                         //缓冲区
    char method[10];                                        //保存请求方法，GET 、POST
    char filename[30];                                      //保存请求的文件名

    read(client_sock,buf,(sizeof buf) -1);                  //读取浏览器的请求内容

    //检查协议是否正确
    if(NULL==strstr(buf,"HTTP/"))
    {
        sendError(sock);
        close(client_sock);
        return ;
    }

    strcpy(method,strtok(buf," /"));                        //提取请求方法(GET)到method数组中
    strcpy(filename,strtok(NULL," /"));                     //提取文件名到filename数组中

    //判断请求方法是否是GET，不是GET就进行请求错误处理
    if(0!=strcmp(method,"GET"))
    {
        sendError(sock);
        close(client_sock);
        return ;
    }

    //访问请求文件
    sendData(sock,filename);
}

/**
 * 处理浏览器请求的文件
 * 参数:客户端套接字地址，void * sock
        请求文件名      char * filename
 */
void sendData(void *sock,char *filename)
{
    int client_sock=*((int *)sock);
    char buf[20];
    char ext[10];

    strcpy(buf,filename);

    //判断文件类型
    strtok(buf,".");
    strcpy(ext,strtok(NULL,"."));
    if(0==strcmp(ext,"php"))
    {
        //留待处理
    }else if (0==strcmp(ext,"html"))
    {
        catHtml(sock,filename);
    }else if(0==strcmp(ext,"jpg"))
    {
        catJpeg(sock,filename);
    }else
    {
        sendError(sock);
        close(client_sock);
        return;
    }
}

/**
 * 读取HTML文件内容
 * 参数： 客户端套接字地址 void *sock
 *        文件名          char* filename
 */
void catHtml(void * sock,char *filename)
{
    int client_sock=*((int *)sock);
    char buf[1024];
    FILE *fp;

    char status[] = "HTTP/1.0 200 OK\r\n";
    char header[] = "Server:A Tiny Web Server\r\nContent-Type: text/html\r\n\r\n";

    write(client_sock,status,sizeof status);              //发送响应报文状态行
    write(client_sock,header,sizeof header);              //发送响应报文消息头

    fp=fopen(filename,"r");
    if(NULL==fp)
    {
        sendError(sock);
        close(client_sock);
        errorHandling("cannnot open file ");
        return ;
    }

    //读取文件内容并发送
    fgets(buf,sizeof buf,fp);
    while(!feof(fp))
    {
        write(client_sock,buf,strlen(buf));
        fgets(buf,sizeof buf,fp);
    }

    fclose(fp);
    close(client_sock);
}

void catJpeg(void *sock,char *filename)
{
    int client_sock = *((int *)sock);
    char buf[1025];
    FILE *fp;
    FILE *fw;

    char status[] = "HTTP/1.0 200 OK\r\n";
    char header[] = "Server:A Tiny Simple Web Server\r\nContent-Type: image/jpeg\r\n\r\n";

    write(client_sock,status,strlen(status));            //发送响应报文状态行
    write(client_sock,header,strlen(header));            //发送响应报文消息头

    //图片以二进制格式打开
    fp=fopen(filename,"rb");
    if(NULL == fp)
    {
        sendError(sock);
        close(client_sock);
        errorHandling("open file failed");
        return ;
    }

    //在套接字上打开一个文件句柄.
    //fdopen取一个现存的文件描述符，并使一个标准的I / O流与该描述符相结合。此函数常用于由创建管道和网络通信通道函数获得的描述符。
    // 因为这些特殊类型的文件不能用标准I/O fopen函数打开，首先必须先调用设备专用函数以获得一个文件描述符，然后用fdopen使一个标准I / O流
    // 与该描述符相结合。
    fw = fdopen (client_sock,"w");
    fread(buf,1,sizeof buf,fp);
    while(!feof(fp))
    {
        fwrite(buf,1,sizeof buf,fw);
        fread(buf,1,sizeof buf,fp);
    }

    fclose(fw);
    fclose(fp);
    close(client_sock);
}


void sendError(void *sock)
{
    int client_sock= *((int *)sock);

    char status[] = "HTTP/1.0 400 Bad Request\r\n";
    char header[] = "Server:A Tiny web Server\r\nContent-Type: text/html\r\n\r\n";
    char body  [] = "<html><head><title>Bad Request</title></head><body><p>请求出差</p></body></html>";

    //向客户端套接字发送信息
    write(client_sock,status,sizeof status);
    write(client_sock,header,sizeof header);
    write(client_sock,body  ,sizeof  body);
}






