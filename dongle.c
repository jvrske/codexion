/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csilva <csilva@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 15:55:10 by csilva            #+#    #+#             */
/*   Updated: 2026/06/09 10:32:44 by csilva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"
#include <sys/time.h>

void	dongle_take(t_dongle *dongle, t_coder *coder)
{
	t_heap_node	node;

	pthread_mutex_lock(&dongle->mutex);
	node.coder_id = coder->coder_id;
	node.request_time = get_time_ms();
	node.deadline = coder->last_compile + coder->config->time_to_burnout;
	heap_push(&dongle->queue, node, coder->sim->config.scheduler);
	while (is_running(coder->sim) && dongle_blocked(dongle, coder))
		timed_wait(dongle);
	heap_pop(&dongle->queue, coder->sim->config.scheduler);
	dongle->held = 1;
	log_state(coder->sim, coder->coder_id, "has taken a dongle");
	pthread_mutex_unlock(&dongle->mutex);
}

int	dongle_blocked(t_dongle *dongle, t_coder *coder)
{
	if (dongle->held == 1)
		return (1);
	if (get_time_ms() < dongle->release_time)
		return (1);
	if (dongle->queue.data[0].coder_id != coder->coder_id)
		return (1);
	return (0);
}

void	dongle_release(t_dongle *dongle, t_coder *coder)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->held = 0;
	dongle->release_time = get_time_ms() + coder->config->dongle_cooldown;
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
}

void	timed_wait(t_dongle *dongle)
{
	struct timeval	tv;
	struct timespec	ts;

	gettimeofday(&tv, NULL);
	ts.tv_sec = tv.tv_sec;
	ts.tv_nsec = (tv.tv_usec + 1000) * 1000;
	if (ts.tv_nsec >= 1000000000)
	{
		ts.tv_sec++;
		ts.tv_nsec -= 1000000000;
	}
	pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
}
