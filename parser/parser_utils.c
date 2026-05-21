/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csilva <csilva@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 12:51:18 by csilva            #+#    #+#             */
/*   Updated: 2026/05/21 15:56:06 by csilva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes.h"

int	int_validator(char *s)
{
	int	i;

	i = 0;
	if (s[i] == '+')
		i++;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (i);
}

int	int_parser(char *s)
{
	int	valid_int;

	if (int_validator(s) == 0 || strlen(s) > 10)
		return (-1);
	valid_int = atoi(s);
	if (valid_int < 0)
		return (-1);
	return (valid_int);
}

t_scheduler	scheduler_parser(char *s)
{
	if (strcmp(s, "fifo") == 0)
		return (FIFO);
	else if (strcmp(s, "edf") == 0)
		return (EDF);
	return (INVALID);
}

int	config_parser(t_config *config)
{
	if (config->number_of_coders <= 0)
		return (fprintf(stderr, "Number of coders must be a valid integer\n"));
	if (config->time_to_burnout <= 0)
		return (fprintf(stderr, "Time to burnout must be a valid integer\n"));
	if (config->time_to_compile <= 0)
		return (fprintf(stderr, "Time to compile must be a valid integer\n"));
	if (config->time_to_debug <= 0)
		return (fprintf(stderr, "Time to debug must be a valid integer\n"));
	if (config->time_to_refactor <= 0)
		return (fprintf(stderr, "Time to refactor must be a valid integer\n"));
	if (config->number_of_compiles_required <= 0)
		return (fprintf(stderr, "Number of compiles required must be a "
				"valid integer\n"));
	if (config->dongle_cooldown < 0)
		return (fprintf(stderr, "Dongle cooldown must be a valid integer\n"));
	if (config->scheduler == INVALID)
		return (fprintf(stderr, "Scheduler format is invalid, "
				"must be FIFO or EDF\n"));
	return (1);
}
