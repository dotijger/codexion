/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   dongle.c                                        \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/21 17:58:46 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/23 14:02:04 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <sys/time.h>

static bool	dongle_available(t_coder *coder, t_dongle *dongle)
{
	return (my_turn(dongle->heap, coder, dongle)
		&& dongle_ready(dongle, coder->data_table->dongle_cooldown));
}

static void	wait_for_dongle(t_coder *coder, t_dongle *dongle)
{
	struct timespec	time_ready;
	t_data			*data_table;

	data_table = coder->data_table;
	while (is_running(coder->data_table) && !dongle_available(coder, dongle))
	{
		if (my_turn(dongle->heap, coder, dongle))
		{
			time_ready = ms_to_ts(available_at(
						dongle, data_table->dongle_cooldown));
			pthread_cond_timedwait(&dongle->cond, &dongle->mtx, &time_ready);
		}
		else
			pthread_cond_wait(&dongle->cond, &dongle->mtx);
	}
}

static int	get_dongle(t_coder *coder, t_dongle *dongle)
{
	if (remove_at_index(dongle->heap, 0))
	{
		pthread_mutex_unlock(&dongle->mtx);
		return (1);
	}
	dongle->taken = true;
	pthread_mutex_unlock(&dongle->mtx);
	log_event(coder->data_table, coder->coder_id + 1, "has taken a dongle\n");
	pthread_cond_broadcast(&dongle->cond);
	return (0);
}

int	acquire_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mtx);
	if (new_request(coder, dongle->heap))
	{
		pthread_mutex_unlock(&dongle->mtx);
		return (1);
	}
	wait_for_dongle(coder, dongle);
	if (!is_running(coder->data_table))
	{
		pthread_mutex_unlock(&dongle->mtx);
		return (0);
	}
	return (get_dongle(coder, dongle));
}

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mtx);
	dongle->taken = false;
	dongle->release_time_in_ms = get_time(MILLISECOND);
	pthread_mutex_unlock(&dongle->mtx);
	pthread_cond_broadcast(&dongle->cond);
}
