/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csilva <csilva@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/05 15:55:10 by csilva            #+#    #+#             */
/*   Updated: 2026/06/08 16:12:02 by csilva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"

void	dongle_take(t_dongle *dongle, t_coder *coder)
{
	t_heap_node	node;

	pthread_mutex_lock(&dongle->mutex);
	node.coder_id = coder->coder_id;
	node.request_time = get_time_ms();
	node.deadline = coder->last_compile + coder->config->time_to_burnout;
	heap_push(&dongle->queue, node, coder->sim->config.scheduler);
	while (is_running(coder->sim) && dongle_blocked(dongle, coder))
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
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
