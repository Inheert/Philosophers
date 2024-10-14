/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Théo <theoclaereboudt@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 09:50:06 by Théo              #+#    #+#             */
/*   Updated: 2024/10/14 18:20:22 by Théo             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <sys/time.h>
# include <pthread.h>

// This structure is used as an helper during the execution,
// it store all useful data.
typedef struct s_helper
{
	int				philo_count;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				meal_count;
}	t_helper;

typedef struct s_shared
{
	int				is_someone_is_dead;
	pthread_mutex_t	check_meal;
	pthread_mutex_t	check_death;
}	t_shared;

// This structure is used to identify a philosopher, each one
// being defined on the basis of this structure.
typedef struct s_philosopher
{
	t_helper		*helper;
	t_shared		*shared;
	pthread_t		thread;
	int				id;
	long int		last_eat;
	long int		died_time;
	int				eat_count;
	int				is_dead;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	right_fork;
}	t_philosopher;

// Print a customize error.
void			raise_error(char *error, char *details);
// Convert STR to a int.
int				ft_atoi(char *str);
// Get the actual time in timestamp.
long int		actual_time(void);
// Prepare all philosophers and create the helper structure, if a problen
// occur then a NULL ptr is returned and the program end.
t_philosopher	*prepare_philosophers(char **argv);
void			print_routine(t_philosopher *philo, char *action);
void			*main_thread(void *param);
// Start all philosophers threads.
int				start_threads(t_philosopher *philo, t_helper *helper);
void			free_t_philosopher(t_philosopher *philo, int philo_count);
void			free_t_shared(t_shared *shared);
#endif
