/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: csilva <csilva@student.42lisboa.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/21 11:24:34 by csilva            #+#    #+#             */
/*   Updated: 2026/05/21 15:48:12 by csilva           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes.h"

int	main(int argc, char **argv)
{
	t_config	*config;

	if (argc != 9)
		return (fprintf(stderr, "Error. Correct usage: ./codexion "
				"<number_of_coders> <time_to_burnout> <time_to_compile> "
				"<time_to_debug> <time_to_refactor> "
				"<number_of_compiles_required> <dongle_cooldown> "
				"<scheduler>\n"), 1);
	config = parser(argv);
	(void)config;
	return (0);
}
