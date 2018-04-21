#include <sys/time.h>

struct keyblock {
  int keyCode;
  int delay;
  struct timeval start;
  struct timeval oldStart;
};
