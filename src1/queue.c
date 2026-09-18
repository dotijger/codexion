/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   queue.c                                         \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/14 16:37:21 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/15 09:11:18 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	swap(t_node **a, t_node **b)
{
	t_node	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	add_front(t_queue *queue, t_node *a)
{
	t_node	*tmp;

	tmp = queue->head->next;
	queue->head->next = a;
	a->next = tmp;
}

void	init_queue(t_queue *queue, int (*cmp)(t_request, t_request))
{
	t_node	*head;

	head->next = NULL;
	queue->head = head;
	queue->size = 0;
	queue->cmp = cmp;
}

void	enqueue(t_queue *queue, t_node *a)
{
	t_node	*current;
	t_node	*tmp;

	current = queue->head->next;
	if (!queue.cmp(current.request, a.request))
	{
		tmp = current;
		current = a;
		a->next = tmp;
		queue->head->next = a;
	}
	while (current->next != NULL)
	{
		
		if (!queue.cmp(current->next.request, a.request))
			


		current = current->next;
	}
}
