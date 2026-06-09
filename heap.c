/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csilva <csilva@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 11:34:55 by csilva            #+#    #+#             */
/*   Updated: 2026/06/09 11:26:17 by csilva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"

void	swap(t_heap_node *a, t_heap_node *b);

int	priority(t_heap_node *a, t_heap_node *b, t_scheduler sched)
{
	if (sched == FIFO)
		return (a->request_time < b->request_time);
	if (a->deadline == b->deadline)
		return (a->coder_id < b->coder_id);
	return (a->deadline < b->deadline);
}

void	heap_push(t_heap *heap, t_heap_node node, t_scheduler sched)
{
	int	i;
	int	parent;

	heap->data[heap->size] = node;
	i = heap->size;
	heap->size++;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!priority(&heap->data[i], &heap->data[parent], sched))
			break ;
		swap(&heap->data[i], &heap->data[parent]);
		i = parent;
	}
}

int	best_child(t_heap *heap, int i, t_scheduler sched)
{
	int	left;
	int	right;
	int	best;

	left = 2 * i + 1;
	right = 2 * i + 2;
	best = i;
	if (left < heap->size
		&& priority(&heap->data[left], &heap->data[best], sched))
		best = left;
	if (right < heap->size
		&& priority(&heap->data[right], &heap->data[best], sched))
		best = right;
	return (best);
}

t_heap_node	heap_pop(t_heap *heap, t_scheduler sched)
{
	t_heap_node	top;
	int			i;
	int			best;

	top = heap->data[0];
	heap->size--;
	heap->data[0] = heap->data[heap->size];
	i = 0;
	while (1)
	{
		best = best_child(heap, i, sched);
		if (best == i)
			break ;
		swap(&heap->data[i], &heap->data[best]);
		i = best;
	}
	return (top);
}

void	swap(t_heap_node *a, t_heap_node *b)
{
	t_heap_node	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}
