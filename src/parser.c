/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezzirar <aezzirar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 10:57:05 by aezzirar          #+#    #+#             */
/*   Updated: 2026/08/20 14:38:36 by aezzirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int iszero(char *name, size_t n)
{
    int len = ft_strlen(name);
    if (n == 0)
    {  
        write(2, name, len);
        write(2, " must be a positive integer\n", 28);
        return 1;
    }
    return 0;
}

static int isscheduler(char* name)
{
    char *edf = "edf";
    char *fifo = "fifo";
    if (ft_strcmp(name, fifo) && ft_strcmp(name, edf))
    {
        write(2, "The scheduler can either be 'edf' or 'fifo'.\n", 46);
        return 1;
    }
    return 0;
}

t_config parser(char **av)
{   
    t_config settings;
    settings.number_of_coders = ft_atoi(av[1]);
    settings.time_to_burnout = ft_atoi(av[2]);
    settings.time_to_compile = ft_atoi(av[3]);
    settings.time_to_debug = ft_atoi(av[4]);
    settings.time_to_refactor = ft_atoi(av[5]);
    settings.number_of_compiles_required = ft_atoi(av[6]);
    settings.dongle_cooldown = ft_atoi(av[7]);
    settings.scheduler = av[8];
    
    if (iszero("number_of_coders", settings.number_of_coders) || iszero("time_to_burnout", settings.time_to_burnout) ||iszero("time_to_compile", settings.time_to_compile) || iszero("time_to_debug", settings.time_to_debug) || iszero("time_to_refactor", settings.time_to_refactor) || iszero("number_of_compiles_required", settings.number_of_compiles_required) || iszero("dongle_cooldown", settings.dongle_cooldown) || isscheduler(settings.scheduler))
        exit(1);
    return (settings);
}
