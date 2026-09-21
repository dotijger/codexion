/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   routine.c                                       \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/14 16:00:09 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/21 12:19:45 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <errno.h>

static bool	burned_out(t_coder *coders)
{
	long	now;
	int		i;

	i = -1;
	while (++i < coders->data_table->number_of_coders)
	{
		now = get_time(MILLISECOND);
		if (!(coders[i].compiles >= coders->data_table->compiles_required) && now > coders[i].burnout_deadline)
		{
			log_event(coders[i].data_table, coders[i].coder_id + 1, "burned out\n");
			return (true);
		}
	}
	return (false);
}

static bool	done(t_coder *coders)
{
	int	i;

	i = -1;
	while (++i < coders->data_table->number_of_coders)
	{
		if (coders[i].compiles < coders[i].data_table->compiles_required)
			return (false);
	}
	return (true);
}

void	*coding_routine(void *arg)
{
	t_data	*data_table;
	t_coder	*coder;

	coder = (t_coder *)arg;
	data_table = coder->data_table;
	while (!is_running(data_table))
		usleep(100);

	while (coder->compiles < data_table->compiles_required
			&& is_running(data_table))
	{
		acquire_dongles(coder);
		compile(coder->coder_id, data_table, coder->data_table->time_to_compile);
		release_dongles(coder);
		if (coder->compiles == data_table->compiles_required)
			return (NULL);
		debug(coder->coder_id, data_table, coder->data_table->time_to_debug);
		refactor(coder->coder_id, data_table, coder->data_table->time_to_refactor);
	}
	return (NULL);

}
struct timespec	ms_to_ts(long ms)
{
	struct timespec	ret_ts;

	ret_ts.tv_sec = ms / 1000;
	ret_ts.tv_nsec = (ms % 1000) * 1000000;

	return ret_ts;
}

struct timespec	give_earliest_deadline(t_coder *coders)
{
	long	deadline_ms;
	long	coder_dl;
	int	i;

	i = -1;
	deadline_ms = -1;
	while (++i < coders->data_table->number_of_coders)
	{
		coder_dl = coders[i].burnout_deadline;
		if (deadline_ms == -1 || deadline_ms > coder_dl)
			deadline_ms = coder_dl;
	}
	return ms_to_ts(deadline_ms);
}

static void	broadcast_to_dongles(t_data *data_table)
{
	int	i;

	i = -1;
	while (++i < data_table->number_of_coders)
	{
		safe_mutex_handle(&data_table->dongles[i].mtx, LOCK);
		safe_cond_handle(&data_table->dongles[i].cond, NULL, ms_to_ts(0), BROADCAST);
		safe_mutex_handle(&data_table->dongles[i].mtx, UNLOCK);
	}
}

void	*monitor_routine(void *arg)
{
	t_data	*data_table;
//	int		ret_code;
	//struct timespec	deadline;

	data_table = (t_data *)arg;
	while (!is_running(data_table))
		usleep(100);

	while (is_running(data_table))
	{
		// deadline = give_earliest_deadline(data_table->coders);
		//cret_code = pthread_cond_timedwait(&data_table->monitor_cond,
											// &data_table->table_mtx, &deadline);
		safe_mutex_handle(&data_table->table_mtx, LOCK);
		if (done(data_table->coders))
			stop_sim(data_table);
		else if (burned_out(data_table->coders))
			stop_sim(data_table);
		safe_mutex_handle(&data_table->table_mtx, UNLOCK);
		if (is_running(data_table))
			precise_usleep(200, data_table);
	}
	broadcast_to_dongles(data_table);
	return (NULL);
}
