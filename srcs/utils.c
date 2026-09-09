/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezzirar <aezzirar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 13:42:51 by aezzirar          #+#    #+#             */
/*   Updated: 2026/09/09 16:44:42 by aezzirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	is_sim_stopped(t_sim *sim)
{
	int	stopped;

	pthread_mutex_lock(&sim->state_mutex);
	stopped = sim->stop_flag;
	pthread_mutex_unlock(&sim->state_mutex);
	return (stopped);
}

void	print_log(t_sim *sim, int coder_id, const char *msg)
{
	long long	now;

	pthread_mutex_lock(&sim->state_mutex);
	if (!sim->stop_flag)
	{
		pthread_mutex_lock(&sim->print_mutex);
		now = get_sim_time(sim->start_time);
		printf("%lld %d %s\n", now, coder_id, msg);
		pthread_mutex_unlock(&sim->print_mutex);
	}
	pthread_mutex_unlock(&sim->state_mutex);
}
