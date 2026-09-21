/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   safe_utils.c                                    \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/10 12:57:35 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/19 16:05:24 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <errno.h>

/*
 * Safe versions of:
 * 		- malloc
 * 		- thread/mutex functions
*/

void	*safe_malloc(size_t size)
{
	void	*pointer;

	pointer = malloc(size);
	if (!pointer)
		error_exit("Malloc error, exiting program.");
	return pointer;
}

void	safe_mutex_handle(t_mtx *mtx, t_pthread_code code)
{
	int	status;

	status = 0;
	if (code == LOCK)
		status = pthread_mutex_lock(mtx);
	else if (code == UNLOCK)
		status = pthread_mutex_unlock(mtx);
	else if (code == INIT)
		status = pthread_mutex_init(mtx, NULL);
	else if (code == DESTROY)
		status = pthread_mutex_destroy(mtx);
	else
		error_exit("Wrong code for mutex_handle function.");
	if (status)
		error_exit("Fatal mutex error, exiting program.");
}

void	safe_cond_handle(t_cond *cond, t_mtx *mtx, struct timespec time, t_pthread_code code)
{
	int	status;

	status = 0;
	if (code == INIT)
		status = pthread_cond_init(cond, NULL);
	else if (code == WAIT)
		status = pthread_cond_wait(cond, mtx);
	else if (code == TIMEDWAIT)
		status = pthread_cond_timedwait(cond, mtx, &time);
	else if (code == SIGNAL)
		status = pthread_cond_signal(cond);
	else if (code == BROADCAST)
		status = pthread_cond_broadcast(cond);
	else if (code == DESTROY)
		status = pthread_cond_destroy(cond);
	else
		error_exit("Wrong code for cond_handle function.");
	if (status != 0 && status != ETIMEDOUT)
		error_exit("Fatal cond error, exiting program.");
}

void	safe_thread_handle(
			pthread_t *thread,
			void *(*start_routine)(void *),
			void *data,
			t_pthread_code code
			)
{
	int	status;
	
	status = 0;
	if (code == CREATE)
		status = pthread_create(thread, NULL, start_routine, data);
	else if (code == JOIN)
		status = pthread_join(*thread, NULL);
	else if (code == DETACH)
		status = pthread_detach(*thread);
	else
		error_exit("Wrong code for thread handle function.");
	if (status)
		error_exit("Fatal thread error, exiting program.");
}

bool	is_running(t_data *data_table)
{
	bool	status;

	safe_mutex_handle(&data_table->sim_mtx, LOCK);
	status = data_table->running;
	safe_mutex_handle(&data_table->sim_mtx, UNLOCK);

	return (status);
}

void	stop_sim(t_data *data_table)
{
	safe_mutex_handle(&data_table->sim_mtx, LOCK);
	data_table->running = false;
	safe_mutex_handle(&data_table->sim_mtx, UNLOCK);
}
