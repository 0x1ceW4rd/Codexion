/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezzirar <aezzirar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 14:05:37 by aezzirar          #+#    #+#             */
/*   Updated: 2026/09/04 00:46:03 by aezzirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void initializer(t_data *data, t_config *config)
{
    data->config = &config;
    data->dongles = malloc(config->number_of_coders * sizeof(t_dongle));
    data->coders = malloc(config->number_of_coders * sizeof(t_coder));
    if (!data->dongles || !data->coders)
    {
        free(data->dongles);
        free(data->coders);
        write(1, "couldn't alocate memory for dongles nor coders!", ft_strlen("couldn't alocate memory for dongles nor coders!"));
        exit(1);
    }
    pthread_mutex_init(&data->write_mutex, NULL);
    pthread_mutex_init(&data->sim_mutex, NULL);
    data->is_over = 0;
    data->start_time = 0;
    
    int i = 0;
    
    while(i < data->config->number_of_coders - 1)
    {
        pthread_mutex_init(&data->dongles[i].lock, NULL);
        pthread_cond_init(&data->dongles[i].sleep, NULL);
        data->dongles[i].available_at = 0;
    }
}