/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclaereb <tclaereb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 14:22:09 by tclaereb          #+#    #+#             */
/*   Updated: 2024/10/21 11:31:58 by tclaereb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	ft_usleep(long int ms)
{
	long int	start;

	start = actual_time();
	while (actual_time() - start < ms)
		usleep(1000);
}

static void	start_sleeping(t_philosopher *philo)
{
	ft_usleep(philo->helper->time_to_sleep);
	print_routine(philo, "is sleeping");
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
	pthread_mutex_lock(&philo->philo_data);
	philo->last_eat = actual_time();
	philo->eat_count++;
	pthread_mutex_unlock(&philo->philo_data);
	ft_usleep(philo->helper->time_to_eat);
	print_routine(philo, "is eating");
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
