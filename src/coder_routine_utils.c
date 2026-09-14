/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   coder_routine_utils.c                           \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/14 12:04:51 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/14 16:22:20 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	debug(int i, t_data *data_table, long time_to_debug)
{
	log_event(i, "is debugging\n");
	precise_usleep(time_to_debug, data_table);
}

void	refactor(int i, t_data *data_table, long time_to_refactor)
{
	log_event(i, "is refactoring\n");
	precise_usleep(time_to_refactor, data_table);
}

void	compile(int i, t_data *data_table, long time_to_compile)
{
	log_event(i, "is compiling\n");
	precise_usleep(time_to_compile, data_table);
}


void	acquire_dongle(t_dongle *dongle)
{
	
}

void	test(int i, t_data *data_table)
{
	t_coder	*coders;

	coders = data_table->coders;
	while (coders[i].state == WAITING
			&& coders[left(i)] != COMPILING
			&& coders[right(i)] != COMPILING)
	{
		coders[i].state = COMPILING;
		safe_cond_handle(&compile_ready, SIGNAL);
	}
}
