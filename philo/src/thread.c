/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Théo <theoclaereboudt@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 14:22:09 by tclaereb          #+#    #+#             */
/*   Updated: 2024/10/17 13:29:32 by Théo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	start_sleeping(t_philosopher *philo)
{
	print_routine(philo, "is sleeping");
	usleep(philo->helper->time_to_sleep * 1000);
	print_routine(philo, "is thinking");
}

static void	start_eating(t_philosopher *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->right_fork);
		print_routine(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_routine(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_routine(philo, "has taken a fork");
		pthread_mutex_lock(&philo->right_fork);
		print_routine(philo, "has taken a fork");
	}
	print_routine(philo, "is eating");
	pthread_mutex_lock(&philo->philo_data);
	philo->last_eat = actual_time();
	philo->eat_count++;
	pthread_mutex_unlock(&philo->philo_data);
	usleep(philo->helper->time_to_eat * 1000);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(&philo->right_fork);
}

void	*main_thread(void *param)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)param;
	pthread_mutex_lock(&philo->philo_data);
	while (!philo->end_of_simu)
	{
		pthread_mutex_unlock(&philo->philo_data);
		start_eating(philo);
		start_sleeping(philo);
		pthread_mutex_lock(&philo->philo_data);
	}
	pthread_mutex_unlock(&philo->philo_data);
	return (NULL);
}
