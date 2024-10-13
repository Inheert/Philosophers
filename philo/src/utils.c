/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclaereb <tclaereb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 12:58:35 by tclaereb          #+#    #+#             */
/*   Updated: 2024/10/13 12:58:51 by tclaereb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(char *str)
{
	int	num;
	int	i;

	if (*str == '-')
		return (raise_error(str, "cant be a negative number."), -1);
	num = 0;
	i = -1;
	while (str[++i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (raise_error(str,
					"arg is not a number or contain invalid char."), -1);
		if ((long)num * 10 + str[i] - '0' > INT_MAX)
			return (raise_error(str,
					"arg exceed the maximum of an int."), -1);
		num = num * 10 + str[i] - '0';
	}
	return (num);
}
