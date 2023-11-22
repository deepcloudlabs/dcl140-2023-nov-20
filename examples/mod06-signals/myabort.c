#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

  return kill(getpid(), SIGABRT);
}
