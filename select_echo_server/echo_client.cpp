#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

#define BUF_SIZE 1024
void error_handling(const char *message);

int main(int argc, const char *argv[]) {
  if (argc != 3) {
    printf("Usage : %s <IP> <port> \n", argv[0]);
    exit(1);
  }
  char message[BUF_SIZE];
  int sock;
  sock = socket(PF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    error_handling("socket() error");
  }

  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
  serv_addr.sin_port = htons(atoi(argv[2]));

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
    error_handling("connect() error");
  } else {
    puts("Connected...");
  }
  int str_len = 0;
  while (1) {
    fputs("Input message(Q to quit) : ", stdout);
    fgets(message, BUF_SIZE, stdin);
    if (!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
      break;
    }

    str_len = write(sock, message, strlen(message));
    printf("write message length = %d\n", str_len);
    int read_len = 0;
    int remain = str_len;
    printf("remain = %d\n", remain);
    while (remain > 0 && (read_len = read(sock, message, BUF_SIZE))) {
      remain -= str_len;
      printf("remain = %d\n", remain);
    }
    message[str_len] = 0;
    printf("Message from server: %s", message);
  }
  close(sock);

  return 0;
}

void error_handling(const char *message) {
  fputs(message, stderr);
  fputc('\n', stderr);
  exit(1);
}