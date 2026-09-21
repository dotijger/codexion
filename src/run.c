/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   run.c                                           \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/14 11:37:58 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/21 12:20:23 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	start_codexion(t_data *data_table)
{
	safe_mutex_handle(&data_table->sim_mtx, LOCK);
	data_table->running = true;
	safe_mutex_handle(&data_table->sim_mtx, UNLOCK);
}

static void	set_burnout_deadline(t_coder *coders)
{
	int	i;

	i = -1;
	while (++i < coders->data_table->number_of_coders)
		coders[i].burnout_deadline = coders->data_table->start_time + coders->data_table->time_to_burnout;
}



void	codexion(t_data *data_table)
{
	int	i;

	i = -1;
	while (++i < data_table->number_of_coders)
		safe_thread_handle(&data_table->coders[i].thread, &coding_routine, (void *)&data_table->coders[i], CREATE);

	safe_thread_handle(&data_table->monitor, &monitor_routine, (void *)data_table, CREATE);

	data_table->start_time = get_time(MILLISECOND);
	set_burnout_deadline(data_table->coders);
	start_codexion(data_table);

	i = -1;
	while (++i < data_table->number_of_coders)
		safe_thread_handle(&data_table->coders[i].thread, NULL, NULL, JOIN);
	safe_thread_handle(&data_table->monitor, NULL, NULL, JOIN);
}
