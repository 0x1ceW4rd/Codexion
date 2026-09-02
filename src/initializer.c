/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezzirar <aezzirar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/24 14:05:37 by aezzirar          #+#    #+#             */
/*   Updated: 2026/09/01 10:38:14 by aezzirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void initializer(t_config config)
{
    t_dongle** dongles = malloc(config.number_of_coders * sizeof(t_dongle));
    t_coder** coders = malloc(config.number_of_coders * sizeof(t_dongle));
    if (!dongles || !coders)
    {
        free(dongles);
        free(coders);
        write(1, "couldn't alocate memory for dongles nor coders!", ft_strlen("couldn't alocate memory for dongles nor coders!"));
        exit(1);
    }
    
}