/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   dongle_utils_2.c                                \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/23 13:56:39 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/24 12:46:18 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
	pthread_mutex_lock(&first->mtx);
	if (new_request(coder, first->heap))
	{
		pthread_mutex_unlock(&first->mtx);
		return (1);
	}
	pthread_mutex_unlock(&first->mtx);
	pthread_mutex_lock(&second->mtx);
	if (new_request(coder, second->heap))
	{
		pthread_mutex_unlock(&first->mtx);
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
