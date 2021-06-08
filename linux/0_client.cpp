#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<assert.h>
#include<stdio.h>
#include<string.h>
#include<iostream>
#define BUF_SIZE 1024
using namespace std;
static bool stop = false;


int main(int argc, char *argv[]){

    if(argc <= 2){
        printf("usage : %s ip_address port_number backlog\n", basename(argv[0]));
        return 1;
    }

    const char *ip = argv[1];
    int port = atoi(argv[2]);
    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &server_address.sin_addr);
    server_address.sin_port = htons(port);
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    
    assert(sock >= 0);
    if(connect(sock, (struct sockaddr*)&server_address, sizeof(server_address)) < 0){
        printf("connection failed\n");
    }else{
        char remote[INET_ADDRSTRLEN];
        printf("client connected with ip : %s and port : %d.\n", 
            inet_ntop(AF_INET, &server_address.sin_addr, remote, INET_ADDRSTRLEN), ntohs(server_address.sin_port));
        

        const char* normal_data = "buaa1";
        const char* oob_data = "oob1";
        send(sock, normal_data, strlen(normal_data), 0);
        send(sock, oob_data, strlen(oob_data), MSG_OOB);
        normal_data = "buaa2";
        oob_data = "oob2";
        send(sock, normal_data, strlen(normal_data), 0);
        //sleep(2);
        //send(sock, oob_data, strlen(oob_data), MSG_OOB);
        /*
        char buffer[BUF_SIZE];
        memset(buffer, '\0', BUF_SIZE);
        int ret = recv(sock, buffer, BUF_SIZE - 1, 0);
        printf("get %d bytes info from server : %s\n", ret, buffer);
        */
    }

    close(sock);
    return 0;
}