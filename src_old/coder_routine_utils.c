/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   coder_routine_utils.c                           \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/14 12:04:51 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/21 12:28:04 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	debug(int i, t_data *data_table, long time_to_debug)
{
	log_event(data_table, i + 1, "is debugging\n");
	precise_usleep(time_to_debug * 1000, data_table);
}

void	refactor(int i, t_data *data_table, long time_to_refactor)
{
	log_event(data_table, i + 1, "is refactoring\n");
	precise_usleep(time_to_refactor * 1000, data_table);
}

void	compile(int i, t_data *data_table, long time_to_compile)
{
	log_event(data_table, i + 1, "is compiling\n");
	safe_mutex_handle(&data_table->table_mtx, LOCK);
	data_table->coders[i].last_compile_start = get_time(MILLISECOND);
	data_table->coders[i].burnout_deadline = get_time(MILLISECOND) + data_table->time_to_burnout;
	safe_mutex_handle(&data_table->table_mtx, UNLOCK);
	safe_cond_handle(&data_table->monitor_cond, NULL, ms_to_ts(0), SIGNAL);
	precise_usleep(time_to_compile * 1000, data_table);
	safe_mutex_handle(&data_table->table_mtx, LOCK);
	data_table->coders[i].compiles++;
	safe_mutex_handle(&data_table->table_mtx, UNLOCK);
}

static void	acquire_dongle(t_coder *coder, t_dongle *dongle)
{
	struct timespec	time_ready;

	safe_mutex_handle(&dongle->mtx, LOCK);
	new_request(coder, dongle->heap);
	while (is_running(coder->data_table)
			&& !(my_turn(dongle->heap, coder, dongle)
			&& dongle_ready(dongle, coder->data_table->dongle_cooldown)))
	{
		if (my_turn(dongle->heap, coder, dongle))
		{
			time_ready = ms_to_ts(available_at(dongle, coder->data_table->dongle_cooldown));
			safe_cond_handle(&dongle->cond, &dongle->mtx, time_ready, TIMEDWAIT);
		}
		else
			safe_cond_handle(&dongle->cond, &dongle->mtx, ms_to_ts(0), WAIT);
	}
	if (!is_running(coder->data_table))
	{
		safe_mutex_handle(&dongle->mtx, UNLOCK);
		return ;
	}
	remove_at_index(dongle->heap, 0);
	dongle->taken = true;
	safe_mutex_handle(&dongle->mtx, UNLOCK);
	log_event(coder->data_table, coder->coder_id + 1, "has taken a dongle\n");
	safe_cond_handle(&dongle->cond, NULL, ms_to_ts(0), BROADCAST);
}

static void	release_dongle(t_dongle *dongle)
{
	safe_mutex_handle(&dongle->mtx, LOCK);
	dongle->taken = false;
	dongle->release_time_in_ms = get_time(MILLISECOND);
	safe_mutex_handle(&dongle->mtx, UNLOCK);
	safe_cond_handle(&dongle->cond, NULL, ms_to_ts(0), BROADCAST);
}

void	acquire_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	if (coder->coder_id % 2 == 0)
	{
		first = coder->right;
		second = coder->left;
	}
	else
	{
		first = coder->left;
		second = coder->right;
	}
	acquire_dongle(coder, first);
	if (first != second && is_running(coder->data_table))
		acquire_dongle(coder, second);
	else if (first == second)
	{
		while (is_running(coder->data_table))
			usleep(1000);
		release_dongle(first);
		return ;
	}
}
void	release_dongles(t_coder *coder)
{
	release_dongle(coder->left);
	release_dongle(coder->right);
}

