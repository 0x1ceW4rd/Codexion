/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezzirar <aezzirar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 13:41:07 by aezzirar          #+#    #+#             */
/*   Updated: 2026/09/09 14:18:51 by aezzirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	enqueue_requests(t_coder *coder)
{
	t_request	req;

	req.coder_id = coder->id;
	req.seq = ++coder->sim->req_seq;
	req.deadline = coder->last_compile_start + coder->sim->time_to_burnout;
	heap_push(&coder->left_dongle->queue, req, coder->sim->scheduler);
	heap_push(&coder->right_dongle->queue, req, coder->sim->scheduler);
}

static int	acquire_dongles(t_coder *coder)
{
	t_sim		*sim;
	long long	now;

	sim = coder->sim;
	pthread_mutex_lock(&sim->state_mutex);
	enqueue_requests(coder);
	while (!sim->stop_flag)
	{
		now = get_sim_time(sim->start_time);
		if (can_take_dongles(coder, now))
		{
			coder->left_dongle->is_in_use = 1;
			coder->right_dongle->is_in_use = 1;
			heap_remove(&coder->left_dongle->queue, coder->id, sim->scheduler);
			heap_remove(&coder->right_dongle->queue, coder->id, sim->scheduler);
			coder->last_compile_start = now;
			pthread_mutex_unlock(&sim->state_mutex);
			return (1);
		}
		wait_for_turn(coder);
	}
	pthread_mutex_unlock(&sim->state_mutex);
	return (0);
}

static void	release_dongles(t_coder *coder)
{
	t_sim		*sim;
	long long	finish_time;

	sim = coder->sim;
	pthread_mutex_lock(&sim->state_mutex);
	finish_time = get_sim_time(sim->start_time);
	coder->left_dongle->is_in_use = 0;
	coder->left_dongle->cooldown_until = finish_time + sim->dongle_cooldown;
	coder->right_dongle->is_in_use = 0;
	coder->right_dongle->cooldown_until = finish_time + sim->dongle_cooldown;
	coder->compiles_count++;
	if (sim->num_compiles_required > 0
		&& coder->compiles_count == sim->num_compiles_required)
	{
		sim->finished_coders++;
		if (sim->finished_coders == sim->num_coders)
			sim->stop_flag = 1;
	}
	pthread_cond_broadcast(&sim->state_cond);
	pthread_mutex_unlock(&sim->state_mutex);
}

static void	*handle_single_coder(t_coder *coder)
{
	print_log(coder->sim, coder->id, "has taken a dongle");
	pthread_mutex_lock(&coder->sim->state_mutex);
	while (!coder->sim->stop_flag)
		pthread_cond_wait(&coder->sim->state_cond, &coder->sim->state_mutex);
	pthread_mutex_unlock(&coder->sim->state_mutex);
	return (NULL);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_sim	*sim;

	coder = (t_coder *)arg;
	sim = coder->sim;
	if (sim->num_coders == 1)
		return (handle_single_coder(coder));
	if (coder->id % 2 == 0)
		precise_sleep(sim->time_to_compile / 2, sim);
	while (!is_sim_stopped(sim))
	{
		if (!acquire_dongles(coder))
			break ;
		print_log(sim, coder->id, "has taken a dongle");
		print_log(sim, coder->id, "has taken a dongle");
		print_log(sim, coder->id, "is compiling");
		precise_sleep(sim->time_to_compile, sim);
		release_dongles(coder);
		if (is_sim_stopped(sim))
			break ;
		print_log(sim, coder->id, "is debugging");
		precise_sleep(sim->time_to_debug, sim);
		if (is_sim_stopped(sim))
			break ;
		print_log(sim, coder->id, "is refactoring");
		precise_sleep(sim->time_to_refactor, sim);
	}
	return (NULL);
}