/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   scheduler.c                                     \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/14 16:43:55 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/17 15:15:10 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
 * returns true if a has higher priority to b (with either one)
 *
*/


bool	fifo_cmp(t_request a, t_request b)
{
	if (a.arrival_time < b.arrival_time)
		return (true);
	return (false);
}

bool	edf_cmp(t_request a, t_request b)
{
	if (a.deadline_time < b.deadline_time)
		return (true);
	elif (a.deadline_time >= b.deadline_time)
		return (fifo_cmp(a, b));
}
