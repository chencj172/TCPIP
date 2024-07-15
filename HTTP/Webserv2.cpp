#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/stat.h>

const int BUF_SIZE = 1024;
const int SMALL_BUF = 100;

void* request_handler(void* arg);
void send_data(FILE* , const std::string& ct, const std::string& fileName);
std::string content_type(const std::string& file);
void send_error(FILE* fp);
void error_handling(std::string msg);

int main(int argc, const char* argv[])
{
    int serv_sock, clnt_sock;
    sockaddr_in serv_addr, clnt_addr;
    socklen_t clntLen = sizeof(clnt_addr);
    pthread_t t_id;

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family =  AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (sockaddr*)&serv_addr, sizeof(sockaddr)) == -1)
        error_handling("bind() error");
    listen(serv_sock, 7);

    while(1)
    {
        clnt_sock = accept(serv_sock, (sockaddr*)&clnt_addr, &clntLen);
        if(clnt_sock == -1)
            error_handling("accept() error");
        puts("connected...");
        pthread_create(&t_id, NULL, request_handler, &clnt_sock);
        pthread_detach(t_id);
    }
    close(serv_sock);
    return 0;
}

void* request_handler(void* arg)
{
    int cSock = *((int*)arg);
    char req_line[SMALL_BUF];
    FILE *clnt_read = fdopen(cSock, "r");
    FILE *clnt_write = fdopen(dup(cSock), "w");

    char method[10];
    char ct[15];
    char file_name[30];

    fgets(req_line, SMALL_BUF, clnt_read);
    if(strstr(req_line, "HTTP/") == NULL)
    {
        // 不是http请求
        send_error(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return NULL;
    }

    //从请求头中找出方法名和文件名
    strcpy(method, strtok(req_line, " /"));
    strcpy(file_name, strtok(NULL, " /"));
    if(strcmp("favicon.ico", file_name) == 0) return NULL;
    strcpy(ct, content_type(file_name).c_str());
    if(strcmp(method, "GET") != 0)
    {
        send_error(clnt_write);
        fclose(clnt_read);
        fclose(clnt_write);
        return NULL;
    }
    fclose(clnt_read);
    send_data(clnt_write, ct, file_name);
}

void send_data(FILE* fp,const std::string& ct, const std::string& fileName)
{
    // 状态行
    char protocol[]="HTTP/1.0 200 OK\r\n";
    // 消息头
	char server[]="Server:Linux Web Server \r\n";
	char cnt_type[SMALL_BUF];
    sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct.c_str());

    FILE* send_file = fopen(fileName.c_str(), "rb");
    if (send_file == NULL)
    {
        send_error(fp);
        return;
    }
    
    puts("start...");
    std::cout << "sending " << fileName << std::endl;
    // 传输头信息
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_type, fp);

    // 传输文件
    size_t read_len;
    char buf[BUF_SIZE];
    while((read_len = fread(buf, 1, BUF_SIZE, send_file)) > 0)
    {
        fwrite(buf, 1, read_len, fp);
        fflush(fp);
    }
    fclose(fp);
    fclose(send_file);
    puts("end...");
}

std::string content_type(const std::string& file)
{
    if((file.find("html") != -1) || (file.find("htm") != -1))
        return "text/html";
    else if((file.find("png") != -1))
        return "image/png";
    else if((file.find("jpg") != -1) || (file.find("jpeg") != -1))
        return "image/jpeg";
    else 
        return "text/plain";
}
void send_error(FILE* fp)
{
    char protocol[]="HTTP/1.0 400 Bad Request\r\n";
	char server[]="Server:Linux Web Server \r\n";
	char cnt_len[]="Content-length:2048\r\n";
	char cnt_type[]="Content-type:text/html\r\n\r\n";
	char content[]="<html><head><title>NETWORK</title></head>"
	       "<body><font size=+5><br>发生错误～"
		   "</font></body></html>";

	fputs(protocol, fp);
	fputs(server, fp);
	fputs(cnt_len, fp);
	fputs(cnt_type, fp);
	fflush(fp);
}
void error_handling(std::string msg)
{
    fputs(msg.c_str(), stderr);
    perror("ERROR");
    exit(-1);
}
