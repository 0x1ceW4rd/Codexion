/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aezzirar <aezzirar@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 13:41:18 by aezzirar          #+#    #+#             */
/*   Updated: 2026/09/09 13:41:18 by aezzirar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>

# define POLICY_FIFO 0
# define POLICY_EDF 1

typedef struct s_request
{
	int			coder_id;
	long long	seq;
	long long	deadline;
}	t_request;

typedef struct s_heap
{
	t_request	*data;
	int			size;
	int			capacity;
}	t_heap;

typedef struct s_dongle
{
	int			id;
	int			is_in_use;
	long long	cooldown_until;
	t_heap		queue;
}	t_dongle;

struct	s_sim;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	int				compiles_count;
	long long		last_compile_start;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	struct s_sim	*sim;
}	t_coder;

typedef struct s_sim
{
	int				num_coders;
	long long		time_to_burnout;
	long long		time_to_compile;
	long long		time_to_debug;
	long long		time_to_refactor;
	int				num_compiles_required;
	long long		dongle_cooldown;
	int				scheduler;
	long long		start_time;
	long long		req_seq;
	int				stop_flag;
	int				finished_coders;
	pthread_mutex_t	state_mutex;
	pthread_mutex_t	print_mutex;
	pthread_cond_t	state_cond;
	pthread_t		monitor_thread;
	t_dongle		*dongles;
	t_coder			*coders;
}	t_sim;

/* Heap Functions */
int			heap_init(t_heap *heap, int capacity);
void		heap_free(t_heap *heap);
int			is_higher_priority(t_request a, t_request b, int scheduler);
void		sift_up(t_heap *heap, int idx, int scheduler);
void		sift_down(t_heap *heap, int idx, int scheduler);
int			heap_push(t_heap *heap, t_request req, int scheduler);
int			heap_peek(t_heap *heap, t_request *out);
void		heap_remove(t_heap *heap, int coder_id, int scheduler);

/* Timing and Helpers */
long long	get_time_ms(void);
long long	get_sim_time(long long start_time);
void		precise_sleep(long long ms, t_sim *sim);
void		get_abs_timespec(struct timespec *ts, long long ms_from_now);
int			is_sim_stopped(t_sim *sim);
void		print_log(t_sim *sim, int coder_id, const char *msg);

/* Coder Helpers */
int			can_take_dongles(t_coder *c, long long now);
long long	calc_wait_time(t_coder *c, long long now);
void		wait_for_turn(t_coder *c);

/* Simulation Setup and Routines */
int			init_simulation(t_sim *sim);
void		clean_simulation(t_sim *sim);
void		*coder_routine(void *arg);
void		*monitor_routine(void *arg);

#endif