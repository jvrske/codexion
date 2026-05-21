/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csilva <csilva@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 11:24:37 by csilva            #+#    #+#             */
/*   Updated: 2026/05/21 16:27:13 by csilva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_H
# define INCLUDES_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

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

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				held;
	long long		release_time;
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
void		free_all(void *config);

#endif