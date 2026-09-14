/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   hold_and_wait.c                                 \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/11 15:15:31 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/14 10:36:20 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
 * To avoid deadlock, will make use of the "hold and wait" prevention:
 *  - all coders can only take the dongles if they are both free.
 *  - so cond_wait while loop to check whether the dongles are both free
 *  - then take both dongles if that is the case
 *
*/

struct timespec	compute_deadline(t_coder *coder)
{
	struct timespec	deadline;
	long	deadline_in_ms;

	deadline_in_ms = coder->last_compile_start + coder->time_to_burnout;
	deadline.tv_sec = deadline_in_ms / 1000;
	deadline.tv_nsec = (deadline_in_ms % 1000) * 1000000; // (ms remainder)

	return deadline;
}

bool	*both_dongles_free(t_data *coder)
{
	return (!coder->left_dongle->taken && !coder->right_dongle->taken);

}

void	acquire_dongles(t_data *coder)
{
	struct timespec	deadline;
	t_data	*data_table;

	data_table = coder->data_table;

	deadline = compute_deadline(coder);

	safe_mutex_lock(data_table->table_mtx, LOCK);
	while (!both_dongles_free(coder))
	{
		safe_cond_handle(data_table->dongle_cond, data_table->table_mtx, &deadline, TIMEDWAIT);

	}

	
}

