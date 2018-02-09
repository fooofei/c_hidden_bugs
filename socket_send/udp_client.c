
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifdef WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#endif

#define EXPECT(expr) \
    do { \
    if(!(expr)) \
        { \
        fprintf(stderr, "unexpect %s  (%s:%d)\n",#expr, __FILE__, __LINE__); \
        fflush(stderr);\
        } \
    } while (0)


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


void client()
{
    int rc;
#ifdef WIN32
    SOCKET sock;
#else
    int sock;
#endif

    enum { _recv_size = 0x200, };
    char * recv_buf;
    int daddr_len = (int)sizeof(struct sockaddr_in);
    struct sockaddr_in daddr;
    //struct in_addr ip_address;

    int sndsize;
    int rcvsize;
    int size;
#ifdef WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed. Error Code : %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }
#endif

    memset(&daddr, 0, sizeof(daddr));
    daddr.sin_family = AF_INET;

    // inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);
    inet_pton(AF_INET, "100.107.70.104", &daddr.sin_addr);
    
    daddr.sin_port = htons(3333);


    sock = socket(AF_INET, SOCK_DGRAM, 0);
    EXPECT(sock >= 0);

    
    sndsize = 0; rcvsize = 0;
    getsockopt_sizes(sock, &rcvsize, &sndsize);

    struct sockaddr * daddr_ptr = (struct sockaddr *)&daddr;


    rc = sendto(sock, "he", 2, 0, daddr_ptr, daddr_len);
    EXPECT(rc == 2);

    size = 212992 * 2;
    recv_buf = malloc(size);
    rc = recvfrom(sock, recv_buf, size, 0, daddr_ptr, &daddr_len);

    EXPECT(rc > 0);

    free(recv_buf);

}


int main()
{
    client();
    return 0;
}
