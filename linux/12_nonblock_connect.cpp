#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/stat.h>
#include<sys/uio.h>
#include<sys/sendfile.h>
#include<sys/types.h>
#include<sys/epoll.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>
#include<pthread.h>
#define BUFFER_SIZE 1024
#define MAX_EVENT_NUMBER 1024

int setnonblocking(int fd){
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}
int unblock_connect(const char *ip1, const char *ip2, int port1, int port2,  int time){
    int ret1 = 0, ret2 = 0;
    struct sockaddr_in address1, address2;
    bzero(&address1, sizeof(address1));
    address1.sin_family = AF_INET;
    inet_pton(AF_INET, ip1, &address1.sin_addr);
    address1.sin_port = htons(port1);
    int sockfd1 = socket(PF_INET, SOCK_STREAM, 0);
    int fdopt1 = setnonblocking(sockfd1);
    ret1 = connect(sockfd1, (struct sockaddr*)&address1, sizeof(address1));

    bzero(&address2, sizeof(address2));
    address2.sin_family = AF_INET;
    inet_pton(AF_INET, ip2, &address2.sin_addr);
    address2.sin_port = htons(port2);
    int sockfd2 = socket(PF_INET, SOCK_STREAM, 0);
    int fdopt2 = setnonblocking(sockfd2);
    ret2 = connect(sockfd2, (struct sockaddr*)&address2, sizeof(address2));

    if(ret1 == 0){
        printf("connect with server immediately\n");
        fcntl(sockfd1, F_SETFL, fdopt1);
        return sockfd1;
    }else if(errno != EINPROGRESS){
        printf("unblock connect not support\n");
        return -1;
    }
    fd_set readfds;
    fd_set writefds;
    struct timeval timeout;
    FD_ZERO(&readfds);
    FD_SET(sockfd1, &writefds);
    FD_SET(sockfd2, &writefds);
    timeout.tv_sec = time;
    timeout.tv_usec = 0;
    int ret = select(sockfd2 + 1, NULL, &writefds, NULL, &timeout);
    if(ret <= 0){
        printf("connection time out\n");
        close(sockfd1);
        return -1;
    }
    if(!FD_ISSET(sockfd1, &writefds)){
        printf("no events on sockfd found\n");
        close(sockfd1);
        return -1;
    }
    if(!FD_ISSET(sockfd2, &writefds)){
        printf("no events on sockfd found\n");
        close(sockfd2);
        return -1;
    }
    int error = 0;
    socklen_t length = sizeof(error);

    if(getsockopt(sockfd1, SOL_SOCKET, SO_ERROR, &error, &length) < 0){
        printf("get socket option failed\n");
        close(sockfd1);
        return -1;
    }
    if(error != 0){
        printf("connection failed after select with the error : %d\n", error);
        close(sockfd1);
        return -1;
    }
    if(getsockopt(sockfd2, SOL_SOCKET, SO_ERROR, &error, &length) < 0){
        printf("get socket option failed\n");
        close(sockfd2);
        return -1;
    }
    if(error != 0){
        printf("connection failed after select with the error : %d\n", error);
        close(sockfd2);
        return -1;
    }
    printf("connection ready after select with the socket : %d\n", sockfd1);
    fcntl(sockfd1, F_SETFL, fdopt1);
    printf("connection ready after select with the socket : %d\n", sockfd2);
    fcntl(sockfd2, F_SETFL, fdopt2);
    return sockfd1;
}

int main(int argc, char *argv[]){

    if(argc <= 2){
        printf("usage : %s ip_address port_number filename\n", basename(argv[0]));
        return 1;
    }
    const char *ip1 = argv[1], *ip2 = argv[3];
    int port1 = atoi(argv[2]), port2 = atoi(argv[4]);

    int sockfd = unblock_connect(ip1, ip2, port1, port2, 10);
    if(sockfd < 0){
        return 1;
    }
    close(sockfd);
    return 0;
    
}