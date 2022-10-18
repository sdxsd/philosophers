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

/* The philosophers dining problem was first invented by Edsgar Dijkstra */
/* Dutch computer scientist. */

#include "../include/philosophers.h"
#include <stdlib.h>

void	set_values(t_philo *philo, int index)
{
	philo->eat_cnt = 0;
	philo->state = sleeping;
	philo->index = index;
	philo->death = FALSE;
	philo->sated = FALSE;
}

t_philo	*init_philosopher(t_philo *l_philo, t_philo *r_philo, int index)
{
	t_philo	*new_philo;
	int		ret;

	new_philo = malloc(sizeof(t_philo));
	if (!new_philo)
		return (NULL);
	set_values(new_philo, index);
	new_philo->l_philo = l_philo;
	new_philo->r_philo = r_philo;
	if (index > 0)
		new_philo->l_fork = l_philo->r_fork;
	new_philo->r_fork = malloc(sizeof(pthread_mutex_t));
	if (!new_philo->r_fork)
	{
		free(new_philo);
		return (NULL);
	}
	ret = pthread_mutex_init(new_philo->r_fork, NULL);
	if (ret > 0)
	{
		free(new_philo->r_fork);
		free(new_philo);
		return (NULL);
	}
	return (new_philo);
}

void	*init_philo_fail(t_philo **db, int n_philos)
{
	int	iter;

	while (iter < n_philos)
	{
		if (db[iter]->r_fork)
		{
			pthread_mutex_destroy(db[iter]->r_fork);
			free(db[iter]->r_fork);
		}
		free(db[iter]);
		iter++;
	}
	free(db);
	return (NULL);
}

t_philo	*populate_table(t_philo *p1, t_philo **db, int n)
{
	t_philo	*p2;
	int		iter;

	iter = 1;
	while (iter < n)
	{
		p2 = init_philosopher(p1, NULL, iter);
		if (!p2)
			return (init_philo_fail(db, iter - 1));
		else
			p1->r_philo = p2;
		p1 = p2;
		db[iter] = p1;
		iter++;
	}
	return (p1);
}

t_philo	**init_philosophers(int n_philos)
{
	t_philo	**philosophers_db;
	t_philo	*initial_philo;
	t_philo	*philo_1;

	if (n_philos < 2)
		return (NULL);
	philosophers_db = malloc(sizeof(t_philo *) * n_philos);
	if (!philosophers_db)
		return (NULL);
	initial_philo = init_philosopher(NULL, NULL, 0);
	if (!initial_philo)
	{
		free(philosophers_db);
		return (NULL);
	}
	philo_1 = initial_philo;
	philosophers_db[0] = initial_philo;
	philo_1 = populate_table(philo_1, philosophers_db, n_philos);
	if (!philo_1)
		return (NULL);
	philo_1->r_philo = initial_philo;
	initial_philo->l_philo = philo_1;
	initial_philo->l_fork = philo_1->r_fork;
	return (philosophers_db);
}