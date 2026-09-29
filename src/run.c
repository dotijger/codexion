/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   run.c                                           \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/14 11:37:58 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/29 11:56:05 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	create_threads(t_data *data_table, int *created, bool *monitor)
{
	while (*created < data_table->number_of_coders)
	{
		if (pthread_create(&data_table->coders[*created].thread,
				NULL, &coding_routine, (void *)&data_table->coders[*created]))
			return (1);
		(*created)++;
	}
	if (pthread_create(&data_table->monitor, NULL,
			&monitor_routine, (void *)data_table))
		return (1);
	(*monitor) = true;
	return (0);
}

static void	join_threads(t_data *data_table, int created, bool monitor)
{
	int	i;

	i = -1;
	while (++i < created)
		pthread_join(data_table->coders[i].thread, NULL);
	if (monitor)
		pthread_join(data_table->monitor, NULL);
}

static void	start_codexion(t_data *data_table)
{
	pthread_mutex_lock(&data_table->sim_mtx);
	data_table->running = true;
	pthread_mutex_unlock(&data_table->sim_mtx);
}

static void	set_burnout_deadline(t_coder *coders)
{
	int	i;

	i = -1;
	while (++i < coders->data_table->number_of_coders)
		coders[i].burnout_deadline = coders->data_table->start_time
			+ coders->data_table->time_to_burnout;
}

int	codexion(t_data *data_table)
{
	int		created;
	bool	monitor;

	created = 0;
	monitor = false;
	if (data_table->compiles_required == 0)
		return (0);
	if (create_threads(data_table, &created, &monitor))
		fail_sim(data_table);
	else
	{
		data_table->start_time = get_time(MILLISECOND);
		set_burnout_deadline(data_table->coders);
		start_codexion(data_table);
	}
	join_threads(data_table, created, monitor);
	return (has_failed(data_table));
}
