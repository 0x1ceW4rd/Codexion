#ifndef CODEXION_H
#define CODEXION_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct t_config
{
    size_t number_of_coders;
    size_t time_to_burnout;
    size_t time_to_compile;
    size_t time_to_debug;
    size_t time_to_refactor;
    size_t number_of_compiles_required;
    size_t dongle_cooldown;
    char *scheduler;
}t_config;


size_t	ft_atoi(const char *str);
int	ft_isdigit(int c);
char	*ft_itoa(size_t n);
char	*ft_strdup(const char *src);
int	ft_strlen(const char *s);
t_config parser(char **av);
int	ft_strcmp(const char *s1, const char *s2);


#endif