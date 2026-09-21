/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   scheduler.c                                     \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/14 16:43:55 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/21 17:50:15 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
	else if (a.deadline_time >= b.deadline_time)
		return (fifo_cmp(a, b));
	return (false);
}
