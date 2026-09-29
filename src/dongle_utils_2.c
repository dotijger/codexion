/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   dongle_utils_2.c                                \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/23 13:56:39 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/29 12:14:49 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	min_wait_time(t_dongle *one, t_dongle *two, long cooldown)
{
	long	ret;

	ret = min_cooldown(available_at(one, cooldown),
			available_at(two, cooldown));
	return (ret);
}

void	assign_order(t_coder *coder, t_dongle **first, t_dongle **second)
{
	if (coder->coder_id % 2 == 0)
	{
		*first = coder->right;
		*second = coder->left;
	}
	else
	{
		*first = coder->left;
		*second = coder->right;
	}
}

int	enter_queue(t_coder *coder, t_dongle *first, t_dongle *second)
{
	long	arrival_time;

	arrival_time = get_time(MILLISECOND);
	pthread_mutex_lock(&first->mtx);
	if (new_request(coder, first->heap, arrival_time))
	{
		pthread_mutex_unlock(&first->mtx);
		return (1);
	}
	pthread_mutex_unlock(&first->mtx);
	pthread_mutex_lock(&second->mtx);
	if (new_request(coder, second->heap, arrival_time))
	{
		pthread_mutex_unlock(&second->mtx);
		return (1);
	}
	pthread_mutex_unlock(&second->mtx);
	return (0);
}

long	min_cooldown(long one, long two)
{
	if (one < two)
		return (one);
	return (two);
}
