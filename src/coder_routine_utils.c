/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   coder_routine_utils.c                           \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/14 12:04:51 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/17 16:17:44 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	debug(int i, t_data *data_table, long time_to_debug)
{
	log_event(data_table, i, "is debugging\n");
	precise_usleep(time_to_debug, data_table);
}

void	refactor(int i, t_data *data_table, long time_to_refactor)
{
	log_event(data_table, i, "is refactoring\n");
	precise_usleep(time_to_refactor, data_table);
}

void	compile(int i, t_data *data_table, long time_to_compile)
{
	log_event(data_table, i, "is compiling\n");
	safe_mutex_handle(&data_table->table_mtx, LOCK);
	data_table->coders[i].last_compile_start = get_time(MILLISECOND);
	data_table->coders[i].burnout_deadline = get_time(MILLISECOND) + data_table->time_to_burnout;
	data_table->coders[i].compiles++;
	safe_cond_handle(&data_table->monitor_cond, NULL, NULL, SIGNAL);
	safe_mutex_handle(&data_table->table_mtx, UNLOCK);
	precise_usleep(time_to_compile, data_table);
}

void	acquire_dongles(t_coder *coder)
{
	new_request(coder, coder->data_table->heap);
	safe_mutex_handle(&coder->data_table->dongle_mtx, LOCK);
	while (coder->data_table->running &&
			!(dongle_ready(coder->left->taken) && dongle_ready(coder->right)
				&& my_turn(coder->data_table->heap, coder, coder->left_rival)
				&& my_turn(coder->data_table->heap, coder, coder->right_rival))
			)
		safe_cond_handle(&coder->data_table->dongle_cond, &coder->data_table->dongle_mtx, NULL, WAIT);
	coder->left->taken = true;
	log_event(coder->data_table, coder->coder_id, "has taken a dongle\n");
	coder->right->taken = true;
	log_event(coder->data_table, coder->coder_id, "has taken a dongle\n");
	remove_at_index(&coder->data_table->heap, get_heap_index(&coder->data_table->heap, coder->coder_id));
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

