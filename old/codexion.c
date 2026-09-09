/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezzirar <aezzirar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 09:23:50 by aezzirar          #+#    #+#             */
/*   Updated: 2026/08/20 10:22:16 by aezzirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

int main(int ac, char **av)
{
    if (ac == 9)
    {
        int i = 1;
        while (i < 8)
        {
            int k = 0;
            if (av[i][k] == '\0')
            {
                char *f = ft_itoa(i);
                write(2, "the argument number '", 21);
                write(2, f, ft_strlen(f));
                free(f);
                write(2, "' is not a positive integer\n", 28);
                return 1;
            }
            while (av[i][k] != '\0')
            {
                if (!ft_isdigit(av[i][k]))
                {
                    char *f = ft_itoa(i);
                    write(2, "the argument number '", 21);
                    write(2, f, ft_strlen(f));
                    free(f);
                    write(2, "' is not a positive integer\n", 28);
                    return 1;
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
        return 1;
    }
    return 0;
}
