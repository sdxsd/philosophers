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
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int	check_sated(t_table *table)
{
	size_t	iter;

	iter = 0;
	if (table->eat_count == 0)
		return (FALSE);
	while (iter < table->n_philo)
	{
		pthread_mutex_lock(table->philo_mutex);
		if (table->philo_db[iter]->eat_cnt > table->eat_count)
			;
		else
		{
			pthread_mutex_unlock(table->philo_mutex);
			return (FALSE);
		}
		pthread_mutex_unlock(table->philo_mutex);
		iter++;
	}
	return (TRUE);
}

/* Big brother is always watching  */
void	big_brother(t_table *table)
{
	int		ret;

	while (TRUE)
	{
		usleep(4096);
		ret = check_sated(table);
		pthread_mutex_lock(table->philo_mutex);
		if (table->gedood || ret)
		{
			if (ret)
			{
				pthread_mutex_lock(table->prnt_lck);
				table->gedood = TRUE;
				pthread_mutex_unlock(table->prnt_lck);
			}
			pthread_mutex_unlock(table->philo_mutex);
			free_table(table, table->time_of_death, table->deadite, ret);
			break ;
		}
		pthread_mutex_unlock(table->philo_mutex);
	}
}

int	main(int argc, char	*argv[])
{
	t_table	*table;

	if (argc > 6 || argc < 5)
		return (0);
	if (!chk_args(argv + 1))
		return (0);
	table = construct_table(argc - 1, argv + 1);
	if (!table)
		return (-1);
	if (table->n_philo < 2 || (table->eat_count == 0 && argc > 5))
	{
		if (table->eat_count < 1 && argc > 5)
			return (0);
		usleep(table->time_to_die * 1000);
		free_table(table, table->time_to_die, 0, 0);
		return (0);
	}
	if (!init_threads(table->n_philo, table->philo_db, table))
	{
		free_table(table, table->time_to_die, 0, 1);
		return (-1);
	}
	big_brother(table);
	return (0);
}
