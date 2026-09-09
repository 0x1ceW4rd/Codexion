/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezzirar <aezzirar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 13:42:28 by aezzirar          #+#    #+#             */
/*   Updated: 2026/09/09 20:31:52 by aezzirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h" 

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (((long long)tv.tv_sec * 1000) + ((long long)tv.tv_usec / 1000));
}

long long	get_sim_time(long long start_time)
{
	return (get_time_ms() - start_time);
}

void	get_abs_timespec(struct timespec *ts, long long ms_from_now)
{
	struct timeval	tv;
	long long		total_usec;

	gettimeofday(&tv, NULL);
	total_usec = tv.tv_usec + (ms_from_now % 1000) * 1000;
	ts->tv_sec = tv.tv_sec + (ms_from_now / 1000) + (total_usec / 1000000);
	ts->tv_nsec = (total_usec % 1000000) * 1000;
}

void	precise_sleep(long long ms, t_sim *sim)
{
	long long	start;

	start = get_time_ms();
	if (ms > 10)
		usleep((ms - 10) * 1000);
	while (!is_sim_stopped(sim))
	{
		if (get_time_ms() - start >= ms)
			break ;
		usleep(1000);
	}
}
