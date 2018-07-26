#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>

#define SERV_TCP_PORT 20000
#define CRIT   40000 // 40ms (Maximum thinkng time in critical section)
#define BLOCKSIZE 100

void err_msg(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int port_no;
    int ipaddr;
    int sockfd;
    struct sockaddr_in serv_addr;
    int rflag, wflag, endflag;
    char sch;
    struct hostent *retrieve;
    int loopnum;
    int i;
    int num;
    char ident1[35];
    char ident2[35];
    char recvblock[BLOCKSIZE];
    char sendblock[BLOCKSIZE];

    if (argc < 2) {
        fprintf(stderr,"Usage: client serv_addr [port_no]\n");
        exit(1);
    }
    port_no = SERV_TCP_PORT;
    if ((retrieve = gethostbyname(argv[1])) == NULL) {
        printf("Unknown host name: %s\n", argv[1]);
        exit(1);
    }
    if (argc > 2) { strcpy(ident1, argv[2]); }
    loopnum = argc > 3 ? atoi(argv[3]) : BLOCKSIZE;
    ipaddr = *(unsigned int *)(retrieve->h_addr_list[0]);
    printf("%d.%d.%d.%d (%d) にアクセスします．Ctrl-A を押すと終了します．\n",
            ipaddr & 0xff, (ipaddr >> 8) & 0xff, (ipaddr >> 16) & 0xff, (ipaddr >> 24) & 0xff,
            port_no);
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = *(unsigned int *)(retrieve->h_addr_list[0]);
    serv_addr.sin_port = htons(port_no);

    for (i = 0; i < loopnum; i++){

        // 通信用のソケットの確保
        if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            err_msg("client: can't open datastream socket");
        }

        // 能動オープン
        if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            err_msg("client: can't connect server address");
        }

        fcntl(fileno(stdin), F_SETFL, O_NONBLOCK);
        fcntl(fileno(stdout), F_SETFL, O_NONBLOCK);
        fcntl(sockfd, F_SETFL, O_NONBLOCK);
        rflag = 1; wflag = 0;
        while(1) {
            if (rflag == 1) {
                if (read(fileno(stdin), &sch, 1) == 1) { rflag = 0; }
            }
            if (rflag == 0) {
                if (write(sockfd, &sch, 1) == 1) { rflag = 1; }
            }
            if (wflag == 0) {
                // read and prepare to send
                if (recv(sockfd, recvblock, BLOCKSIZE, 0) != -1) {
                    sscanf(recvblock, "%d %s %s", &num, ident1, ident2);
                    memset(&sendblock, 0x0, BLOCKSIZE);
                    sprintf(sendblock, "%d %s %s", num + 1, argv[2], ident1);
                    wflag = 1;
                }
            }
            if (wflag == 1) {
                // send
                fprintf(stderr, "%s\n", sendblock);
                if (send(sockfd, sendblock, BLOCKSIZE, 0) != -1) {
                    endflag = 1;
                }
            }
            if (endflag == 1) {
                endflag = 0;
                break;
            }
        }
        usleep(CRIT);
        close(sockfd);
    }
    write(fileno(stdout), "\nEnd.\n", strlen("\nEnd.\n")+1);
    return 0;
}
