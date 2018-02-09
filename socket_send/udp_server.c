
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#ifdef WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#endif


// struct sockaddr_in, struct in_addr https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html

// wireshark 抓包疑惑:
//  linux 发送到 windows 发送成功的那个包大小是 SO_MAX_MSG_SIZE=65507
//  再大了就不成功了 
//  wireshark 看到分片过程 是 ipv4 协议，不是 udp， 并且分片有丢包


#define EXPECT(expr) \
    do { \
    if(!(expr)) \
        { \
        fprintf(stderr, "unexpect %s  (%s:%d)\n",#expr, __FILE__, __LINE__); \
        fflush(stderr);\
        } \
    } while (0)


// from https://stackoverflow.com/questions/40159892/using-asprintf-on-windows

#ifndef _vscprintf
/* For some reason, MSVC fails to honour this #ifndef. */
/* Hence function renamed to _vscprintf_so(). */
int _vscprintf_so(const char * format, va_list pargs) {
    int retval;
    va_list argcopy;
    va_copy(argcopy, pargs);
    retval = vsnprintf(NULL, 0, format, argcopy);
    va_end(argcopy);
    return retval;
}
#endif // _vscprintf

#ifndef vasprintf
int vasprintf(char **strp, const char *fmt, va_list ap) {
    int len = _vscprintf_so(fmt, ap);
    if (len == -1) return -1;
    char *str = malloc((size_t)len + 1);
    if (!str) return -1;
    int r = vsnprintf(str, len + 1, fmt, ap); /* "secure" version of vsprintf */
    if (r == -1) return free(str), -1;
    *strp = str;
    return r;
}
#endif // vasprintf

#ifndef asprintf
int asprintf(char *strp[], const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int r = vasprintf(strp, fmt, ap);
    va_end(ap);
    return r;
}
#endif // asprintf

int  make_large_data(char ** p, int size)
{
    //return asprintf(p, "very long string %.*d",size, 0);
    char * pv = malloc(size+1);
    memset(pv, '1', size);
    pv[size] = 0;
    *p = pv;
    return size;
}

void getsockopt_sizes(
#ifdef WIN32
    SOCKET sock
#else
    int sock
#endif
    , int * rcvsize
    , int * sndsize
)
{
    int optval;
    int optval_size;
    int rc;

    

#ifdef WIN32
    optval = 0; optval_size = sizeof(optval);
    rc = getsockopt(sock, SOL_SOCKET, SO_MAX_MSG_SIZE, (void*)&optval, &optval_size);
    EXPECT(rc >= 0 && optval_size == sizeof(optval));
    printf("[+] getsockopt SO_MAX_MSG_SIZE=%d\n", optval);
#endif

    optval = 0; optval_size = sizeof(optval);
    rc = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&optval, &optval_size);
    EXPECT(rc >= 0 && optval_size == sizeof(optval));
    printf("[+] getsockopt SO_SNDBUF=%d\n", optval);
    *sndsize = optval;

    optval = 0; optval_size = sizeof(optval);
    rc = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&optval, &optval_size);
    EXPECT(rc >= 0 && optval_size == sizeof(optval));
    printf("[+] getsockopt SO_RCVBUF=%d\n", optval);
    *rcvsize = optval;
}

void server()
{
    struct sockaddr_in saddr;
    int rc;
#ifdef WIN32
    SOCKET sock;
#else
    int sock;
#endif
    int sndsize;
    int rcvsize;
    int size;

    enum{_recv_size=0x200,};
    char recv_buf[_recv_size];
    int daddr_len = (int)sizeof(struct sockaddr_in);
    struct sockaddr_in daddr;
    //struct in_addr ip_address;

#ifdef WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed. Error Code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
#endif

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET; 

    //inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);
    saddr.sin_addr.s_addr = 0; // 写 127.0.0.1 其他机器就无法连接到
    saddr.sin_port = htons(3333);
    

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    EXPECT(sock >= 0);


    rc = bind(sock, (struct sockaddr*)&saddr, sizeof(saddr));

    EXPECT(rc >= 0);

    getsockopt_sizes(sock, &rcvsize, & sndsize);


    struct sockaddr * daddr_ptr = (struct sockaddr *)&daddr;
    char * msg_send;
    for (;;)
    {
        recv_buf[0] = 0;
        memset(&daddr, 0, sizeof(daddr));
        printf("[+] recvfrom ing\n");
        rc = recvfrom(sock, recv_buf, _recv_size, 0, daddr_ptr, &daddr_len);

        if (rc > 0) {
            printf("[+] recv (%d)%.*s\n", rc, rc, recv_buf);
            
            //rc = sendto(sock, "received", 8, 0, daddr_ptr, daddr_len);
            //EXPECT(rc == 8);
            size = sndsize;
            for (;;)
            {
                msg_send = 0;
                rc = make_large_data(&msg_send, size);

                rc = sendto(sock, msg_send, strlen(msg_send), 0, daddr_ptr, daddr_len);
                if (rc >= 0) {
                    break;
                }
                printf("[!] error=%d %s\n", errno, strerror(errno));
                free(msg_send);
                size -= 1;
            }
            
            if (rc < 0) {
                printf("[!] error=%d %s\n", errno, strerror(errno));
            }
            printf("[+] sendto %d/%d\n", rc, strlen(msg_send));
            free(msg_send);
        }


    }

}


int main()
{
    server();
    return 0;
}


/*
测试结果：
没有找到 sendto 返回值比要求发送大小小的情景， 发送不成功就是返回 -1， 再就是发送成功，
返回大小是要求发送的大小
*/
