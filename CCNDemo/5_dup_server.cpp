#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<assert.h>
#include<stdio.h>
#include<netdb.h>
#include<string.h>
#include<iostream>
#define BUF_SIZE 1024
using namespace std;
static bool stop = false;


int main(int argc, char *argv[]){

    const char *ip = argv[1];
    int port = atoi(argv[2]);
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    int ret = bind(sock, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(sock, 5);
    assert(ret != -1);
    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);

    int connfd = accept(sock, (struct sockaddr*)&client, &client_addrlength);

    if(connfd < 0){
        printf("errno is : %d\n", errno);
    }else{
        close(STDOUT_FILENO);
        dup(connfd);
        //char buffer[BUF_SIZE];
        //memset(buffer, 'a', BUF_SIZE);
        //send(connfd, buffer, BUF_SIZE, 0);
        printf("buaa666\n");
        close(connfd);
    }
    close(sock);



    return 0;
}