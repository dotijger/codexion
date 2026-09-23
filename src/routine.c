/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   routine.c                                       \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/14 16:00:09 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/23 14:04:26 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	burned_out(t_coder *coders)
{
	long	now;
	int		i;

	i = -1;
	while (++i < coders->data_table->number_of_coders)
	{
		now = get_time(MILLISECOND);
		if (!(coders[i].compiles >= coders->data_table->compiles_required)
			&& now > coders[i].burnout_deadline)
		{
			log_event(coders[i].data_table,
				coders[i].coder_id + 1, "burned out\n");
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

static void	broadcast_to_dongles(t_data *data_table)
{
	int	i;

	i = -1;
	while (++i < data_table->number_of_coders)
	{
		pthread_mutex_lock(&data_table->dongles[i].mtx);
		pthread_cond_broadcast(&data_table->dongles[i].cond);
		pthread_mutex_unlock(&data_table->dongles[i].mtx);
	}
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
		if (acquire_dongles(coder))
		{
			fail_sim(data_table);
			return (NULL);
		}
		compile(coder->coder_id,
			data_table, coder->data_table->time_to_compile);
		release_dongles(coder);
		if (coder->compiles == data_table->compiles_required)
			return (NULL);
		debug(coder->coder_id, data_table, coder->data_table->time_to_debug);
		refactor(coder->coder_id,
			data_table, coder->data_table->time_to_refactor);
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_data	*data_table;

	data_table = (t_data *)arg;
	while (!is_running(data_table))
		usleep(100);
	while (is_running(data_table))
	{
		pthread_mutex_lock(&data_table->table_mtx);
		if (done(data_table->coders))
			stop_sim(data_table);
		else if (burned_out(data_table->coders))
			stop_sim(data_table);
		pthread_mutex_unlock(&data_table->table_mtx);
		if (is_running(data_table))
			precise_usleep(200, data_table);
	}
	broadcast_to_dongles(data_table);
	return (NULL);
}
