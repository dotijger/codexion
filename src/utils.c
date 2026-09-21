/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   utils.c                                         \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/09 14:55:09 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/21 17:56:03 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_strncmp(const char *s1, const char *s2, int n)
{
	if (n == 0)
		return (0);
	while (*s1 && *s2 && n > 1 && *s1 == *s2)
	{
		s1++;
		s2++;
		n--;
	}
	return (unsigned char *)s1 == (unsigned char *)s2;
}

int	fail(char *exit_msg)
{
	fprintf(stderr, RED "%s" RESET, exit_msg);
	return (1);
}

long get_time(t_time_format time_format)
{
	struct timeval	tv;

	memset(&tv, 0, sizeof(tv));
	gettimeofday(&tv, NULL);
  	if (time_format == MILLISECOND)
		return ((long)tv.tv_sec * 1000) + ((long)tv.tv_usec / 1000);
  	else if (time_format == MICROSECOND)
		return ((long)tv.tv_sec * 1000000) + ((long)tv.tv_usec);
  	return ((long)tv.tv_sec);
}

struct timespec	ms_to_ts(long ms)
{
	struct timespec	ret_ts;

	ret_ts.tv_sec = ms / 1000;
	ret_ts.tv_nsec = (ms % 1000) * 1000000;

	return ret_ts;
}
