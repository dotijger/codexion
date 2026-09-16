/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   coder_routine_utils.c                           \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/14 12:04:51 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/16 14:02:58 by odschreu            ..+::##::+..         */
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
	safe_mutex_handle(&data_table->table_mtx, LOCK);
	data_table->coders[i].last_compile_start = get_time(MILLISECOND);
	data_table->coders[i].compiles++;
	safe_cond_handle(&data_table->monitor_cond, NULL, NULL, SIGNAL);
	safe_mutex_handle(&data_table->table_mtx, UNLOCK);
	precise_usleep(time_to_compile, data_table);
}

bool	dongles_ready(t_dongle *a, t_dongle *b, long cooldown_ms)
{
	now = get_time(MILLISECOND);
	return (now >= (a.release_time_in_ms + cooldown_ms)
			&& now >= (b.release_time_in_ms + cooldown_ms));
}

void	acquire_dongles(t_coder *coder)
{
	safe_mutex_handle(&coder->data_table->dongle_mtx, LOCK);
	while (coder->left->taken && coder->right->taken
			&& dongles_ready(coder->left, coder->right, coder->data_table->dongle_cooldown)
			&& my_turn(coder)) // TODO write the myturn logic !!! 
		safe_cond_handle(&coder->data_table->dongle_cond, &coder->data_table->dongle_mtx, NULL, WAIT);
	coder->left->taken = true;
	coder->right->taken = true;
	safe_mutex_handle(&coder->data_table->dongle_mtx, UNLOCK);
}

void	release_dongles(t_coder *coder)
{
	safe_mutex_handle(&coder->data_table->dongle_mtx, LOCK);
	coder->left->taken = false;
	coder->left->release_time_in_ms = get_time(MILLISECOND);
	coder->right->taken = false;
	coder->right->release_time_in_ms = get_time(MILLISECOND);
	safe_mutex_handle(&coder->data_table->dongle_mtx, UNLOCK);
	safe_cond_handle(&coder->data_table->dongle_cond, BROADCAST);
}

