/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclaereb <tclaereb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 09:50:04 by Théo              #+#    #+#             */
/*   Updated: 2024/10/13 12:59:21 by tclaereb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Manage args
// Create structure to store philo and philo params

t_helper	*parse_param(char **argv)
{
	t_helper	*helper;

	helper = malloc(sizeof(helper));
	if (!helper)
		return (raise_error("malloc",
				"allocation failed for the helper struct."), NULL);
	helper->philo_count = ft_atoi(argv[0]);
	helper->time_to_die = ft_atoi(argv[1]);
	helper->time_to_eat = ft_atoi(argv[2]);
	helper->time_to_sleep = ft_atoi(argv[3]);
	helper->meal_count = 0;
	if (argv[4])
		helper->meal_count = ft_atoi(argv[4]);
	if (helper->philo_count == -1 || helper->time_to_die == -1
		|| helper->time_to_eat == -1 || helper->philo_count == -1
		|| helper->philo_count == -1)
		return (free(helper), NULL);
	return (helper);
}

int	main(int argc, char **argv)
{
	t_helper	*helper;

	if (argc != 5 && argc != 6)
		return (raise_error("argc", "number of argument is not equal to 5."),
			1);
	helper = parse_param(++argv);
	if (!helper)
		return (1);
	free(helper);
	return (0);
}
