/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezzirar <aezzirar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 13:41:49 by aezzirar          #+#    #+#             */
/*   Updated: 2026/09/09 15:55:37 by aezzirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	init_dongles(t_sim *sim)
{
	int	i;

	sim->dongles = (t_dongle *)malloc(sizeof(t_dongle) * sim->num_coders);
	if (!sim->dongles)
		return (0);
	i = 0;
	while (i < sim->num_coders)
	{
		sim->dongles[i].id = i;
		sim->dongles[i].is_in_use = 0;
		sim->dongles[i].cooldown_until = 0;
		if (!heap_init(&sim->dongles[i].queue, sim->num_coders + 2))
			return (0);
		i++;
	}
	return (1);
}

static void	init_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->num_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].compiles_count = 0;
		sim->coders[i].last_compile_start = 0;
		sim->coders[i].sim = sim;
		sim->coders[i].left_dongle = &sim->dongles[i];
		if (sim->num_coders > 1)
			sim->coders[i].right_dongle = &sim->dongles[(i + 1)
				% sim->num_coders];
		else
			sim->coders[i].right_dongle = NULL;
		i++;
	}
}

int	init_simulation(t_sim *sim)
{
	if (!init_dongles(sim))
		return (0);
	sim->coders = (t_coder *)malloc(sizeof(t_coder) * sim->num_coders);
	if (!sim->coders)
		return (0);
	init_coders(sim);
	if (pthread_mutex_init(&sim->state_mutex, NULL) != 0)
		return (0);
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
		return (0);
	if (pthread_cond_init(&sim->state_cond, NULL) != 0)
		return (0);
	sim->stop_flag = 0;
	sim->finished_coders = 0;
	sim->req_seq = 0;
	return (1);
}

void	clean_simulation(t_sim *sim)
{
	int	i;

	if (sim->dongles)
	{
		i = 0;
		while (i < sim->num_coders)
		{
			heap_free(&sim->dongles[i].queue);
			i++;
		}
		free(sim->dongles);
		sim->dongles = NULL;
	}
	if (sim->coders)
	{
		free(sim->coders);
		sim->coders = NULL;
	}
	pthread_mutex_destroy(&sim->state_mutex);
	pthread_mutex_destroy(&sim->print_mutex);
	pthread_cond_destroy(&sim->state_cond);
}