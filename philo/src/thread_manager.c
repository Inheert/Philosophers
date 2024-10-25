/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_manager.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclaereb <tclaereb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 18:19:35 by Théo              #+#    #+#             */
/*   Updated: 2024/10/25 10:26:31 by tclaereb         ###   ########.fr       */
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
		print_routine(philo, "died");
		set_end_of_simu(philo, helper);
		return (1);
	}
	i = -1;
	while (++i < helper->philo_count)
	{
		pthread_mutex_lock(&philo[i].philo_data);
		if (philo[i].last_eat + helper->time_to_die <= actual_time())
		{
			pthread_mutex_unlock(&philo[i].philo_data);
			print_routine(&philo[i], "died");
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
		pthread_mutex_lock(&philo[i].philo_data);
		if (philo[i].eat_count < helper->meal_count)
			end_simu = 0;
		else
			philo[i].end_of_simu = 1;
		pthread_mutex_unlock(&philo[i].philo_data);
	}
	if (!end_simu)
		return (1);
	set_end_of_simu(philo, helper);
	return (0);
}

void	clean_exit_threads(t_philosopher *philo, int failer)
{
	int	i;

	i = failer;
	while (--i >= 0)
	{
		pthread_mutex_lock(&philo[i].philo_data);
		philo[i].end_of_simu = 1;
		pthread_mutex_unlock(&philo[i].philo_data);
	}
	while (failer >= 0)
		pthread_join(philo[i].thread, NULL);
}

int	start_threads(t_philosopher *philo, t_helper *helper)
{
	int			i;

	i = -1;
	while (++i < helper->philo_count && helper->philo_count > 1)
	{
		if (pthread_create(&philo[i].thread, NULL, &main_thread,
				&philo[i]) != 0)
			return (clean_exit_threads(philo, i), raise_error("thread",
					"a problem occur while creating threads."), 1);
	}
	while (!is_someone_dead(philo, helper)
		&& is_someone_still_hungry(philo, helper))
		usleep(500);
	i = -1;
	while (++i < helper->philo_count)
		pthread_join(philo[i].thread, NULL);
	return (0);
}
