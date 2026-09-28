/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   clean.c                                         \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/18 18:48:32 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/28 15:46:08 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	destroy_mutexes(t_data *data_table)
{
	int	i;

	i = -1;
	while (++i < data_table->mtx_created)
	{
		if (i == 0)
			pthread_mutex_destroy(&data_table->sim_mtx);
		else if (i == 1)
			pthread_mutex_destroy(&data_table->log_mtx);
		else if (i == 2)
			pthread_mutex_destroy(&data_table->table_mtx);
	}
}

void	free_heap(t_dongle *dongle)
{
	free(dongle->heap->queue);
	free(dongle->heap);
	dongle->heap = NULL;
}

void	clean_up(t_data *data_table)
{
	int	i;

	i = -1;
	while (++i < data_table->dongles_created)
	{
		free_heap(&data_table->dongles[i]);
		if (data_table->dongles[i].mtx_success)
			pthread_mutex_destroy(&data_table->dongles[i].mtx);
		if (data_table->dongles[i].cond_success)
			pthread_cond_destroy(&data_table->dongles[i].cond);
	}
	destroy_mutexes(data_table);
	free(data_table->coders);
	free(data_table->dongles);
}
