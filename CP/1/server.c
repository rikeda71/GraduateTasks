#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/wait.h>
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
    int sockid, sockfp;
    int fp;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    char inbuf[BLOCKSIZE], outbuf[BLOCKSIZE];
    int pid;
    int chnum = 0; // number of child processes
    int status;
    int rdnum;

    // 引数にファイルを設定しなければエラー
    if (argc < 2) {
        err_msg("set a file of storing as an argument");
    }
    // ポート番号の指定があればそれを利用
    port_no = argc > 2 ? atoi(argv[2]) : SERV_TCP_PORT;

    // コネクション要求受付用のソケットの確保
    if ((sockid = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        // できなければ
        err_msg("server: can't open datastream socket");
    }

    /* 空状態のファイルを作成 */
    fp = fopen(argv[1], "w");
    close(fp);

    /* 適当なポートでコネクション待受状態になる */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port      = htons(port_no);

    // ソケットとポートの対応付け
    if (bind(sockid, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        err_msg("server: can't bind local address");
    }

    // 受動オープン状態に移行
    if (listen(sockid, 5) == -1) {
        err_msg("server: listen failed");
    }

    /* 繰り返す */
    while (1) {
        if (chnum) {
            /* for terminating child process */
            while (chnum && ((pid = waitpid(-1, &status, WNOHANG)) > 0)) {
                fprintf(stderr, "Terminate child process: %d\n", pid);
                chnum--;
            }
        }
        /* クライアントからコネクション要求を受け入れる */
        if ((sockfp = accept(sockid, (struct sockaddr *)&cli_addr, &cli_len)) < 0) {
            close(sockid);
            fprintf(stderr,"server: can't accept");
            break;
        }
        /* forkする */
        pid = fork();
        // fork error
        if (pid < 0) {
            close(sockfp);
            close(sockid);
            break;
        }
        // parent process
        else if (pid > 0) {
            close(sockfp);
            chnum++;
            continue;
        }

        // child process
        pid = getpid();
        fprintf(stderr, "\nI am child process %d\n",pid);
        close(sockid);
        while (1) {
            // file open
            if ((fp = open(argv[1], O_RDWR | O_CREAT, 0644)) == -1){
                perror(argv[1]);
                exit(1);
            }
#ifdef WITH_FLOCK
            if (flock(fp, LOCK_EX) == -1){
                perror(argv[1]);
                close(fp);
                exit(1);
            }
#endif
            // read file content
            lseek(fp, -(BLOCKSIZE+1), SEEK_END);
            rdnum = read(fp, inbuf, BLOCKSIZE);
            // make output
            memset(&outbuf, 0x0, BLOCKSIZE);
            if (rdnum == 0) { sprintf(outbuf, "0 None None"); }
            else { strcpy(outbuf, inbuf); }
            fprintf(stderr, "%s\n", outbuf);
            // send and recv
            send(sockfp, outbuf, BLOCKSIZE, 0);
            rdnum = recv(sockfp, inbuf, BLOCKSIZE, 0);
            // write to file
            if (rdnum == BLOCKSIZE) {
                lseek(fp, 0, SEEK_END);
                write(fp, inbuf, BLOCKSIZE);
                write(fp, "\n", 1);
            }
            else { break; }
#ifdef WITH_FLOCK
            flock(fp, LOCK_UN);
#endif
            close(fp);
            close(sockfp);
        }
    }

    return 0;
}
