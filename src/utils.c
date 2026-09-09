#include "codexion.h"

long get_time(t_time_format time_format)
{
	struct timeval	tv;

  if (gettimeofday(&tv, NULL))
    return (0);
  if (time_format == SECONDS)
	  return ((uint64_t)tv.tv_sec) + ((uint64_t)tv.tv_usec / 1000000);
  elif (time_format == MILLISECONDS)
	  return ((long)tv.tv_sec * 1000) + ((long)tv.tv_usec / 1000);
  elif (time_format == MICROSECONDS)
	  return ((long)tv.tv_sec * 1000000) + ((long)tv.tv_usec);
  // tv_sec is whole seconds since 1970, usec is MICROseconds in current second,
  // divide by 1000 to get leftover milliseconds to add
  // (basically tv_sec gives time, usec gives the fractional remainder of the
  // current second)
}
