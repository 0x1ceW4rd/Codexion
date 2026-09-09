/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezzirar <aezzirar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 13:41:36 by aezzirar          #+#    #+#             */
/*   Updated: 2026/09/09 20:38:49 by aezzirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_init(t_heap *heap, int capacity)
{
	heap->capacity = capacity;
	heap->size = 0;
	heap->data = (t_request *)malloc(sizeof(t_request) * capacity);
	if (!heap->data)
		return (0);
	return (1);
}

void	heap_free(t_heap *heap)
{
	if (heap->data)
	{
		free(heap->data);
		heap->data = NULL;
	}
	heap->size = 0;
	heap->capacity = 0;
}

int	is_higher_priority(t_request a, t_request b, int scheduler)
{
	if (scheduler == POLICY_FIFO)
	{
		if (a.seq != b.seq)
			return (a.seq < b.seq);
		return (a.coder_id < b.coder_id);
	}
	if (a.deadline != b.deadline)
		return (a.deadline < b.deadline);
	if (a.seq != b.seq)
		return (a.seq < b.seq);
	return (a.coder_id < b.coder_id);
}

void	sift_up(t_heap *heap, int idx, int scheduler)
{
	int			parent;
	t_request	tmp;

	while (idx > 0)
	{
		parent = (idx - 1) / 2;
		if (is_higher_priority(heap->data[idx], heap->data[parent], scheduler))
		{
			tmp = heap->data[idx];
			heap->data[idx] = heap->data[parent];
			heap->data[parent] = tmp;
			idx = parent;
		}
		else
			break ;
	}
}

void	sift_down(t_heap *heap, int idx, int scheduler)
{
	int			smallest;
	int			left;
	int			right;
	t_request	tmp;

	while (1)
	{
		smallest = idx;
		left = 2 * idx + 1;
		right = 2 * idx + 2;
		if (left < heap->size && is_higher_priority(heap->data[left],
				heap->data[smallest], scheduler))
			smallest = left;
		if (right < heap->size && is_higher_priority(heap->data[right],
				heap->data[smallest], scheduler))
			smallest = right;
		if (smallest == idx)
			break ;
		tmp = heap->data[idx];
		heap->data[idx] = heap->data[smallest];
		heap->data[smallest] = tmp;
		idx = smallest;
	}
}
