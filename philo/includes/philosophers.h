/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclaereb <tclaereb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 09:50:06 by Théo              #+#    #+#             */
/*   Updated: 2024/10/13 14:20:51 by tclaereb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <sys/time.h>
# include <pthread.h>

// This structure is used as an helper during the execution,
// it store all useful data.
typedef struct s_helper
{
	int	philo_count;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	meal_count;
}	t_helper;

// This structure is used to identify a philosopher, each one
// being defined on the basis of this structure.
typedef struct s_philosopher
{
	t_helper		*helper;
	pthread_t		*thread;
	int				id;
	int				last_eat;
	int				eat_count;
	pthread_mutex_t	left_fork;
	pthread_mutex_t	right_fork;
}	t_philosopher;

// Print a customize error.
void	raise_error(char *error, char *details);
// Convert STR to a int.
int		ft_atoi(char *str);
// Get the actual time in timestamp.
long int	actual_time(void);
// Prepare all philosophers and create the helper structure, if a problen
// occur then a NULL ptr is returned and the program end.
t_philosopher	*prepare_philosophers(char **argv);

#endif
