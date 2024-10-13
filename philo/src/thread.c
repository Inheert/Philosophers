/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Théo <theoclaereboudt@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 14:22:09 by tclaereb          #+#    #+#             */
/*   Updated: 2024/10/13 20:55:50 by Théo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_philo_should_die(t_philosopher *philo)
{
	long int	died_time;

	if (philo->last_eat + philo->helper->time_to_die < actual_time()
		|| &philo->right_fork == philo->left_fork)
	{
		philo->is_dead = 1;
		if (philo->shared->is_someone_is_dead)
			return (1);
		philo->shared->is_someone_is_dead = 1;
		died_time = actual_time();
		pthread_mutex_lock(&philo->shared->write);
		printf("%ld %d died.\n", died_time, philo->id);
		pthread_mutex_unlock(&philo->shared->write);
		philo->died_time = died_time;
		return (1);
	}
	return (0);
}

int		is_philo_should_exit(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->shared->check_death);
	if (philo->shared->is_someone_is_dead)
		return (pthread_mutex_unlock(&philo->shared->check_death), 1);
	pthread_mutex_unlock(&philo->shared->check_death);
	return (0);
}

void	start_sleeping(t_philosopher *philo)
{
	if (is_philo_should_exit(philo))
		return ;
	pthread_mutex_lock(&philo->shared->write);
	printf("%ld %d is sleeping.\n", actual_time(), philo->id);
	pthread_mutex_unlock(&philo->shared->write);
	usleep(philo->helper->time_to_sleep * 1000);
}

void	start_eating(t_philosopher *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(&philo->right_fork);
	}
	else
	{
		pthread_mutex_lock(&philo->right_fork);
		pthread_mutex_lock(philo->left_fork);
	}
	if (is_philo_should_exit(philo))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(&philo->right_fork);
		return ;
	}
	pthread_mutex_lock(&philo->shared->write);
	printf("%ld %d has taken a fork.\n", actual_time(), philo->id);
	printf("%ld %d has taken a fork.\n", actual_time(), philo->id);
	printf("%ld %d is eating.\n", actual_time(), philo->id);
	pthread_mutex_unlock(&philo->shared->write);
	philo->last_eat = actual_time();
	usleep(philo->helper->time_to_eat * 1000);
	philo->eat_count++;
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(&philo->right_fork);
}

void	*main_thread(void *param)
{
	t_philosopher	*philo;
	t_helper		*helper;

	philo = (t_philosopher *)param;
	helper = philo->helper;
	if (is_philo_should_die(philo))
		return (NULL);
	while ((philo->eat_count < helper->meal_count || helper->meal_count == 0)
		&& !philo->shared->is_someone_is_dead)
	{
		pthread_mutex_lock(&philo->shared->check_death);
		if (is_philo_should_die(philo))
			return (pthread_mutex_unlock(&philo->shared->check_death), NULL);
		pthread_mutex_unlock(&philo->shared->check_death);
		if (is_philo_should_exit(philo))
			return (NULL);
		start_eating(philo);
		if (is_philo_should_exit(philo))
			return (NULL);
		start_sleeping(philo);
		if (is_philo_should_exit(philo))
			return (NULL);
		pthread_mutex_lock(&philo->shared->write);
		printf("%ld %d is thinking.\n", actual_time(), philo->id);
		pthread_mutex_unlock(&philo->shared->write);
	}
	return (NULL);
}

int	start_threads(t_philosopher *philo, t_helper *helper)
{
	t_shared		*shared;
	int				i;

	shared = philo->shared;
	i = -1;
	while (++i < helper->philo_count)
	{
		if (pthread_create(&philo[i].thread, NULL, &main_thread, &philo[i]) != 0)
			return (raise_error("thread",
					"a problem occur while creating threads."), 1);
	}
	i = -1;
	while (++i < helper->philo_count)
		pthread_join(philo[i].thread, NULL);
	while (1)
	{
		pthread_mutex_lock(&shared->check_death);
		if (shared->is_someone_is_dead || (philo->eat_count == helper->meal_count && helper->meal_count != 0))
		{
			pthread_mutex_unlock(&shared->check_death);
			break ;
		}
		pthread_mutex_unlock(&shared->check_death);
	}
	if (philo->shared->is_someone_is_dead)
		printf("%ld a philosopher is dead.\n", actual_time());
	return (0);
}
