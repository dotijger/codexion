/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   routine.c                                       \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/14 16:00:09 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/16 14:05:36 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	burned_out(t_coder *coders)
{
	long	now;

	while (coders++)
	{
		now = get_time(MILLISECOND);
		if (coder->time_to_burnout < now)
			return (true);
	}
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

void	coding_routine(void *arg)
{
	t_data	*data_table;
	t_coder	coder;

	coder = (t_coder)&arg;
	data_table = coder.data_table;
	while (!data_table->running)
		;

	while (coder.compiles < data_table->compiles_required
			&& data_table->running)
	{
		acquire_dongles(coder);
		compile(i, data_table, coder.time_to_compile);
		release_dongles(coder);
		debug(i, data_table, coder.time_to_debug);
		refactor(i, data_table, coder.time_to_refactor);
	}

}

void	monitor_routine(void *arg)
{
	t_data	*data_table;

	data_table = (t_data *)arg;
	safe_mutex_handle(&data_table->table_mtx, LOCK);
	data_table->running = true;
	data_table->start_time = get_time(MILLISECOND);
	safe_mutex_handle(&data_table->table_mtx, UNLOCK);
	while (data_table->running)
	{
		if 
	}
	safe_mutex_handle(&data_table->table_mtx, LOCK);
	data_table->running = false;
	safe_mutex_handle(&data_table->table_mtx, UNLOCK);
}
