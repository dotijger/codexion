/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   run.c                                           \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/14 11:37:58 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/14 16:06:14 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	codexion(t_data *data_table)
{
	i = -1;
	while (++i < data_table->number_of_coders)
		safe_thread_handle(&data_table->coders[i].thread_id, coding, data_table->coders[i], CREATE);

	safe_thread_handle(&data_table->monitor, monitor, data_table, CREATE);

	i = -1;
	while (++i < data_table->number_of_coders)
		safe_thread_handle(data_table->coders[i].thread_id, NULL, NULL, JOIN);
	safe_thread_handle(&data_table->monitor, NULL, NULL, JOIN);
}
