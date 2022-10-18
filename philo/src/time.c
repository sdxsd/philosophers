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
#include <sys/time.h>
#include <unistd.h>

size_t	sec_to_mil(size_t seconds)
{
	return (seconds * 1000);
}

size_t	mic_to_mil(size_t mic_seconds)
{
	return (mic_seconds / 1000);
}

size_t	exact_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (sec_to_mil(time.tv_sec) + mic_to_mil(time.tv_usec));
}

size_t	time_since(size_t epoch, size_t current)
{
	return ((current - epoch));
}

void	i_sleep(int milsecs, t_philo *p, t_table *t)
{
	int	iter;

	iter = 0;
	while (iter < milsecs)
	{
		usleep(((milsecs * 1000) / 10));
		iter += milsecs / 10;
		check_death(p, t);
	}
}
