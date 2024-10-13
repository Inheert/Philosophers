/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclaereb <tclaereb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 14:22:09 by tclaereb          #+#    #+#             */
/*   Updated: 2024/10/13 15:42:34 by tclaereb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_all_philo_alive(t_philosopher *philo, t_helper *helper)
{
	int	i;

	i = -1;
	while (++i < helper->philo_count)
	{
		//printf("%d died: %d\n", philo->id, philo->is_dead);
		if (philo[i].is_dead)
			return (0);
	}
	return (1);
}

int	is_philo_should_die(t_philosopher *philo)
{
	long int	died_time;

	if (philo->last_eat + philo->helper->time_to_die < actual_time())
	{
		philo->is_dead = 1;
		died_time = actual_time();
		printf("%ld %d died.\n", died_time, philo->id);
		philo->died_time = died_time;
		return (1);
	}
	return (0);
}

void	start_eating(t_philosopher *philo)
{
	pthread_mutex_lock(&philo->right_fork);
	printf("%ld %d has taken a fork.\n", actual_time(), philo->id);
	pthread_mutex_lock(&philo->left_fork);
	printf("%ld %d has taken a fork.\n", actual_time(), philo->id);
	printf("%ld %d is eating.\n", actual_time(), philo->id);
	philo->last_eat = actual_time();
	usleep(philo->helper->time_to_eat);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->right_fork);
	pthread_mutex_unlock(&philo->left_fork);
}

void	start_sleeping(t_philosopher *philo)
{
	printf("%ld %d is sleeping.\n", actual_time(), philo->id);
	usleep(philo->helper->time_to_sleep);
}

void	*main_thread(void *param)
{
	t_philosopher	*all_philo;
	t_philosopher	*philo;
	t_helper		*helper;

	all_philo = ((t_philosopher **)param)[0];
	philo = ((t_philosopher **)param)[1];
	helper = philo->helper;
	while (is_all_philo_alive(all_philo, helper)
		&& (philo->eat_count < helper->meal_count || helper->meal_count == 0))
	{
		if (is_philo_should_die(philo))
			return (NULL);
		start_eating(philo);
		start_sleeping(philo);
		printf("%ld %d is thinking.\n", actual_time(), philo->id);
		continue ;
	}
	return (NULL);
}

int	start_threads(t_philosopher *philo, t_helper *helper)
{
	t_philosopher	*thread_params[2];
	int				i;

	i = -1;
	while (++i < helper->philo_count)
	{
		thread_params[0] = philo;
		thread_params[1] = &philo[i];
		if (pthread_create(&philo[i].thread, NULL, main_thread,
				thread_params) != 0)
			return (raise_error("thread",
					"a problem occur while creating threads."), 1);
	}
	i = -1;
	while (++i < helper->philo_count)
		pthread_join(philo[i].thread, NULL);
	while (is_all_philo_alive(philo, philo->helper))
		continue ;
	printf("%ld a philosopher is dead.\n", actual_time());
	return (0);
}
