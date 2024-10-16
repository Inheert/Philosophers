/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threading_preparation.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclaereb <tclaereb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:19:35 by Théo              #+#    #+#             */
/*   Updated: 2024/10/16 12:46:15 by tclaereb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	set_end_of_simu(t_philosopher *philo, t_helper *helper)
{
	int	i;

	i = -1;
	while (++i < helper->philo_count)
	{
		pthread_mutex_lock(&philo[i].philo_data);
		philo[i].end_of_simu = 1;
		pthread_mutex_unlock(&philo[i].philo_data);
	}
}

int	is_someone_dead(t_philosopher *philo, t_helper *helper)
{
	int	i;

	if (helper->philo_count < 2)
	{
		philo->died_time = actual_time();
		print_routine(philo, "is dead.");
		set_end_of_simu(philo, helper);
		return (1);
	}
	i = -1;
	while (++i < helper->philo_count)
	{
		pthread_mutex_lock(&philo[i].philo_data);
		if (philo[i].last_eat + helper->time_to_die < actual_time())
		{
			philo[i].died_time = actual_time();
			pthread_mutex_unlock(&philo[i].philo_data);
			print_routine(&philo[i], "is dead.");
			set_end_of_simu(philo, helper);
			return (1);
		}
		pthread_mutex_unlock(&philo[i].philo_data);
	}
	return (0);
}

int	is_someone_still_hungry(t_philosopher *philo, t_helper *helper)
{
	int	end_simu;
	int	i;

	end_simu = 1;
	i = -1;
	if (helper->meal_count == 0)
		return (1);
	while (++i < helper->philo_count)
	{
		if (philo[i].eat_count < helper->meal_count)
			end_simu = 0;
		else
			philo[i].end_of_simu = 1;
	}
	if (!end_simu)
		return (1);
	set_end_of_simu(philo, helper);
	return (0);
}

int	start_threads(t_philosopher *philo, t_helper *helper)
{
	t_shared	*shared;
	int			i;

	shared = philo->shared;
	i = -1;
	while (++i < helper->philo_count && helper->philo_count > 1)
	{
		if (pthread_create(&philo[i].thread, NULL, &main_thread,
				&philo[i]) != 0)
			return (raise_error("thread",
					"a problem occur while creating threads."), 1);
	}
	while (!is_someone_dead(philo, helper)
		&& is_someone_still_hungry(philo, helper))
		continue ;
	i = -1;
	while (++i < helper->philo_count)
		pthread_join(philo[i].thread, NULL);
	return (0);
}
