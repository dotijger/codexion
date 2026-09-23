/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   heap_utils.c                                    \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/21 18:07:23 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/23 14:09:57 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	swap(t_request *a, t_request *b)
{
	t_request	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	heapify(t_heap *heap, int i)
{
	int	parent;
	int	left_child;
	int	right_child;

	parent = i;
	left_child = 2 * i + 1;
	right_child = 2 * i + 2;
	if (left_child < heap->size
		&& heap->cmp(heap->queue[left_child], heap->queue[parent]))
		parent = left_child;
	if (right_child < heap->size
		&& heap->cmp(heap->queue[right_child], heap->queue[parent]))
		parent = right_child;
	if (parent != i)
	{
		swap(&heap->queue[i], &heap->queue[parent]);
		heapify(heap, parent);
	}
}
