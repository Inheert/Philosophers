/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preprocessing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclaereb <tclaereb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 13:07:23 by tclaereb          #+#    #+#             */
/*   Updated: 2024/10/13 18:17:34 by tclaereb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static t_helper	*parse_param(char **argv)
{
	t_helper	*helper;

	helper = malloc(sizeof(t_helper));
	if (!helper)
		return (raise_error("malloc",
				"allocation failed for the helper struct."), NULL);
	helper->philo_count = ft_atoi(argv[0]);
	helper->time_to_die = ft_atoi(argv[1]);
	helper->time_to_eat = ft_atoi(argv[2]);
	helper->time_to_sleep = ft_atoi(argv[3]);
	helper->meal_count = 0;
	if (argv[4])
		helper->meal_count = ft_atoi(argv[4]);
	if (helper->philo_count == -1 || helper->time_to_die == -1
		|| helper->time_to_eat == -1 || helper->time_to_sleep == -1
		|| helper->meal_count == -1)
		return (free(helper), NULL);
	return (helper);
}

static t_helper	*copy_t_helper_struct(t_helper *helper)
{
	t_helper	*new;

	new = malloc(sizeof(t_helper));
	if (!new)
		return (NULL);
	new->philo_count = helper->philo_count;
	new->time_to_die = helper->time_to_die;
	new->time_to_eat = helper->time_to_eat;
	new->time_to_sleep = helper->time_to_sleep;
	new->meal_count = helper->meal_count;
	return (new);
}

static t_shared	*create_shared_struct()
{
	t_shared	*shared;

	shared = malloc(sizeof(t_shared));
	shared->is_someone_is_dead = 0;
	if (pthread_mutex_init(&shared->write, NULL) != 0)
		return (free(shared), NULL);
	return (shared);
}

int	initialize_philosophers_struct(t_philosopher *philo,
	t_helper *helper, t_shared *shared)
{
	long int		start;
	int				i;

	start = actual_time();
	i = -1;
	while (++i < helper->philo_count)
	{
		if (pthread_mutex_init(&philo[i].right_fork, NULL) != 0)
			return (raise_error("mutex", "problem occur with mutex init."), 1);
		if (i != 0)
			philo[i - 1].left_fork = &philo[i].right_fork;
		if (i == helper->philo_count - 1)
			philo[i].left_fork = &philo[0].right_fork;
		philo[i].helper = copy_t_helper_struct(helper);
		if (!philo[i].helper)
			return (1);
		philo->shared = shared;
		philo[i].id = i + 1;
		philo[i].last_eat = start;
		philo[i].eat_count = 0;
		philo[i].is_dead = 0;
	}
	return (0);
}

t_philosopher	*prepare_philosophers(char **argv)
{
	t_shared		*shared;
	t_helper		*helper;
	t_philosopher	*philosophers;

	shared = create_shared_struct();
	if (!shared)
		return (raise_error("shared",
				"a problem occur when initialize the shared struture."), NULL);
	helper = parse_param(++argv);
	if (!helper)
		return (raise_error("helper",
				"a problem occur when initialize the helper struture."), NULL);
	philosophers = malloc(sizeof(t_philosopher) * (helper->philo_count + 1));
	if (!philosophers)
		return (free(helper), raise_error("philosophers",
				"allocation failed for the philosophers structure."), NULL);
	if (initialize_philosophers_struct(philosophers, helper, shared) != 0)
		return (free(helper), free(philosophers), NULL);
	return (philosophers);
}
