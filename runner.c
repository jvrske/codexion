/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   runner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csilva <csilva@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 15:38:56 by csilva            #+#    #+#             */
/*   Updated: 2026/06/09 10:58:20 by csilva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"

void	run_sim(t_sim *sim)
{
	int	i;
	int	ret;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		ret = pthread_create(&sim->threads[i], NULL, coder_routine,
				&sim->coders[i]);
		if (ret != 0)
			printf("pthread_create failed: %d\n", ret);
		i++;
	}
	pthread_create(&sim->threads[sim->config.number_of_coders],
		NULL, monitor_routine, sim);
	i = 0;
	while (i < sim->config.number_of_coders + 1)
	{
		pthread_join(sim->threads[i], NULL);
		i++;
	}
}
