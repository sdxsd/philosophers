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

static void	set_values(t_philo *philo, int index)
{
	philo->t_eaten = 0;
	philo->state = SLEEPING;
	philo->idx = index;
	philo->t_since_meal = 0;
}

static t_philo	*init_philosopher(t_philo *l_philo, t_philo *r_philo, int index)
{
	t_philo	*new_philo;

	new_philo = malloc(sizeof(t_philo));
	if (!new_philo)
		return (NULL);
	set_values(new_philo, index);
	if (pthread_mutex_init(&new_philo->self_lck, NULL) > 0)
	{
		free(new_philo);
		return (NULL);
	}
	if (pthread_mutex_init(&new_philo->r_fork, NULL) > 0)
	{
		pthread_mutex_destroy(&new_philo->self_lck);
		free(new_philo);
		return (NULL);
	}
	new_philo->l_philo = l_philo;
	new_philo->r_philo = r_philo;
	if (index > 0)
		new_philo->l_fork = &l_philo->r_fork;
	return (new_philo);
}

static t_philo	*populate_table(t_philo *ip, t_philo *p1, t_philo **db, int n)
{
	t_philo	*p2;
	int		iter;

	iter = 1;
	while (iter < n)
	{
		p2 = init_philosopher(p1, NULL, iter);
		if (!p2)
		{
			while (iter-- >= 0)
				free(db[iter]);
			return (NULL);
		}
		else
			p1->r_philo = p2;
		p1 = p2;
		db[iter] = p1;
		iter++;
	}
	p1->r_philo = ip;
	ip->l_philo = p1;
	ip->l_fork = &p1->r_fork;
	return (p1);
}

t_philo	**init_philosophers(int n_philos)
{
	t_philo	**philosophers_db;
	t_philo	*ip;
	t_philo	*last_philo;

	philosophers_db = malloc(sizeof(t_philo *) * n_philos);
	if (!philosophers_db)
		return (NULL);
	ip = init_philosopher(NULL, NULL, 0);
	if (!ip)
	{
		free(philosophers_db);
		return (NULL);
	}
	philosophers_db[0] = ip;
	last_philo = populate_table(ip, ip, philosophers_db, n_philos);
	if (!last_philo)
	{
		free(philosophers_db);
		free(ip);
		return (NULL);
	}
	return (philosophers_db);
}
