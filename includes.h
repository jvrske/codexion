/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   includes.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csilva <csilva@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 11:24:37 by csilva            #+#    #+#             */
/*   Updated: 2026/05/21 15:46:54 by csilva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCLUDES_H
# define INCLUDES_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

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

typedef struct s_coder
{
	int			coder_id;
	int			compiles_done;
	long		last_compile;

	t_config	*config;
}	t_coder;

/* Parser */
int			int_validator(char *s);
int			int_parser(char *s);
int			config_parser(t_config *config);
t_config	*parser(char **argv);
t_scheduler	scheduler_parser(char *s);

/* Utils */
void		free_all(void *config);

#endif