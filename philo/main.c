/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclaereb <tclaereb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 09:50:04 by Théo              #+#    #+#             */
/*   Updated: 2024/10/13 14:09:36 by tclaereb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// Manage args
// Create structure to store philo and philo params

int	main(int argc, char **argv)
{
	t_philosopher	*philosophers;

	if (argc != 5 && argc != 6)
		return (raise_error("argc", "number of argument is not equal to 5 or 6."),
			1);
	philosophers = prepare_philosophers(argv);
	if (!philosophers)
		return (1);
	free(philosophers->helper);
	free(philosophers);
	return (0);
}
