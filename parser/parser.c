/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csilva <csilva@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 11:24:25 by csilva            #+#    #+#             */
/*   Updated: 2026/05/21 15:44:35 by csilva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

t_config	*parser(char **argv)
{
	t_config	*config;

	config = malloc(sizeof(t_config));
	if (!config)
		return (NULL);
	config->number_of_coders = int_parser(argv[1]);
	config->time_to_burnout = int_parser(argv[2]);
	config->time_to_compile = int_parser(argv[3]);
	config->time_to_debug = int_parser(argv[4]);
	config->time_to_refactor = int_parser(argv[5]);
	config->number_of_compiles_required = int_parser(argv[6]);
	config->dongle_cooldown = int_parser(argv[7]);
	config->scheduler = scheduler_parser(argv[8]);
	if (!config_parser(config))
	{
		free_all(config);
		return (NULL);
	}
	return (config);
}
