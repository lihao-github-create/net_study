#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
using namespace std;

const int BUF_SIZE = 100;
void error_handling(const char *buf);

int main(int argc, const char *argv[]) {
  if (argc != 2) {
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }

  int serv_sock = socket(PF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(sockaddr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));

  if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
    error_handling("bind() error");
  }

  if (listen(serv_sock, 5) == -1) {
    error_handling("listen() error");
  }

  fd_set reads, cpy_reads;
  FD_ZERO(&reads);
  FD_SET(serv_sock, &reads);
  int fd_max = serv_sock;

  struct timeval timeout;
  char buf[BUF_SIZE];
  while (1) {
    cpy_reads = reads;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    int fd_num = 0;
    if ((fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout)) == -1) {
      break;
    }
    if (fd_num == 0) {
      continue;
    }

    for (int i = 0; i < fd_max + 1; i++) {
      if (FD_ISSET(i, &cpy_reads)) {
        if (i == serv_sock) {
          struct sockaddr_in clnt_addr;
          socklen_t addr_sz = 0;
          int clnt_sock =
              accept(serv_sock, (struct sockaddr *)&clnt_addr, &addr_sz);
          FD_SET(clnt_sock, &reads);
          fd_max = max(fd_max, clnt_sock);
          printf("connected client : %d\n", clnt_sock);
        } else {  // read message
          auto str_len = read(i, buf, BUF_SIZE);
          if (str_len == 0) {
            FD_CLR(i, &reads);
            close(i);
            printf("close client : %d\n", i);
          } else {
            write(i, buf, str_len);  // echo
          }
        }
      }
    }
  }
  close(serv_sock);
  return 0;
}

void error_handling(const char *buf) {
  fputs(buf, stderr);
  fputc('\n', stderr);
  exit(1);
}