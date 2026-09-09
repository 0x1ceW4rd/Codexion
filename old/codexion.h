#ifndef CODEXION_H
#define CODEXION_H

typedef struct t_data t_data;
typedef struct t_coder t_coder;

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

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

typedef struct t_queue
{
    struct t_coder** coders;
    size_t size;
    size_t capacity;
    pthread_mutex_t queue_mutex; 
}t_queue;

typedef struct t_dongle
{
    pthread_mutex_t lock;
    long long available_at;
    pthread_cond_t sleep;
}t_dongle;

typedef struct t_coder
{
    pthread_t thread;
    size_t coder_ID;
    size_t com_compiles;
    long long last_compile_start;
    t_dongle *left_dongle;
    t_dongle *right_dongle;
    struct t_data *data;
}t_coder;

typedef struct t_data
{
    struct t_dongle* dongles;
    struct t_coder* coders;
    struct t_config* config;
    pthread_mutex_t write_mutex;
    size_t is_over;
    pthread_mutex_t sim_mutex;
    long long start_time;
    pthread_t monitor;
    t_queue queue;
}t_data;

size_t	ft_atoi(const char *str);
int	ft_isdigit(int c);
char	*ft_itoa(size_t n);
char	*ft_strdup(const char *src);
int	ft_strlen(const char *s);
t_config parser(char **av);
int	ft_strcmp(const char *s1, const char *s2);


#endif