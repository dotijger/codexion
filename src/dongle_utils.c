/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   dongle_utils.c                                  \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/14 12:52:10 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/18 11:24:53 by odschreu            ..+::##::+..         */
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

bool	my_turn(t_heap *heap, t_coder *coder, t_coder *rival)
{
	int		coder_idx;
	int		rival_idx;

	coder_idx = get_heap_index(heap, coder->coder_id);
	rival_idx = get_heap_index(heap, rival->coder_id);
	if (rival_idx == -1 || coder_idx == 0)
		return (true);
	return (heap->cmp(heap->queue[coder_idx], heap->queue[rival_idx]));
}

void	new_request(t_coder *coder, t_heap *heap)
{
	t_request	request;

	request.id = coder->coder_id;
	request.arrival_time = get_time(MILLISECOND);
	request.deadline_time = coder->burnout_deadline;
	insert(heap, request);	
}

