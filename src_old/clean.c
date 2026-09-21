/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   clean.c                                         \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/18 18:48:32 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/19 15:44:34 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	clean_up(t_data *data_table)
{
	int	i;
	// destroy the mutexes and conds
	safe_cond_handle(&data_table->dongle_cond, NULL, ms_to_ts(0), DESTROY);
	safe_cond_handle(&data_table->monitor_cond, NULL, ms_to_ts(0), DESTROY);
	safe_mutex_handle(&data_table->log_mtx, DESTROY);
	safe_mutex_handle(&data_table->table_mtx, DESTROY);
	safe_mutex_handle(&data_table->dongle_mtx, DESTROY);
	i = -1;
	while (++i < data_table->number_of_coders)
	{
		free(data_table->dongles[i].heap->queue);
		free(data_table->dongles[i].heap);
		safe_mutex_handle(&data_table->dongles[i].mtx, DESTROY);
		safe_cond_handle(&data_table->dongles[i].cond, NULL, ms_to_ts(0), DESTROY);
	}
	// free the coders and dongles
	free(data_table->coders);
	free(data_table->dongles);
}
