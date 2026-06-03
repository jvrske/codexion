/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csilva <csilva@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 12:23:32 by csilva            #+#    #+#             */
/*   Updated: 2026/06/03 17:48:11 by csilva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"

int	init_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	sim->dongles = malloc(sizeof(t_dongle) * sim->config.number_of_coders);
	if (!sim->dongles)
		return (-1);
	while (i < sim->config.number_of_coders)
	{
		sim->dongles[i].held = 0;
		sim->dongles[i].release_time = 0;
		pthread_mutex_init(&sim->dongles[i].mutex, NULL);
		pthread_cond_init(&sim->dongles[i].cond, NULL);
		i++;
	}
	return (0);
}

int	init_coders(t_sim *sim)
{
	int	i;

	i = 0;
	sim->coders = malloc(sizeof(t_coder) * sim->config.number_of_coders);
	if (!sim->coders)
		return (-1);
	while (i < sim->config.number_of_coders)
	{
		sim->coders[i].coder_id = i + 1;
		sim->coders[i].compiles_done = 0;
		sim->coders[i].last_compile = 0;
		sim->coders[i].config = &sim->config;
		sim->coders[i].sim = sim;
		i++;
	}
	return (0);
}

int	init_sim(t_sim *sim)
{
	pthread_mutex_init(&sim->print_mutex, NULL);
	pthread_mutex_init(&sim->state_mutex, NULL);
	if (init_dongles(sim))
		return (clean_sim(sim), -1);
	if (init_coders(sim))
		return (clean_sim(sim), -1);
	sim->threads = malloc(sizeof(pthread_t) * (sim->config.number_of_coders
				+ 1));
	if (!sim->threads)
		return (clean_sim(sim), -1);
	sim->running = 1;
	sim->start_time = get_time_ms();
	return (0);
}

void	clean_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_cond_destroy(&sim->dongles[i].cond);
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		i++;
	}
	free(sim->dongles);
	sim->dongles = NULL;
}

void	clean_sim(t_sim *sim)
{
	if (sim->threads)
	{
		free(sim->threads);
		sim->threads = NULL;
	}
	if (sim->coders)
	{
		free(sim->coders);
		sim->coders = NULL;
	}
	if (sim->dongles)
		clean_dongles(sim);
	pthread_mutex_destroy(&sim->state_mutex);
	pthread_mutex_destroy(&sim->print_mutex);
}
