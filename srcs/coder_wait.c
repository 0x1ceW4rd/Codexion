/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_wait.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezzirar <aezzirar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 13:40:55 by aezzirar          #+#    #+#             */
/*   Updated: 2026/09/09 20:57:21 by aezzirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	can_take_dongles(t_coder *c, long long now)
{
	t_request	l_req;
	t_request	r_req;

	if (!heap_peek(&c->left_dongle->queue, &l_req)
		|| l_req.coder_id != c->id)
		return (0);
	if (!heap_peek(&c->right_dongle->queue, &r_req)
		|| r_req.coder_id != c->id)
		return (0);
	if (c->left_dongle->is_in_use
		|| now < c->left_dongle->cooldown_until)
		return (0);
	if (c->right_dongle->is_in_use
		|| now < c->right_dongle->cooldown_until)
		return (0);
	return (1);
}

long long	calc_wait_time(t_coder *c, long long now)
{
	t_request	l_req;
	t_request	r_req;
	long long	wait_until;

	wait_until = 0;
	if (heap_peek(&c->left_dongle->queue, &l_req) && l_req.coder_id == c->id
		&& heap_peek(&c->right_dongle->queue, &r_req)
		&& r_req.coder_id == c->id)
	{
		if (!c->left_dongle->is_in_use
			&& now < c->left_dongle->cooldown_until)
			wait_until = c->left_dongle->cooldown_until;
		if (!c->right_dongle->is_in_use
			&& now < c->right_dongle->cooldown_until)
		{
			if (wait_until == 0 || c->right_dongle->cooldown_until > wait_until)
				wait_until = c->right_dongle->cooldown_until;
		}
	}
	return (wait_until);
}

void	wait_for_turn(t_coder *c)
{
	long long	now;
	long long	wait_until;

	now = get_sim_time(c->sim->start_time);
	wait_until = calc_wait_time(c, now);
	if (wait_until > now)
	{
		pthread_mutex_unlock(&c->sim->state_mutex);
		usleep((wait_until - now) * 1000);
		pthread_mutex_lock(&c->sim->state_mutex);
	}
	else
		pthread_cond_wait(&c->sim->state_cond, &c->sim->state_mutex);
}

void	*handle_single_coder(t_coder *coder)
{
	print_log(coder->sim, coder->id, "has taken a dongle");
	pthread_mutex_lock(&coder->sim->state_mutex);
	while (!coder->sim->stop_flag)
		pthread_cond_wait(&coder->sim->state_cond, &coder->sim->state_mutex);
	pthread_mutex_unlock(&coder->sim->state_mutex);
	return (NULL);
}
