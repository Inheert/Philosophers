/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading_preparation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Théo <theoclaereboudt@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:19:35 by Théo              #+#    #+#             */
/*   Updated: 2024/10/14 18:22:07 by Théo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	have_all_the_philos_finished_eating(t_philosopher *philo, t_helper *helper)
{
	int	i;

	if (helper->meal_count == 0)
		return (0);
	i = -1;
	while (++i < helper->philo_count)
	{
		pthread_mutex_lock(&philo->shared->check_meal);
		if (philo[i].eat_count < helper->meal_count)
			return (pthread_mutex_unlock(&philo->shared->check_meal), 0);
		pthread_mutex_unlock(&philo->shared->check_meal);
	}
	return (1);
}

int	start_threads(t_philosopher *philo, t_helper *helper)
{
	int				i;

	i = -1;
	while (++i < helper->philo_count)
	{
		if (pthread_create(&philo[i].thread, NULL, &main_thread,
				&philo[i]) != 0)
			return (raise_error("thread",
					"a problem occur while creating threads."), 1);
	}
	while (++i < helper->philo_count)
		pthread_join(philo[i].thread, NULL);
	return (0);
}
