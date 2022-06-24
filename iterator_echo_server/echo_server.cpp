#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define BUF_SIZE 1024
void error_handling(const char *message);

int main(int argc, const char *argv[]) {
  if (argc != 2) {
    printf("Usage : %s <port>\n", argv[0]);
    exit(1);
  }

  int serv_sock;
  char message[BUF_SIZE];
  serv_sock = socket(PF_INET, SOCK_STREAM, 0);
  if (serv_sock == -1) {
    error_handling("socket() error");
  }

  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));

  if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
    error_handling("bind() error");
  }

  if (listen(serv_sock, 5) == -1) {
    error_handling("listen() error");
  }

  struct sockaddr_in clnt_addr;
  socklen_t clnt_addr_sz;
  int count = 0;
  while (1) {
    int clnt_sock =
        accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_sz);
    if (clnt_sock == -1) {
      error_handling("accept() error");
    } else {
      printf("Connected client %d \n", count++);
    }
    int str_len = 0;
    while ((str_len = read(clnt_sock, message, BUF_SIZE)) != 0) {
      // 正常情况下，会一直服务到client首先关闭连接
      printf("length of message = %d\n", str_len);
      write(clnt_sock, message, str_len);
    }
    close(clnt_sock);
  }
  close(serv_sock);
  return 0;
}

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}