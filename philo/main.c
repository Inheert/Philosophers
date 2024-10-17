/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Théo <theoclaereboudt@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 09:50:04 by Théo              #+#    #+#             */
/*   Updated: 2024/10/17 13:31:38 by Théo             ###   ########.fr       */
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
