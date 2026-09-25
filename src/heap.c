/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   heap.c                                          \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/15 09:50:37 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/24 12:26:02 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_heap(t_heap **heap, bool (*cmp)(t_request, t_request))
{
	*heap = malloc(sizeof(t_heap));
	if (!*heap)
		return (fail("Malloc failed during creation of *heap.\n"));
	(*heap)->capacity = 2;
	(*heap)->size = 0;
	(*heap)->queue = malloc(sizeof(t_request) * (*heap)->capacity);
	if (!(*heap)->queue)
		return (fail("Malloc failed during creation of heap queue.\n"));
	(*heap)->cmp = cmp;
	return (0);
}

int	get_heap_index(t_heap *heap, int coder_id)
{
	int		i;
	bool	found;

	i = -1;
	found = false;
	while (++i < heap->size)
	{
		if (heap->queue[i].coder_id == coder_id)
		{
			found = true;
			break ;
		}
	}
	if (found)
		return (i);
	return (-1);
}

int	insert(t_heap *heap, t_request request)
{
	int	i;

	if (heap->size == heap->capacity)
		return (fail("Heap overflow."));
	i = heap->size++;
	heap->queue[i] = request;
	while (i != 0 && !heap->cmp(heap->queue[(i - 1) / 2], heap->queue[i]))
	{
		swap(&heap->queue[i], &heap->queue[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
	return (0);
}

int	remove_at_index(t_heap *heap, int index)
{
	if (index >= heap->size)
		return (fail("Cannot remove node from heap at invalid index."));
	heap->queue[index] = heap->queue[heap->size - 1];
	heap->size--;
	heapify(heap, index);
	return (0);
}

void	print_queue(t_heap *heap)
{
	int	i;

	i = -1;
	while (++i < heap->size)
		printf("position %d : coder %d\n", i, heap->queue[i].coder_id);
}
