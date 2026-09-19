/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   run.c                                           \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/14 11:37:58 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/19 10:13:04 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	codexion(t_data *data_table)
{
	int	i;

	i = -1;
	while (++i < data_table->number_of_coders)
		safe_thread_handle(&data_table->coders[i].thread, &coding_routine, (void *)&data_table->coders[i], CREATE);

	safe_thread_handle(&data_table->monitor, &monitor_routine, (void *)data_table, CREATE);

	i = -1;
	while (++i < data_table->number_of_coders)
		safe_thread_handle(&data_table->coders[i].thread, NULL, NULL, JOIN);
	safe_thread_handle(&data_table->monitor, NULL, NULL, JOIN);
}
