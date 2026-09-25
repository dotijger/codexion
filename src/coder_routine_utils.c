/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   coder_routine_utils.c                           \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/14 12:04:51 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/24 13:55:09 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	debug(int i, t_data *data_table, long time_to_debug)
{
	log_event(data_table, i + 1, "is debugging\n");
	precise_usleep(time_to_debug * 1000, data_table);
}

void	refactor(int i, t_data *data_table, long time_to_refactor)
{
	log_event(data_table, i + 1, "is refactoring\n");
	precise_usleep(time_to_refactor * 1000, data_table);
}

void	compile(int i, t_data *data_table, long time_to_compile)
{
	log_event(data_table, i + 1, "is compiling\n");
	pthread_mutex_lock(&data_table->table_mtx);
	data_table->coders[i].last_compile_start = get_time(MILLISECOND);
	data_table->coders[i].burnout_deadline = get_time(MILLISECOND)
		+ data_table->time_to_burnout;
	pthread_mutex_unlock(&data_table->table_mtx);
	precise_usleep(time_to_compile * 1000, data_table);
	pthread_mutex_lock(&data_table->table_mtx);
	data_table->coders[i].compiles++;
	pthread_mutex_unlock(&data_table->table_mtx);
}

int	acquire_dongles(t_coder *coder)
{
	t_dongle	*first;
	t_dongle	*second;

	assign_order(coder, &first, &second);
	if (first == second)
	{
		log_event(coder->data_table, coder->coder_id + 1, "has taken a dongle\n");
		while (is_running(coder->data_table))
			usleep(1000);
		return (0);
	}
	if (enter_queue(coder, first, second))
		return (1);
	if (take_dongles(coder, first, second))
		return (1);
	return (0);
}

void	release_dongles(t_coder *coder)
{
	release_dongle(coder->left);
	release_dongle(coder->right);
}
