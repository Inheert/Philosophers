/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclaereb <tclaereb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 09:50:04 by Théo              #+#    #+#             */
/*   Updated: 2024/10/16 12:46:00 by tclaereb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	main(int argc, char **argv)
{
	t_philosopher	*philosophers;

	actual_time();
	if (argc != 5 && argc != 6)
		return (raise_error("argc",
				"number of argument is not equal to 5 or 6."), 1);
	philosophers = prepare_philosophers(argv);
	if (!philosophers)
		return (1);
	start_threads(philosophers, philosophers->helper);
	free_t_shared(philosophers->shared);
	free_t_philosopher(philosophers, philosophers->helper->philo_count);
	return (0);
}
