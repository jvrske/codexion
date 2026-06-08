/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csilva <csilva@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 16:37:02 by csilva            #+#    #+#             */
/*   Updated: 2026/06/08 16:13:42 by csilva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	printf("coder %d started\n", coder->coder_id);
	while (is_running(coder->sim))
	{
		coder_compile(coder);
		if (!is_running(coder->sim))
			break ;
		log_state(coder->sim, coder->coder_id, "is debugging");
		usleep(coder->config->time_to_debug * 1000);
		if (!is_running(coder->sim))
			break ;
		log_state(coder->sim, coder->coder_id, "is refactoring");
		usleep(coder->config->time_to_refactor * 1000);
	}
	return (NULL);
}

void	coder_compile(t_coder *coder)
{
	int	left;
	int	right;

	left = coder->coder_id -1;
	right = coder->coder_id % coder->sim->config.number_of_coders;
	if (coder->coder_id == coder->sim->config.number_of_coders)
	{
		dongle_take(&coder->sim->dongles[right], coder);
		dongle_take(&coder->sim->dongles[left], coder);
	}
	else
	{
		dongle_take(&coder->sim->dongles[left], coder);
		dongle_take(&coder->sim->dongles[right], coder);
	}
	pthread_mutex_lock(&coder->sim->state_mutex);
	coder->last_compile = get_time_ms();
	pthread_mutex_unlock(&coder->sim->state_mutex);
	log_state(coder->sim, coder->coder_id, "is compiling");
	usleep(coder->config->time_to_compile * 1000);
	dongle_release(&coder->sim->dongles[left], coder);
	dongle_release(&coder->sim->dongles[right], coder);
	pthread_mutex_lock(&coder->sim->state_mutex);
	coder->compiles_done++;
	pthread_mutex_unlock(&coder->sim->state_mutex);
}

int	is_running(t_sim *sim)
{
	int	running;

	pthread_mutex_lock(&sim->state_mutex);
	running = sim->running;
	pthread_mutex_unlock(&sim->state_mutex);
	return (running);
}
