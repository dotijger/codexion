/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   init.c                                          \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/10 12:51:42 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/11 16:02:08 by odschreu            ..+::##::+..         */
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
	data_table->coders[i].left_dongle = &data_table->dongles[i];
	data_table->coders[i].right_dongle = &data_table->dongles[i - 1 + n % n];
}

static void	init_coder(t_data *data_table, int i)
{
	data_table->coders[i].id = i + 1;
	data_table->coders[i].number_of_compiles = 0;
	data_table->coders[i].done = false;
	data_table->coders[i].data_table = data_table;
	assign_dongles(data_table, i);
	safe_mutex_handle(&data_table->coders[i].coder_mtx, INIT);
	// TODO: last_compile_start and time_to_burnout have not been initialized (!)
}

static void	init_dongle(t_data *data_table, int i)
{
	data_table->dongles[i].dongle_id = i;
	data_table->dongles[i].taken = false;
	data_table->dongles[i].cooling_down = false;
	safe_mutex_handle(&data_table->dongles[i].dongle_mtx, INIT);
	safe_cond_handle(&data_table->dongles[i].dongle_cond, NULL, INIT);
}

void	codexion_init(t_data *data_table)
{
	int	i;

	data_table->start = false;
	data_table->finished = false;
	data_table->coders = (t_coder *)safe_malloc(sizeof(t_coder) * data_table->number_of_coders);
	data_table->dongles = (t_dongle *)safe_malloc(sizeof(t_dongle) * data_table->number_of_coders);
	i = -1;
	while (++i < data_table->number_of_coders)
		init_dongle(data_table, i);
	i = -1;
	while (++i < data_table->number_of_coders)
		init_coder(data_table, i);
	safe_mutex_handle(&data_table->read_mtx, CREATE);
	safe_mutex_handle(&data_table->write_mtx, CREATE);
}
