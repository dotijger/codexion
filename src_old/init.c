/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   init.c                                          \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/10 12:51:42 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/21 15:12:47 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
 * Initializing :
 * 		- the array of coders (malloc) and initializing values of the coder struct
 * 		- the array of dongles (malloc) and initializing the values of the dongle struct
 *
*/ 

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
	data_table->coders[i].left = &data_table->dongles[left(i)];
	data_table->coders[i].right = &data_table->dongles[right(i, n)];
}

static void	init_coder(t_data *data_table, int i)
{
	data_table->coders[i].coder_id = i;
	data_table->coders[i].compiles = 0;
	data_table->coders[i].data_table = data_table;
	data_table->coders[i].last_compile_start = 0;
	data_table->coders[i].burnout_deadline = get_time(MILLISECOND) + data_table->time_to_burnout;
	assign_dongles_and_rivals(data_table, i);
}

static void	init_dongle(t_data *data_table, int i)
{

	data_table->dongles[i].dongle_id = i;
	data_table->dongles[i].taken = false;
	data_table->dongles[i].release_time_in_ms = 0;
	if (data_table->scheduler == FIFO)
		init_heap(&data_table->dongles[i].heap, fifo_cmp);
	else
		init_heap(&data_table->dongles[i].heap, edf_cmp);
	safe_mutex_handle(&data_table->dongles[i].mtx, INIT);
	safe_cond_handle(&data_table->dongles[i].cond, NULL, ms_to_ts(0), INIT);
}

void	codexion_init(t_data *data_table)
{
	int	i;

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
	safe_mutex_handle(&data_table->sim_mtx, INIT);
	safe_mutex_handle(&data_table->log_mtx, INIT);
	safe_mutex_handle(&data_table->table_mtx, INIT);
}
