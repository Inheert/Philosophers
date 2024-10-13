/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclaereb <tclaereb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 14:22:09 by tclaereb          #+#    #+#             */
/*   Updated: 2024/10/13 16:30:46 by tclaereb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	is_all_philo_alive(t_philosopher *philo, t_helper *helper)
{
	int	i;

	i = -1;
	while (++i < helper->philo_count)
	{
		if (philo[i].is_dead)
			return (0);
	}
	return (1);
}

int	is_philo_should_die(t_philosopher *philo)
{
	long int	died_time;

	if (philo->last_eat + philo->helper->time_to_die < actual_time()
		|| &philo->right_fork == philo->left_fork)
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
	printf("%ld %d has taken a right fork.\n", actual_time(), philo->id);
	pthread_mutex_lock(philo->left_fork);
	printf("%ld %d has taken a left fork.\n", actual_time(), philo->id);
	printf("%ld %d is eating.\n", actual_time(), philo->id);
	philo->last_eat = actual_time();
	usleep(philo->helper->time_to_eat);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
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
	printf("hi %d\n", philo->id);
	helper = philo->helper;
	if (is_philo_should_die(philo))
		return (NULL);
	while (is_all_philo_alive(all_philo, helper)
		&& (philo->eat_count < helper->meal_count || helper->meal_count == 0))
	{
		if (is_philo_should_die(philo))
			return (NULL);
		start_eating(philo);
		start_sleeping(philo);
		printf("%ld %d is thinking.\n", actual_time(), philo->id);
	}
	return (NULL);
}

int	start_threads(t_philosopher *philo, t_helper *helper)
{
	t_philosopher	*thread_params[2];
	int				i;

	i = -1;
	thread_params[0] = philo;
	while (++i < helper->philo_count)
	{
		thread_params[1] = philo + i;
		if (pthread_create(&philo[i].thread, NULL, main_thread,
				thread_params) != 0)
			return (raise_error("thread",
					"a problem occur while creating threads."), 1);
	}
	i = -1;
	while (++i < helper->philo_count)
		pthread_join(philo[i].thread, NULL);
	//printf("%d %d %d\n", philo->id, philo->eat_count, philo->helper->meal_count);
	while (is_all_philo_alive(philo, philo->helper)
		&& philo->eat_count < philo->helper->meal_count)
		continue ;
	if (philo->eat_count < philo->helper->meal_count)
		printf("%ld a philosopher is dead.\n", actual_time());
	return (0);
}
