/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   utils.c                                         \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/09 14:55:09 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/09 16:11:37 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
 * Error exit function (that will print error message and then exit)
*/

void	error_exit(char *exit_msg)
{
	fprintf(stderr, RED "%s" RESET, exit_msg);
	exit(EXIT_FAILURE);
}

/*
 * Time getter: get_time (returns time in seconds, milliseconds or microseconds)
 * getttimeofday explained:
 * 			- tv_sec is whole seconds since 1970
 * 			- usec is MICROseconds in the current second
 * 				- divide by 1000 to get leftover milliseconds to add
 * 			- TLDR: tv_sec fives time, usec gives fractional remainder of current s
*/

long get_time(t_time_format time_format)
{
	struct timeval	tv;

  if (gettimeofday(&tv, NULL))
    return (0);
  if (time_format == SECOND)
	  return ((uint64_t)tv.tv_sec) + ((uint64_t)tv.tv_usec / 1000000);
  else if (time_format == MILLISECOND)
	  return ((long)tv.tv_sec * 1000) + ((long)tv.tv_usec / 1000);
  else if (time_format == MICROSECOND)
	  return ((long)tv.tv_sec * 1000000) + ((long)tv.tv_usec);
  return (0);
}

/*
 * Precise usleep function: usleep is not precise enough
 * 1) get the 'main' sleep portion from usleep
 * 2) use a spinlock for the precise rest (so that there is no spillage)
*/


