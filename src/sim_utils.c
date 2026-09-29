/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   sim_utils.c                                     \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/21 17:51:13 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/29 11:37:20 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	is_running(t_data *data_table)
{
	bool	status;

	pthread_mutex_lock(&data_table->sim_mtx);
	status = data_table->running;
	pthread_mutex_unlock(&data_table->sim_mtx);
	return (status);
}

bool	has_failed(t_data *data_table)
{
	bool	status;

	pthread_mutex_lock(&data_table->sim_mtx);
	status = data_table->failed;
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
