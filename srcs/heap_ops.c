/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezzirar <aezzirar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 13:41:27 by aezzirar          #+#    #+#             */
/*   Updated: 2026/09/09 20:39:00 by aezzirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_push(t_heap *heap, t_request req, int scheduler)
{
	if (heap->size >= heap->capacity)
		return (0);
	heap->data[heap->size] = req;
	sift_up(heap, heap->size, scheduler);
	heap->size++;
	return (1);
}

int	heap_peek(t_heap *heap, t_request *out)
{
	if (heap->size == 0)
		return (0);
	*out = heap->data[0];
	return (1);
}

void	heap_remove(t_heap *heap, int coder_id, int scheduler)
{
	int	idx;
	int	i;

	idx = -1;
	i = 0;
	while (i < heap->size)
	{
		if (heap->data[i].coder_id == coder_id)
		{
			idx = i;
			break ;
		}
		i++;
	}
	if (idx == -1)
		return ;
	heap->data[idx] = heap->data[heap->size - 1];
	heap->size--;
	if (idx < heap->size)
	{
		sift_down(heap, idx, scheduler);
		sift_up(heap, idx, scheduler);
	}
}
