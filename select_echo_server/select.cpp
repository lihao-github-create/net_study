#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>

#include <cstdio>
const int BUF_SIZE = 30;
using namespace std;

int main(int argc, const char *argv[]) {
  fd_set reads, temps;
  FD_ZERO(&reads);
  FD_SET(0, &reads);

  struct timeval timeout;

  char buf[BUF_SIZE];
  while (1) {
    temps = reads;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
    auto result = select(1, &temps, 0, 0, &timeout);
    if (result == -1) {
      puts("select() error");
      break;
    } else if (result == 0) {
      puts("time-out!");
    } else {
      if (FD_ISSET(0, &temps)) {
        auto str_len = read(0, buf, BUF_SIZE);
        buf[str_len] = 0;
        printf("message from console : %s", buf);
      }
    }
  }
  return 0;
}