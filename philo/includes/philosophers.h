/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclaereb <tclaereb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 09:50:06 by Théo              #+#    #+#             */
/*   Updated: 2024/10/13 12:59:11 by tclaereb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <limits.h>

typedef struct s_helper
{
	int	philo_count;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	meal_count;
}	t_helper;

typedef struct s_philosopher
{
	int			id;
	t_helper	helper;
}	t_philosopher;

// Print a customize error.
void	raise_error(char *error, char *details);
// Convert STR to a int.
int		ft_atoi(char *str);

#endif
