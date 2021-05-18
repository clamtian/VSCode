#include <string.h>
#include <vector>
#include <string>
#include <unistd.h>
#include <list>
#include <map> 
#include <vector> 
#include <algorithm>
#include <set>
#include <iostream>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <queue>
#include <sstream> 
#include <errno.h>
#include <sys/types.h>    
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;


#define N 50
#define MAXBUF 100
#define MAXLINE 100
#define RIO_BUFSIZE     4096


typedef struct
{
    int rio_fd;      //与缓冲区绑定的文件描述符的编号
    int rio_cnt;        //缓冲区中还未读取的字节数
    char *rio_bufptr;   //当前下一个未读取字符的地址
    char rio_buf[RIO_BUFSIZE];
}rio_t;

void rio_readinitb(rio_t *rp, int fd);
ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n);
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
ssize_t rio_writen(int fd, void *usrbuf, size_t n);



int main(){
    int fd1, fd2;
    char c;
    char buf[MAXBUF] = {};
    //fd1 = open("../inputTest.txt", O_RDONLY, 0);
    fd2 = open("../inputTest.txt", O_RDWR|O_APPEND, 0);
    //read(fd2, &c, 1);
    dup2(fd2, STDOUT_FILENO);

    int len = read(STDIN_FILENO, buf, MAXBUF);
    write(STDOUT_FILENO, buf, len);
    return 1;

}


int main6(){
    int n = 0;
    rio_t rio;
    char buf[MAXLINE];

    rio_readinitb(&rio, STDIN_FILENO);
    while((n = rio_readlineb(&rio, buf, MAXLINE)) != 0){
        rio_writen(STDOUT_FILENO, buf, n);
    }
    return 1;
}

void rio_readinitb(rio_t *rp, int fd)
/**
 * @brief rio_readinitb     rio_t 结构体初始化,并绑定文件描述符与缓冲区
 *
 * @param rp                rio_t结构体
 * @param fd                文件描述符
 */
{
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;

    return;
}

ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
/**
 * @brief rio_read  RIO--Robust I/O包 底层读取函数。当缓冲区数据充足时，此函数直接拷贝缓
 *                  冲区的数据给上层读取函数；当缓冲区不足时，该函数通过系统调用
 *                  从文件中读取最大数量的字节到缓冲区，再拷贝缓冲区数据给上层函数
 *
 * @param rp        rio_t，里面包含了文件描述符和其对应的缓冲区数据
 * @param usrbuf    读取的目的地址
 * @param n         读取的字节数量
 *
 * @returns         返回真正读取到的字节数（<=n）
 */
{
    int cnt;

    while(rp->rio_cnt <= 0)     
    {
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
        if(rp->rio_cnt < 0)
        {
            if(errno != EINTR)  //遇到中断类型错误的话应该进行读取，否则就返回错误
                return -1;
        }
        else if(rp->rio_cnt == 0)   //读取到了EOF
            return 0;
        else
            rp->rio_bufptr = rp->rio_buf;       //重置bufptr指针，令其指向第一个未读取字节，然后便退出循环
    }

    cnt = n;
    if((size_t)rp->rio_cnt < n)     
        cnt = rp->rio_cnt;
    memcpy(usrbuf, rp->rio_bufptr, n);
    rp->rio_bufptr += cnt;      //读取后需要更新指针
    rp->rio_cnt -= cnt;         //未读取字节也会减少

    return cnt;
}

ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n)
/**
 * @brief rio_readnb    供用户使用的读取函数。从缓冲区中读取最大maxlen字节数据
 *
 * @param rp            rio_t，文件描述符与其对应的缓冲区
 * @param usrbuf        void *, 目的地址
 * @param n             size_t, 用户想要读取的字节数量
 *
 * @returns             真正读取到的字节数。读到EOF返回0,读取失败返回-1。
 */
{
    size_t leftcnt = n;
    ssize_t nread;
    char *buf = (char *)usrbuf;

    while(leftcnt > 0)
    {
        if((nread = rio_read(rp, buf, n)) < 0)
        {
            if(errno == EINTR)      //其实这里可以不用判断EINTR,rio_read()中已经对其处理了
                nread = 0;
            else 
                return -1;
        }
        leftcnt -= nread;
        buf += nread;
    }

    return n-leftcnt;
}

ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen)
/**
 * @brief rio_readlineb 读取一行的数据，遇到'\n'结尾代表一行
 *
 * @param rp            rio_t包
 * @param usrbuf        用户地址，即目的地址
 * @param maxlen        size_t, 一行最大的长度。若一行数据超过最大长度，则以'\0'截断
 *
 * @returns             真正读取到的字符数量
 */
{
    size_t n;
    int rd;
    char c, *bufp = (char *)usrbuf;

    for(n = 1; n < maxlen; n++)     //n代表已接收字符的数量
    {
        if((rd = rio_read(rp, &c, 1)) == 1)
        {
            *bufp++ = c;
            if(c == '\n')
                break;
        }
        else if(rd == 0)        //没有接收到数据
        {
            if(n == 1)          //如果第一次循环就没接收到数据，则代表无数据可接收
                return 0;
            else
                break;
        }
        else                    
            return -1;
    }
    *bufp = 0;

    return n;
}

