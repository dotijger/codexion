/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   dongle_utils.c                                  \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/14 12:52:10 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/23 13:58:15 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	dongle_ready(t_dongle *a, long cooldown_ms)
{
	if (a->taken)
		return (false);
	if (a->release_time_in_ms == 0)
		return (true);
	return (get_time(MILLISECOND) >= (a->release_time_in_ms + cooldown_ms));
}

long	available_at(t_dongle *a, long cooldown_ms)
{
	return (a->release_time_in_ms + cooldown_ms);
}

t_coder	*get_rival(t_coder *coder, t_dongle *a)
{
	if (a == coder->left)
		return (coder->left_rival);
	return (coder->right_rival);
}

bool	my_turn(t_heap *heap, t_coder *coder, t_dongle *a)
{
	int		coder_idx;
	int		rival_idx;
	t_coder	*rival;

	rival = get_rival(coder, a);
	coder_idx = get_heap_index(heap, coder->coder_id);
	rival_idx = get_heap_index(heap, rival->coder_id);
	if (rival_idx == -1 || coder_idx == 0)
		return (true);
	return (heap->cmp(heap->queue[coder_idx], heap->queue[rival_idx]));
}

int	new_request(t_coder *coder, t_heap *heap)
{
	t_request	request;

	request.coder_id = coder->coder_id;
	request.arrival_time = get_time(MILLISECOND);
	pthread_mutex_lock(&coder->data_table->table_mtx);
	request.deadline_time = coder->burnout_deadline;
	pthread_mutex_unlock(&coder->data_table->table_mtx);
	if (insert(heap, request))
		return (1);
	return (0);
}
