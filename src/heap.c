/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   heap.c                                          \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/15 09:50:37 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/17 16:25:52 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	swap(t_request *a, t_request *b)
{
	t_request	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	heapify(t_heap *heap, int i)
{
	int	parent;
	int	left_child;
	int	right_child;

	parent = i;
	left_child = 2 * i + 1;
	right_child = 2 * i + 2;

	if (left_child < heap->size &&
			heap->cmp(heap->queue[left_child], heap->queue[parent]))
		parent = left_child;
	if (right_child < heap->size &&
			heap->cmp(heap->queue[right_child], heap->queue[parent]))
		parent = right_child;
	if (parent != i)
	{
		swap(&heap->queue[i], &heap->array[parent]);
		heapify(heap, parent);
	}
}

void init_heap(t_heap **heap, int capacity, int (*cmp)(t_request, t_request))
{
	*heap = safe_malloc(sizeof(t_heap)); 
	(*heap)->capacity = capacity;
	(*heap)->size = 0;
	(*heap)->queue = safe_malloc(sizeof(t_request) * heap->capacity);
	(*heap)->cmp = cmp;
}

int	get_heap_index(t_heap *heap, int coder_id)
{
	int	i;
	
	i = -1;
	while (++i < heap->size)
	{
		if (heap->queue[i].id == coder_id)
			break ;
	}
	return (i);
}

void	insert(t_heap *heap, t_request request)
{
	if (heap->size == heap->capacity)
		error_exit("Heap overflow.");
	
	int	i;
	i = heap->size++;
	heap->queue[i] = request;

	while (i != 0 && !heap->cmp(heap->queue[(i - 1) / 2], heap->queue[i]))
	{
		swap(&heap->queue[i], &heap->queue[(i - 1) / 2]);
		i = (i - 1) / 2;
	}
}

t_request	*pop(t_heap *heap)
{
	t_request	*root;

	if (heap->size <= 0)
		error_exit("Cannot extract from empty heap.");
	root = heap->queue[0];
	if (heap->size == 1)
	{
		heap->size--;
		return root;
	}
	heap->queue[0] = heap->queue[--heap->size];
	heapify(heap, 0);
	return root;
}

void	remove_at_index(t_heap *heap, int index)
{
	if (index >= heap->size)
		error_exit("Cannot remove node from heap at invalid index.");
	heap->queue[index] = heap->queue[heap->size - 1];
	heap->size--;
	heapify(heap, index);
}

