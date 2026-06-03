/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csilva <csilva@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 14:55:04 by csilva            #+#    #+#             */
/*   Updated: 2026/06/02 11:24:50 by csilva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"
#include "sys/time.h"

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	log_state(t_sim *sim, int id, char *msg)
{
	long long	ts;

	pthread_mutex_lock(&sim->print_mutex);
	if (sim->running)
	{
		ts = get_time_ms() - sim->start_time;
		printf("%lld %d %s\n", ts, id, msg);
	}
	pthread_mutex_unlock(&sim->print_mutex);
}

void	log_burnout(t_sim *sim, int id)
{
	long long	ts;

	pthread_mutex_lock(&sim->print_mutex);
	ts = get_time_ms() - sim->start_time;
	printf("%lld %d burned out\n", ts, id);
	pthread_mutex_unlock(&sim->print_mutex);
}

void	free_all(void *config)
{
	if (config)
		free(config);
}
