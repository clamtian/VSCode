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
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>
#include<poll.h>
#define BUFFER_SIZE 1024

static int connfd;

void sig_urg(int sig){
    int save_errno = errno;
    char buffer[BUFFER_SIZE];
    memset(buffer, '\0', BUFFER_SIZE);
    int ret = recv(connfd, buffer, BUFFER_SIZE - 1, MSG_OOB);
    printf("got %d bytes of oob data '%s'\n", ret, buffer);
    errno = save_errno;
}
void addsig(int sig, void (*sig_handler)(int)){
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = sig_handler;
    sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);

}


int main(int argc, char *argv[]){

    if(argc <= 2){
        printf("usage : %s ip_address port_number filename\n", basename(argv[0]));
        return 1;
    }
    const char *ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);

    int ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(listenfd, 5);
    assert(ret != -1);

    struct sockaddr_in client_address;
    socklen_t client_addrlength = sizeof(client_address);
    connfd = accept(listenfd, (struct sockaddr*)&client_address, &client_addrlength);
    if(connfd < 0){
        printf("errno is : %d\n", errno);
    }else{
        addsig(SIGURG, sig_urg);
        fcntl(connfd, F_SETOWN, getpid());
        char buffer[BUFFER_SIZE];
        while(1){
            memset(buffer, '\0', BUFFER_SIZE);
            ret = recv(connfd, buffer, BUFFER_SIZE - 1, 0);
            if(ret <= 0){
                //break;
            }else{
                printf("get %d bytes of normal data : %s\n", ret, buffer);
            }
        }
        close(connfd);
    }
    close(listenfd);
    return 0;
}