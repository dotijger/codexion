/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   sim_utils.c                                     \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/21 17:51:13 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/23 14:02:38 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_event(t_data *data_table, int id, char *event)
{
	long	now;

	if (!is_running(data_table))
		return ;
	pthread_mutex_lock(&data_table->log_mtx);
	now = get_time(MILLISECOND);
	printf("%ld %d %s", (now - data_table->start_time), id, event);
	fflush(stdout);
	pthread_mutex_unlock(&data_table->log_mtx);
}

void	precise_usleep(long usec, t_data *data_table)
{
	long	start;
	long	elapsed;
	long	rem;

	start = get_time(MICROSECOND);
	while (get_time(MICROSECOND) - start < usec)
	{
		if (!is_running(data_table))
			break ;
		elapsed = get_time(MICROSECOND) - start;
		rem = usec - elapsed;
		if (rem > 1000)
		{
			usleep(rem / 4);
		}
		else
		{
			while (get_time(MICROSECOND) - start < usec)
				usleep(50);
		}
	}
}

bool	is_running(t_data *data_table)
{
	bool	status;

	pthread_mutex_lock(&data_table->sim_mtx);
	status = data_table->running;
	pthread_mutex_unlock(&data_table->sim_mtx);
	return (status);
}

void	stop_sim(t_data *data_table)
{
	pthread_mutex_lock(&data_table->sim_mtx);
	data_table->running = false;
	pthread_mutex_unlock(&data_table->sim_mtx);
}

void	fail_sim(t_data *data_table)
{
	pthread_mutex_lock(&data_table->sim_mtx);
	data_table->failed = true;
	data_table->running = false;
	pthread_mutex_unlock(&data_table->sim_mtx);
}
