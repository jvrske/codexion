/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csilva <csilva@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 15:42:16 by csilva            #+#    #+#             */
/*   Updated: 2026/06/05 15:46:30 by csilva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"

int	heap_init(t_heap *heap, int capacity)
{
	heap->data = malloc(sizeof(t_heap_node) * capacity);
	if (!heap->capacity)
		return (-1);
	heap->size = 0;
	heap->capacity = capacity;
	return (0);
}

void	heap_free(t_heap *heap)
{
	if (heap->data)
	{
		free(heap->data);
		heap->data = NULL;
	}
	heap->size = 0;
}
