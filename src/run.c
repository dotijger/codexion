/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   run.c                                           \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/14 11:37:58 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/23 16:06:20 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
	int	i;

	i = -1;
	if (data_table->compiles_required == 0)
		return (0);
	while (++i < data_table->number_of_coders)
		pthread_create(&data_table->coders[i].thread,
			NULL, &coding_routine, (void *)&data_table->coders[i]);
	pthread_create(&data_table->monitor,
		NULL, &monitor_routine, (void *)data_table);
	data_table->start_time = get_time(MILLISECOND);
	set_burnout_deadline(data_table->coders);
	start_codexion(data_table);
	i = -1;
	while (++i < data_table->number_of_coders)
		pthread_join(data_table->coders[i].thread, NULL);
	pthread_join(data_table->monitor, NULL);
	if (data_table->failed)
		return (1);
	return (0);
}
