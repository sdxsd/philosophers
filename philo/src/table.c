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

void	set_variables(t_table *table, int args, char **argv)
{
	table->time_of_death = 0;
	table->gedood = FALSE;
	table->n_philo = ft_atoi(argv[0]);
	table->time_to_die = ft_atoi(argv[1]);
	table->time_to_eat = ft_atoi(argv[2]);
	table->time_to_sleep = ft_atoi(argv[3]);
	if (args > 4)
	{
		table->eat_limit = TRUE;
		table->eat_count = ft_atoi(argv[4]);
	}
	else
	{
		table->eat_limit = FALSE;
		table->eat_count = 0;
	}
}

t_table	*construct_table(int args, char **argv)
{
	t_table	*table;
	int		ret;

	table = malloc(sizeof(t_table));
	if (!table)
		return (NULL);
	set_variables(table, args, argv);
	table->philo_db = init_philosophers(table->n_philo);
	if (!table->philo_db && table->n_philo > 2)
		return (free_table(table));
	table->prnt_lck = malloc(sizeof(pthread_mutex_t));
	if (!table->prnt_lck)
		return (free_table(table));
	table->philo_mutex = malloc(sizeof(pthread_mutex_t));
	if (!table->philo_mutex)
		return (free_table(table));
	ret = pthread_mutex_init(table->prnt_lck, NULL);
	if (ret > 0)
		return (free_table(table));
	ret = pthread_mutex_init(table->philo_mutex, NULL);
	if (ret > 0)
		return (free_table(table));
	return (table);
}
