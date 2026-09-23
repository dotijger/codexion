/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   init.c                                          \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/10 12:51:42 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/23 13:52:05 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	assign_dongles_and_rivals(t_data *data_table, int i)
{
	int	n;
	int	left_idx;
	int	right_idx;

	n = (int)data_table->number_of_coders;
	left_idx = (i + 1 + n) % n;
	right_idx = (i - 1 + n) % n;
	data_table->coders[i].left_rival = &data_table->coders[left_idx];
	data_table->coders[i].right_rival = &data_table->coders[right_idx];
	data_table->coders[i].left = &data_table->dongles[i];
	data_table->coders[i].right = &data_table->dongles[right_idx];
}

static void	init_coder(t_data *data_table, int i)
{
	data_table->coders[i].coder_id = i;
	data_table->coders[i].compiles = 0;
	data_table->coders[i].data_table = data_table;
	data_table->coders[i].last_compile_start = 0;
	data_table->coders[i].burnout_deadline = get_time(MILLISECOND)
		+ data_table->time_to_burnout;
	assign_dongles_and_rivals(data_table, i);
}

static int	init_dongle(t_data *data_table, int i)
{
	data_table->dongles[i].mtx_success = true;
	data_table->dongles[i].cond_success = true;
	data_table->dongles[i].dongle_id = i;
	data_table->dongles[i].taken = false;
	data_table->dongles[i].release_time_in_ms = 0;
	if (data_table->scheduler == FIFO)
	{
		if (init_heap(&data_table->dongles[i].heap, fifo_cmp))
			return (1);
	}
	else
		if (init_heap(&data_table->dongles[i].heap, edf_cmp))
			return (1);
	if (pthread_mutex_init(&data_table->dongles[i].mtx, NULL))
	{
		data_table->dongles[i].mtx_success = false;
		return (fail("Dongle mutex init failed.\n"));
	}
	if (pthread_cond_init(&data_table->dongles[i].cond, NULL))
	{
		data_table->dongles[i].cond_success = false;
		return (fail("Dongle cond init failed.\n"));
	}
	data_table->dongles_created++;
	return (0);
}

static int	create_table_mutexes(t_data *data_table)
{
	if (pthread_mutex_init(&data_table->sim_mtx, NULL))
		return (1);
	data_table->mtx_created++;
	if (pthread_mutex_init(&data_table->log_mtx, NULL))
		return (1);
	data_table->mtx_created++;
	if (pthread_mutex_init(&data_table->table_mtx, NULL))
		return (1);
	data_table->mtx_created++;
	return (0);
}

int	codexion_init(t_data *data_table)
{
	int	i;

	data_table->running = false;
	data_table->failed = false;
	data_table->start_time = 0;
	data_table->coders = (t_coder *)malloc(sizeof(t_coder)
			* data_table->number_of_coders);
	data_table->dongles = (t_dongle *)malloc(sizeof(t_dongle)
			* data_table->number_of_coders);
	if (!data_table->coders || !data_table->dongles)
		return (fail("Malloc failed during *coders and *dongles init.\n"));
	i = -1;
	while (++i < data_table->number_of_coders)
	{
		if (init_dongle(data_table, i))
			return (1);
		init_coder(data_table, i);
	}
	if (create_table_mutexes(data_table))
		return (fail("Mutex creation init failure during data_table init.\n"));
	return (0);
}
