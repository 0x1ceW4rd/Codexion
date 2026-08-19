/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezzirar <aezzirar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 09:23:50 by aezzirar          #+#    #+#             */
/*   Updated: 2026/08/19 18:25:51 by aezzirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

int main(int ac, char **av)
{
    if (ac == 9)
    {
        int i = 1;
        while (i != 8)
        {
            int k = 0;
            while (av[i][k] != '\0')
            {
                if (!ft_isdigit(av[i][k]))
                {
                    write(1, "the argument number '", 21);
                    write(1, ft_itoa(i), 1);
                    write(1, "' is not a positive integer\n", 28);
                    return 0;
                }
                k++;
            }
            i++;
        }
        t_config settings = parser(av);
        write(1, settings.scheduler, ft_strlen(settings.scheduler));
        write(1, "\n", 1);
        
    }
    else
    {
        write(2, "all of the this data (number_of_coders, time_to_burnout, time_to_compile, time_to_debug, time_to_refactor, number_of_compiles_required, dongle_cooldown, and scheduler) is required for the program to work!\n", 206);    
    }
    return 0;
}
