/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   log.c                                           \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/29 11:37:01 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/29 11:37:44 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_event(t_data *data_table, int id, char *event)
{
	pthread_mutex_lock(&data_table->log_mtx);
	if (is_running(data_table))
		printf("%ld %d %s",
			get_time(MILLISECOND) - data_table->start_time, id, event);
	pthread_mutex_unlock(&data_table->log_mtx);
}

void	log_burnout(t_data *data_table, int id)
{
	pthread_mutex_lock(&data_table->log_mtx);
	if (is_running(data_table))
	{
		stop_sim(data_table);
		printf("%ld %d burned out\n",
			get_time(MILLISECOND) - data_table->start_time, id);
	}
	pthread_mutex_unlock(&data_table->log_mtx);
}
