/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezzirar <aezzirar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 13:42:17 by aezzirar          #+#    #+#             */
/*   Updated: 2026/09/09 20:32:36 by aezzirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	check_coder_burnout(t_sim *sim)
{
	int			i;
	long long	now;

	now = get_sim_time(sim->start_time);
	i = 0;
	while (i < sim->num_coders)
	{
		if (sim->num_compiles_required > 0
			&& sim->coders[i].compiles_count >= sim->num_compiles_required)
		{
			i++;
			continue ;
		}
		if (now - sim->coders[i].last_compile_start >= sim->time_to_burnout)
		{
			sim->stop_flag = 1;
			pthread_mutex_lock(&sim->print_mutex);
			printf("%lld %d burned out\n", now, sim->coders[i].id);
			pthread_mutex_unlock(&sim->print_mutex);
			pthread_cond_broadcast(&sim->state_cond);
			return (1);
		}
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (1)
	{
		pthread_mutex_lock(&sim->state_mutex);
		if (sim->stop_flag)
		{
			pthread_mutex_unlock(&sim->state_mutex);
			break ;
		}
		if (check_coder_burnout(sim))
		{
			pthread_mutex_unlock(&sim->state_mutex);
			break ;
		}
		pthread_mutex_unlock(&sim->state_mutex);
		usleep(2000);
	}
	return (NULL);
}
