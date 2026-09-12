/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezzirar <aezzirar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 13:42:06 by aezzirar          #+#    #+#             */
/*   Updated: 2026/09/12 15:17:59 by aezzirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long long	parse_long(const char *str, int *err)
{
	long long	res;
	int			i;

	res = 0;
	i = 0;
	if (!str || !str[0])
		*err = 1;
	while (!(*err) && str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			*err = 1;
		else
			res = res * 10 + (str[i++] - '0');
		if (res > 2147483647)
			*err = 1;
	}
	if (*err)
		return (-1);
	return (res);
}

static int	parse_sched(t_sim *sim, const char *arg, int *err)
{
	if (strcmp(arg, "fifo") == 0)
		sim->scheduler = POLICY_FIFO;
	else if (strcmp(arg, "edf") == 0)
		sim->scheduler = POLICY_EDF;
	else
		*err = 1;
	return (!(*err));
}

static int	parse_args(t_sim *sim, int argc, char **argv)
{
	int	err;

	err = 0;
	if (argc != 9)
		return (0);
	sim->num_coders = (int)parse_long(argv[1], &err);
	sim->time_to_burnout = parse_long(argv[2], &err);
	sim->time_to_compile = parse_long(argv[3], &err);
	sim->time_to_debug = parse_long(argv[4], &err);
	sim->time_to_refactor = parse_long(argv[5], &err);
	sim->num_compiles_required = (int)parse_long(argv[6], &err);
	sim->dongle_cooldown = parse_long(argv[7], &err);
	parse_sched(sim, argv[8], &err);
	if (err || sim->num_coders <= 0 || sim->time_to_burnout <= 0
		|| sim->num_compiles_required <= 0)
		return (0);
	return (1);
}

static int	start_threads(t_sim *sim)
{
	int	i;

	sim->start_time = get_time_ms();
	i = 0;
	while (i < sim->num_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_routine, &sim->coders[i]) != 0)
			return (0);
		i++;
	}
	if (pthread_create(&sim->monitor_thread, NULL,
			monitor_routine, sim) != 0)
		return (0);
	pthread_join(sim->monitor_thread, NULL);
	i = 0;
	while (i < sim->num_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_sim	sim;

	memset(&sim, 0, sizeof(t_sim));
	if (!parse_args(&sim, argc, argv))
	{
		write(2, "Error: Invalid arguments\n", 25);
		return (1);
	}
	if (!init_simulation(&sim))
	{
		clean_simulation(&sim);
		write(2, "Error: Simulation initialization failed\n", 40);
		return (1);
	}
	start_threads(&sim);
	clean_simulation(&sim);
	return (0);
}
