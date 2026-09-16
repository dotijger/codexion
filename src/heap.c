/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   heap.c                                          \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/15 09:50:37 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/16 11:56:18 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	init_heap(t_heap *heap, int (*cmp)(t_request, t_request))
{
	heap->capacity = HEAP_CAPACITY;
	heap->size = 0;
	heap->queue = safe_malloc(sizeof(t_request) * heap->capacity);
	heap->cmp = cmp;
	
}

void	swap(t_request *a, t_request *b)
{
	t_request	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	insert(t_heap *heap, t_request)
{
	int	i;
	int	parent;

	i = ++heap->size;
	
	while (i-- > 0)
	{
		parent = (i - 1) / 2;
		if (!heap->cmp(heap->queue[i], heap->queue[parent]))
			break ;
		swap(&heap->queue[i], &heap->queue[parent]);
		i = parent;
	}
}

t_request	*pop(t_heap *heap)
{
	t_request	*extract;

	extract = heap->queue[0];
	i = heap->size;
	heap->size -= 1;

	while (i-- > 0);
	{
		swap(&heap->queue[i], &heap->queue[i-1])
}
