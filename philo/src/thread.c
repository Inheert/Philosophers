/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Théo <theoclaereboudt@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 14:22:09 by tclaereb          #+#    #+#             */
/*   Updated: 2024/10/14 21:34:33 by Théo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	is_philo_should_die(t_philosopher *philo)
{
	long int	died_time;

	pthread_mutex_lock(&philo->shared->check_death);
	if (philo->last_eat + philo->helper->time_to_die < actual_time()
		|| philo->helper->philo_count < 2)
	{
		philo->is_dead = 1;
		if (philo->shared->is_someone_is_dead)
			return (pthread_mutex_unlock(&philo->shared->check_death), 1);
		philo->shared->is_someone_is_dead = 1;
		printf("%ld %d died.\n", actual_time(), philo->id);
		pthread_mutex_unlock(&philo->shared->check_death);
		died_time = actual_time();
		philo->died_time = died_time;
		return (1);
	}
	pthread_mutex_unlock(&philo->shared->check_death);
	return (0);
}

static void	start_sleeping(t_philosopher *philo)
{
	usleep(philo->helper->time_to_sleep * 1000);
	print_routine(philo, "is sleeping.");
	print_routine(philo, "is thinking.");
}

static void	start_eating(t_philosopher *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_routine(philo, "has taken a fork.");
		pthread_mutex_lock(&philo->right_fork);
		print_routine(philo, "has taken a fork.");
	}
	else
	{
		pthread_mutex_lock(&philo->right_fork);
		print_routine(philo, "has taken a fork.");
		pthread_mutex_lock(philo->left_fork);
		print_routine(philo, "has taken a fork.");
	}
	philo->last_eat = actual_time();
	usleep(philo->helper->time_to_eat * 1000);
	print_routine(philo, "is eating.");
	pthread_mutex_lock(&philo->shared->check_meal);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->shared->check_meal);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(&philo->right_fork);
}

static int	is_philo_thread_still_valid(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->shared->check_meal);
	if (philo->eat_count >= philo->helper->meal_count
		&& philo->helper->meal_count != 0)
		return (pthread_mutex_unlock(&philo->shared->check_meal), 0);
	pthread_mutex_unlock(&philo->shared->check_meal);
	pthread_mutex_lock(&philo->shared->check_death);
	if (philo->shared->is_someone_is_dead)
		return (pthread_mutex_unlock(&philo->shared->check_death), 0);
	pthread_mutex_unlock(&philo->shared->check_death);
	return (1);
}

void	*main_thread(void *param)
{
	t_philosopher	*philo;

	philo = (t_philosopher *)param;
	if (is_philo_should_die(philo))
		return (NULL);
	while (is_philo_thread_still_valid(philo))
	{
		if (is_philo_should_die(philo))
			return (NULL);
		start_eating(philo);
		start_sleeping(philo);
	}
	return (NULL);
}
