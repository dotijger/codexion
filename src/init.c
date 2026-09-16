/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   init.c                                          \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/10 12:51:42 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/16 13:49:54 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
 * Initializing :
 * 		- the array of coders (malloc) and initializing values of the coder struct
 * 		- the array of dongles (malloc) and initializing the values of the dongle struct
 *
*/ 

static void	assign_dongles(t_data *data_table, int i)
{
	int	n;

	n = (int)data_table->number_of_coders;
	data_table->coders[i].left = &data_table->dongles[i];
	data_table->coders[i].right = &data_table->dongles[(i - 1 + n) % n];
}

static void	init_coder(t_data *data_table, int i)
{
	data_table->coders[i].id = i + 1;
	data_table->coders[i].compiles = 0;
	data_table->coders[i].data_table = data_table;
	data_table->coders[i].last_compile_start = get_time(MILLISECOND);
	data_table->coders[i].time_to_burnout = get_time(MILLISECOND) + data_table->time_to_burnout;
	assign_dongles(data_table, i);
}

static void	init_dongle(t_data *data_table, int i)
{
	data_table->dongles[i].dongle_id = i;
	data_table->dongles[i].taken = false;
}

void	codexion_init(t_data *data_table)
{
	int	i;

	if (data_table->scheduler == FIFO)
		init_heap(&data_table->heap, fifo_cmp);
	else
		init_heap(&data_table->heap, edf_cmp);
	data_table->running = false;
	data_table->start_time = 0;
	data_table->coders = (t_coder *)safe_malloc(sizeof(t_coder) * data_table->number_of_coders);
	data_table->dongles = (t_dongle *)safe_malloc(sizeof(t_dongle) * data_table->number_of_coders);
	i = -1;
	while (++i < data_table->number_of_coders)
		init_dongle(data_table, i);
	i = -1;
	while (++i < data_table->number_of_coders)
		init_coder(data_table, i);
	safe_cond_handle(&data_table->dongle_cond, NULL, NULL, CREATE);
	safe_cond_handle(&data_table->monitor_cond, NULL, NULL, CREATE);
	safe_mutex_handle(&data_table->log_mtx, CREATE);
	safe_mutex_handle(&data_table->table_mtx, CREATE);
	safe_mutex_handle(&data_table->dongle_mtx, CREATE);
}
