/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Théo <theoclaereboudt@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 14:22:09 by tclaereb          #+#    #+#             */
/*   Updated: 2024/10/14 18:03:40 by Théo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	print_routine(t_philosopher *philo, char *action)
{
	pthread_mutex_lock(&philo->shared->check_death);
	if (philo->shared->is_someone_is_dead)
	{
		pthread_mutex_unlock(&philo->shared->check_death);
		return ;
	}
	printf("%ld %d %s\n", actual_time(), philo->id, action);
	pthread_mutex_unlock(&philo->shared->check_death);
}

int	is_philo_should_die(t_philosopher *philo)
{
	long int	died_time;

	pthread_mutex_lock(&philo->shared->check_death);
	if (philo->last_eat + philo->helper->time_to_die < actual_time())
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

void	start_sleeping(t_philosopher *philo)
{
	usleep(philo->helper->time_to_sleep * 1000);
	print_routine(philo, "is sleeping.");
	print_routine(philo, "is thinking.");
}

void	start_eating(t_philosopher *philo)
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

int	is_philo_thread_still_valid(t_philosopher *philo)
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
	while (1)
	{
		usleep(10 * 1000);
		if (have_all_the_philos_finished_eating(philo, helper))
			break ;
		pthread_mutex_lock(&shared->check_death);
		if (philo->shared->is_someone_is_dead)
		{
			printf("%ld a philosopher is dead.\n", actual_time());
			pthread_mutex_unlock(&shared->check_death);
			break ;
		}
		pthread_mutex_unlock(&shared->check_death);
	}
	while (++i < helper->philo_count)
		pthread_join(philo[i].thread, NULL);
	return (0);
}
