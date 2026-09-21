/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   dongle.c                                        \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/21 17:58:46 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/21 17:59:16 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	acquire_dongle(t_coder *coder, t_dongle *dongle)
{
	struct timespec	time_ready;

	pthread_mutex_lock(&dongle->mtx);
	new_request(coder, dongle->heap);
	while (is_running(coder->data_table) && !(my_turn(dongle->heap, coder, dongle)
			&& dongle_ready(dongle, coder->data_table->dongle_cooldown)))
	{
		if (my_turn(dongle->heap, coder, dongle))
		{
			time_ready = ms_to_ts(available_at(dongle, coder->data_table->dongle_cooldown));
			pthread_cond_timedwait(&dongle->cond, &dongle->mtx, time_ready);
		}
		else
			pthread_cond_wait(&dongle->cond, &dongle->mtx);
	}
	if (!is_running(coder->data_table))
	{
		pthread_mutex_unlock(&dongle->mtx);
		return (0);
	}
	remove_at_index(dongle->heap, 0);
	dongle->taken = true;
	pthread_mutex_unlock(&dongle->mtx);
	log_event(coder->data_table, coder->coder_id + 1, "has taken a dongle\n");
	pthread_cond_broadcast(&dongle->cond);
	return (0);
}

void	release_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mtx);
	dongle->taken = false;
	dongle->release_time_in_ms = get_time(MILLISECOND);
	pthread_mutex_unlock(&dongle->mtx);
	pthread_cond_broadcast(&dongle->cond);
}
