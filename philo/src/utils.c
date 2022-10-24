/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   CODAM C FILE                                       :+:    :+:            */
/*                                                     +:+                    */
/*   By: wmaguire <wmaguire@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 1970/01/01 00:00:00 by wmaguire      #+#    #+#                 */
/*   Updated: 1970/01/01 00:00:00 by wmaguire     ########   codam.nl         */
/*                                                                            */
/* ************************************************************************** */

/*
THIS FILE IS LICENSED UNDER THE GNU GPLv3
Copyright (C) 2022  Will Maguire

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>

The definition of Free Software is as follows:
				- The freedom to run the program, for any purpose.
				- The freedom to study how the program works, and adapt it to your needs.
				- The freedom to redistribute copies so you can help your neighbor.
				- The freedom to improve the program, and release
				your improvements to the public, so that the whole community benefits.

A program is free software if users have all of these freedoms.
*/

#include "../include/philosophers.h"
#include <limits.h>

static int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' \
	|| c == '\v' || c == '\f' || c == '\r' || c == '\b')
		return (TRUE);
	else
		return (FALSE);
}

static int	num_spaces(const char *str)
{
	int	iter;

	iter = 0;
	while (ft_isspace(str[iter]))
		iter++;
	return (iter);
}

long	ft_atoi(const char *str)
{
	long	iterator;
	long	nbr;
	long	minus;

	nbr = 0;
	iterator = 0;
	minus = 0;
	iterator += num_spaces(str);
	if (str[iterator] == '-' || str[iterator] == '+')
	{
		if (str[iterator + 1] == '+' || str[iterator + 1] == '-')
			return (0);
		++iterator;
	}
	while (str[iterator] >= '0' && str[iterator] <= '9')
	{
		nbr *= 10;
		nbr += (int)str[iterator] - '0';
		++iterator;
	}
	if (minus == -1)
		nbr = (nbr - (nbr * 2));
	return (nbr);
}

/* NOTE: Checks a string for illegal characters. */
static int	chk_string(char *string)
{
	int	iter;

	iter = 0;
	while (string[iter] != '\0')
	{
		if (!(string[iter] >= '0' && string[iter] <= '9'))
			return (FALSE);
		iter++;
	}
	return (TRUE);
}

/* NOTE: Checks the programs arguments for invalid input. */
int	chk_args(char **args)
{
	long	arg_int;
	int		iter;

	iter = 0;
	while (args[iter])
	{
		if (!chk_string(args[iter]))
			return (FALSE);
		arg_int = ft_atoi(args[iter]);
		if (iter == 0 && arg_int < 1)
			return (FALSE);
		if (arg_int > INT_MAX || arg_int < INT_MIN || arg_int < 0)
			return (FALSE);
		iter++;
	}
	return (TRUE);
}
