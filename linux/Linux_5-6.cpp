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
        const char* oob_data = "buaa";
        const char* normal_data = "buaa123";
        send(sock, normal_data, strlen(normal_data), 0);
        //send(sock, oob_data, strlen(oob_data), MSG_OOB);
        send(sock, normal_data, strlen(normal_data), 0);
    }

    close(sock);
    return 0;
}