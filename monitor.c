/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csilva <csilva@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 15:12:11 by csilva            #+#    #+#             */
/*   Updated: 2026/06/09 11:34:02 by csilva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"

void	*monitor_routine(void *arg)
{
	t_sim	*sim;
	int		i;

	sim = (t_sim *)arg;
	while (is_running(sim))
	{
		i = 0;
		while (i < sim->config.number_of_coders)
		{
			if (check_burnout(sim, &sim->coders[i]))
			{
				broadcast_all(sim);
				return (NULL);
			}
			i++;
		}
		if (all_compiled(sim))
		{
			broadcast_all(sim);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

int	check_burnout(t_sim *sim, t_coder *coder)
{
	long long	ref;

	pthread_mutex_lock(&sim->state_mutex);
	ref = coder->last_compile;
	pthread_mutex_unlock(&sim->state_mutex);
	if (ref == 0)
		ref = sim->start_time;
	if (get_time_ms() - ref > sim->config.time_to_burnout)
	{
		log_burnout(sim, coder->coder_id);
		pthread_mutex_lock(&sim->state_mutex);
		sim->running = 0;
		pthread_mutex_unlock(&sim->state_mutex);
		return (1);
	}
	return (0);
}

int	all_compiled(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_lock(&sim->state_mutex);
		if (sim->coders[i].compiles_done
			< sim->config.number_of_compiles_required)
		{
			pthread_mutex_unlock(&sim->state_mutex);
			return (0);
		}
		pthread_mutex_unlock(&sim->state_mutex);
		i++;
	}
	pthread_mutex_lock(&sim->state_mutex);
	sim->running = 0;
	pthread_mutex_unlock(&sim->state_mutex);
	return (1);
}

void	broadcast_all(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_cond_broadcast(&sim->dongles[i].cond);
		i++;
	}
}
