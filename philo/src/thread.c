/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclaereb <tclaereb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 14:22:09 by tclaereb          #+#    #+#             */
/*   Updated: 2024/10/16 12:44:37 by tclaereb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	start_sleeping(t_philosopher *philo)
{
	print_routine(philo, "is sleeping.");
	usleep(philo->helper->time_to_sleep * 1000);
	print_routine(philo, "is thinking.");
}

static void	start_eating(t_philosopher *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&philo->right_fork);
		print_routine(philo, "has taken right fork.");
		pthread_mutex_lock(philo->left_fork);
		print_routine(philo, "has taken left fork.");
	}
	else
	{
		pthread_mutex_lock(philo->left_fork);
		print_routine(philo, "has taken left fork.");
		pthread_mutex_lock(&philo->right_fork);
		print_routine(philo, "has taken right fork.");
	}
	pthread_mutex_lock(&philo->philo_data);
	philo->last_eat = actual_time();
	pthread_mutex_unlock(&philo->philo_data);
	print_routine(philo, "is eating.");
	usleep(philo->helper->time_to_eat * 1000);
	philo->eat_count++;
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
