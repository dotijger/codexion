/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   routine.c                                       \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/14 16:00:09 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/18 11:28:08 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	burned_out(t_coder *coders)
{
	long	now;
	bool	burnout;

	burnout = false;
	while (coders++)
	{
		now = get_time(MILLISECOND);
		if (coders->burnout_deadline < now)
		{
			log_event(coders->data_table, coders->coder_id, "burned out\n");
			burnout = true;
		}
	}
	if (burnout)
		return (true);
	return (false);
}

static bool	done(t_coder *coders)
{
	while (coders++)
	{
		if (coders->compiles < coders->data_table->compiles_required)
			return (false);
	}
	return (true);
}

static void	start_codexion(t_data *data_table)
{
	safe_mutex_handle(&data_table->table_mtx, LOCK);
	data_table->running = true;
	data_table->start_time = get_time(MILLISECOND);
	safe_mutex_handle(&data_table->table_mtx, UNLOCK);
}


void	coding_routine(void *arg)
{
	t_data	*data_table;
	t_coder	coder;

	coder = &(t_coder *)arg;
	data_table = coder.data_table;
	while (!data_table->running)
		;

	while (coder.compiles < data_table->compiles_required
			&& data_table->running)
	{
		acquire_dongles(&coder);
		compile(i, data_table, coder.time_to_compile);
		release_dongles(&coder);
		if (coder.compiles == data__table->compiles_required)
			return ;
		debug(i, data_table, coder.time_to_debug);
		refactor(i, data_table, coder.time_to_refactor);
	}

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
	int	i;

	i = -1;
	deadline = -1;
	while (++i < coders[i].data_table->number_of_coders)
	{
		coder_dl = coders[i].burnout_deadline;
		if (deadline_ms == -1 || deadline_ms > coder_dl)
			deadline_ms = coder_dl;
	}
	return ms_to_ts(deadline_ms);
}

void	monitor_routine(void *arg)
{
	t_data	*data_table;
	int		ret_code;
	struct timespec	deadline;

	data_table = (t_data *)arg;
	start_codexion(data_table);
	safe_mutex_handle(&data_table->table_mtx, LOCK);
	while (data_table->running)
	{
		deadline = give_earliest_deadline(&data_table->coders);
		ret_code = pthread_cond_timedwait(&data_table->monitor_cond,
											&data_table->table_mtx, &deadline);
		if (ret_code == ETIMEDOUT)
		{
			if (burned_out(&data_table->coders))
				data_table->running = false;
		}
		if (done(&data_table->coders))
			data_table->running = false;
	}
	safe_mutex_handle(&data_table->table_mtx, UNLOCK);
}
