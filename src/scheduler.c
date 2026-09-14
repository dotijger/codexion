/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   scheduler.c                                     \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/14 16:43:55 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/14 16:47:55 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
 * returns 1 if a has higher priority to b (with either one)
 *
*/


int	fifo_cmp(t_request a, t_request b)
{
	if (a.arrival_time < b.arrival_time)
		return (1);
	elif (a.arrival_time > b.arrival_time)
		return (-1);
	return (0);
}

int	edf_cmp(t_request a, t_request b)
{
	if (a.deadline_time < b.deadline_time)
		return (1);
	elif (a.deadline_time > b.deadline_time)
		return (fifo_cmp(a, b));
}
