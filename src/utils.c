/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   utils.c                                         \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/09 14:55:09 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/20 10:30:16 by odschreu            ..+::##::+..         */
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

/* Logging function (to print the statements)
 *
*/

void	log_event(t_data *data_table, int id, char *event)
{
	t_mtx	*mtx;
	long	now;

	if (!is_running(data_table))
		return ;
	now = get_time(MILLISECOND);
	mtx = &data_table->log_mtx;
	safe_mutex_handle(mtx, LOCK);
	printf("%ld %d %s", (now - data_table->start_time), id, event);
	if (id < 0 || id > data_table->number_of_coders)
		abort();
	fflush(stdout);
	safe_mutex_handle(mtx, UNLOCK);
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

void	precise_usleep(long usec, t_data *data_table)
{
	long	start;
	long	elapsed;
	long	rem;
	
	start = get_time(MICROSECOND);
	if (start == 0)
		error_exit("precise_usleep failed.\n");
	while (get_time(MICROSECOND) - start < usec)
	{
		if (!is_running(data_table))
			break ;
		elapsed = get_time(MICROSECOND) - start;
		rem = usec - elapsed;
		if (rem > 1000) // if remaining time is bigger than 1 millisecond
		{
			usleep(rem / 4); // safety margin against sleep
		}
		else // spinlock until time is up
		{
			while(get_time(MICROSECOND) - start < usec)
				usleep(50);
		}
	}
}

/*
 * Functions to get the left and right positions of the coder
*/ 

int	left(int i)
{
	return (i);
}

int	right(int i, int n)
{
	return ((i - 1 + n) % n);
}