ssize_t rio_writen(int fd, void *usrbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = (char *)usrbuf;

    while(nleft > 0)
    {
        if((nwritten = write(fd, bufp, nleft)) <= 0)
        {
            if(errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        bufp += nwritten;
        nleft -= nwritten;
    }

    return n;
}

void handler1(int sig){
    int olderrno = errno;

    int pid = 0;
    while((pid = waitpid(-1, NULL, 0)) > 0){
        cout << "Handler reaped child " << pid << endl;
    }
    
    
    cout << pid << " sleep... " << endl;

    sleep(1);

    cout << pid << " end sleep" << endl;
    errno = olderrno;
}
int main5(){
    char buf[MAXBUF];

    if(signal(SIGCHLD, handler1) == SIG_ERR){
        cout << "signal error" << endl;
    }

    for(int i = 0; i < 3; ++i){
        if(fork() == 0){
            cout << "Hello from child " << getpid() << endl;
            exit(0);
        }
    }
    int n = read(STDIN_FILENO, buf, sizeof(buf));
    if(n < 0){
        cout << "read error" << endl;
    }
    cout << "parent procesing" << endl;
    while(1);
    exit(0);
}











void sigint_handler(int sig){
    cout << "Caught SIGINT " << sig << endl;
    //exit(0); 
}
int main4(){
    
    pid_t pid = 0;

    if((pid = fork()) == 0){
        if(signal(SIGCONT, sigint_handler) == SIG_ERR){
            cout << "signal error" << endl;
        }
        cout << "pause" << endl;
        pause();
        cout << "control should never reach here!" << endl;
        exit(0);
    }

    sleep(2);
    kill(pid, SIGCONT);
    exit(0);

}
int main3(){
    pid_t pid = 0;

    if((pid = fork()) == 0){
        cout << "pause" << endl;
        pause();
        cout << "control should never reach here!" << endl;
        exit(0);
    }

    sleep(2);
    kill(pid, SIGCONT);
    sleep(2);
    exit(0);
}
int main2(){
    int status = 0;
    pid_t pid = 0;

    cout << "Hello" << endl;

    pid = fork();

    cout << !pid << endl;

    if(pid){
        if(waitpid(-1, &status, 0) > 0){
            if(WIFEXITED(status)){
                cout << WEXITSTATUS(status) << endl;
            }
        }
    }
    cout << "Bye" << endl;

    exit(2);
}
int main1()
{
    int status, i;
    pid_t pid;

    for(int i = 0; i < N; ++i){
        if((pid = fork()) == 0){
            exit( 100 + i );
        }
    }

    while((pid = waitpid(-1, &status, 0)) > 0){
        if(WIFEXITED(status)){
            cout << "child " << pid << " terminated normally with exit status = " << WEXITSTATUS(status) << endl; 
        }else{
            cout << "child " << pid << " terminated abnormally" << endl;
        }
    }

    if(errno != ECHILD){
        cout << "waitpid error" << endl;
    }

    exit(0);
    
    return 1;
}