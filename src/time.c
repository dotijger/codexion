#include "codexion.h"
#include <stdint.h>
#include <sys/types.h>

uint64_t get_current_time_ms(void) {
  struct timeval tv;

  if (gettimeofday(&tv, NULL))
    return (0);
  return ((uint64_t)tv.tv_sec * 1000) + ((uint64_t)tv.tv_usec / 1000);
  // tv_sec is whole seconds since 1970, usec is MICROseconds in current second,
  // divide by 1000 to get leftover milliseconds to add
  // (basically tv_sec gives time, usec gives the fractional remainder of the
  // current second)
}
