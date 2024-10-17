/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Théo <theoclaereboudt@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 09:50:06 by Théo              #+#    #+#             */
/*   Updated: 2024/10/17 12:47:35 by Théo             ###   ########.fr       */
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
	pthread_mutex_t	print_routine;
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
	int				eat_count;
	int				end_of_simu;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	right_fork;
	pthread_mutex_t	philo_data;
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
// Used to print what a philosopher is actually doing thread safely.
void			print_routine(t_philosopher *philo, char *action);
// Function where the thread start and eat, sleep, think or die.
void			*main_thread(void *param);
// Start all philosophers threads.
int				start_threads(t_philosopher *philo, t_helper *helper);
// Free the t_philosopher struct and its copie of the t_helper struct,
// also destroy forks mutexes.
void			free_t_philosopher(t_philosopher *philo, int philo_count);
// Free the shared struct and destroy mutexes.
void			free_t_shared(t_shared *shared);
#endif
