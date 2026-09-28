/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   dongle.c                                        \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/21 17:58:46 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/28 18:32:05 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <sys/time.h>

static bool	dongle_available(t_coder *coder, t_dongle *dongle)
{
	return (my_turn(dongle->heap, coder, dongle)
		&& dongle_ready(dongle, coder->data_table->dongle_cooldown));
}

static void	wait_for_dongles(t_coder *coder, t_dongle *dongle, t_dongle *other)
{
	struct timespec	time_ready;
	struct timespec	time_one;
	long			time_ready_ms;
	t_data			*data_table;

	data_table = coder->data_table;
	while (is_running(coder->data_table) && !(dongle_available(coder, dongle)
		&& dongle_available(coder, other)))
	{
		time_ready_ms = min_cooldown(
				available_at(dongle, data_table->dongle_cooldown),
				available_at(other, data_table->dongle_cooldown));
		pthread_mutex_unlock(&other->mtx);
		if (my_turn(dongle->heap, coder, dongle))
		{

			time_ready = ms_to_ts(time_ready_ms);
			time_one = ms_to_ts(get_time(MILLISECOND) + 1);
			if (get_time(MILLISECOND) >= time_ready_ms)
					pthread_cond_timedwait(&dongle->cond, &dongle->mtx, &time_one);
			else
				pthread_cond_timedwait(&dongle->cond, &dongle->mtx, &time_ready);
		}
		else
			pthread_cond_wait(&dongle->cond, &dongle->mtx);
		pthread_mutex_lock(&other->mtx);
	}
}

static int	get_dongles(t_coder *coder, t_dongle *dongle, t_dongle *other)
{
	int	dongle_idx;
	int	other_idx;

	dongle_idx = get_heap_index(dongle->heap, coder->coder_id);
	other_idx = get_heap_index(other->heap, coder->coder_id);
	if (remove_at_index(dongle->heap, dongle_idx)
		|| remove_at_index(other->heap, other_idx))
	{
		pthread_mutex_unlock(&dongle->mtx);
		pthread_mutex_unlock(&other->mtx);
		return (1);
	}
	dongle->taken = true;
	other->taken = true;
	pthread_cond_broadcast(&dongle->cond);
	pthread_cond_broadcast(&other->cond);
	pthread_mutex_unlock(&dongle->mtx);
	pthread_mutex_unlock(&other->mtx);
	log_event(coder->data_table, coder->coder_id + 1, "has taken a dongle\n");
	log_event(coder->data_table, coder->coder_id + 1, "has taken a dongle\n");
	return (0);
}

int	take_dongles(t_coder *coder, t_dongle *dongle, t_dongle *other)
{
	pthread_mutex_lock(&dongle->mtx);
	pthread_mutex_lock(&other->mtx);
	wait_for_dongles(coder, dongle, other);
	if (!is_running(coder->data_table))
	{
		pthread_mutex_unlock(&dongle->mtx);
		pthread_mutex_unlock(&other->mtx);
		return (1);
	}
	return (get_dongles(coder, dongle, other));
}

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mtx);
	dongle->taken = false;
	dongle->release_time_in_ms = get_time(MILLISECOND);
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mtx);
}
