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

int	construct_table(t_table *table, int args, char **argv)
{
	if (args > 6 || args < 5)
		return (FAILURE);
	if (!chk_args(argv + 1))
		return (FAILURE);
	table->death = FALSE;
	table->n_philo = ft_atoi(argv[1]);
	table->time_to_die = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	table->meal_limit = args - 5;
	if (table->meal_limit)
		table->p_to_eat = ft_atoi(argv[5]);
	else
		table->p_to_eat = 0;
	if (table->n_philo < 2 || (table->p_to_eat == 0 && table->meal_limit))
	{
		if (!(table->p_to_eat == 0 && table->meal_limit))
		{
			usleep(table->time_to_die * 1000);
			printf("%ld %d has died\n", table->time_to_die, 1);
		}
		return (FAILURE);
	}
	if (pthread_mutex_init(&table->tbl_lck, NULL) > 0)
		return (FAILURE);
	return (SUCCESS);
}

int	main(int argc, char	*argv[])
{
	t_table	table;

	if (!construct_table(&table, argc, argv))
		return (FAILURE);
	table.philo_db = init_philosophers(table.n_philo);
	if (!table.philo_db && table.n_philo > 2)
		return (FAILURE);
	if (!init_threads(table.n_philo, table.philo_db, &table))
	{
		free_table(&table);
		return (-1);
	}
	big_brother(&table, table.philo_db[0]);
	free_table(&table);
	return (0);
}
