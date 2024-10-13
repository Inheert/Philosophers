/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tclaereb <tclaereb@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 12:28:16 by tclaereb          #+#    #+#             */
/*   Updated: 2024/10/13 12:50:16 by tclaereb         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	raise_error(char *error, char *details)
{
	if (error)
		printf("%s", error);
	if (error && details)
		printf(": ");
	if (details)
		printf("%s\n", details);
}
