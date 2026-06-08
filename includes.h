/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csilva <csilva@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 11:24:37 by csilva            #+#    #+#             */
/*   Updated: 2026/06/05 16:55:28 by csilva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_H
# define INCLUDES_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

typedef struct s_sim	t_sim;

typedef enum schedule
{
	FIFO,
	EDF,
	INVALID
}	t_scheduler;

typedef struct s_config
{
	int			number_of_coders;
	int			time_to_burnout;
	int			time_to_compile;
	int			time_to_debug;
	int			time_to_refactor;
	int			number_of_compiles_required;
	int			dongle_cooldown;
	t_scheduler	scheduler;
}	t_config;

typedef struct s_heap_node
{
	int			coder_id;
	long long	request_time;
	long long	deadline;
}	t_heap_node;

typedef struct s_heap
{
	t_heap_node	*data;
	int			size;
	int			capacity;
}	t_heap;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				held;
	long long		release_time;
	t_heap			queue;
}	t_dongle;

typedef struct s_coder
{
	int				coder_id;
	int				compiles_done;
	long long		last_compile;
	t_config		*config;
	t_sim			*sim;
}	t_coder;

typedef struct s_sim
{
	t_config		config;
	t_dongle		*dongles;
	t_coder			*coders;
	pthread_t		*threads;
	long long		start_time;
	int				running;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	state_mutex;
}	t_sim;

/* Parser */
int			int_validator(char *s);
int			int_parser(char *s);
int			config_parser(t_config *config);
t_config	*parser(char **argv);
t_scheduler	scheduler_parser(char *s);

/* Utils */
long long	get_time_ms(void);
void		log_state(t_sim *sim, int id, char *msg);
void		log_burnout(t_sim *sim, int id);
void		free_all(void *config);

/* Simulator */
int			init_dongles(t_sim *sim);
int			init_coders(t_sim *sim);
int			init_sim(t_sim *sim);
void		clean_sim(t_sim *sim);
void		clean_dongles(t_sim *sim);

/* Heap */
int			heap_init(t_heap *heap, int capacity);
int			priority(t_heap_node *a, t_heap_node *b, t_scheduler sched);
int			best_child(t_heap *heap, int i, t_scheduler sched);
void		heap_push(t_heap *heap, t_heap_node node, t_scheduler shed);
void		swap(t_heap_node *a, t_heap_node *b);
void		heap_free(t_heap *heap);
t_heap_node	heap_pop(t_heap *heap, t_scheduler sched);

/* Dongle */
int			dongle_blocked(t_dongle *dongle, t_coder *coder);
void		dongle_take(t_dongle *dongle, t_coder *coder);
void		dongle_release(t_dongle *dongle, t_coder *coder);

/* Coder */
int			is_running(t_sim *sim);
void		*coder_routine(void *arg);
void		coder_compile(t_coder *coder);

#endif