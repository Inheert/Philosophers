/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading_preparation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Théo <theoclaereboudt@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:19:35 by Théo              #+#    #+#             */
/*   Updated: 2024/10/14 21:22:18 by Théo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	start_threads(t_philosopher *philo, t_helper *helper)
{
	t_shared	*shared;
	int			i;

	shared = philo->shared;
	i = -1;
	while (++i < helper->philo_count)
	{
		if (pthread_create(&philo[i].thread, NULL, &main_thread,
				&philo[i]) != 0)
			return (raise_error("thread",
					"a problem occur while creating threads."), 1);
	}
	pthread_mutex_lock(&shared->check_death);
	shared->thread_ready = 1;
	pthread_mutex_unlock(&shared->check_death);
	i = -1;
	while (++i < helper->philo_count)
		pthread_join(philo[i].thread, NULL);
	return (0);
}
