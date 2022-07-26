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
#include <stdio.h>
#include <unistd.h>

void	print_state(size_t milsec, size_t state, size_t index, t_table *t)
{
	pthread_mutex_lock(t->prnt_lck);
	if (state == sleeping)
		printf("%ld %ld is sleeping", milsec, index);
	else if (state == eating)
		printf("%ld %ld is eating", milsec, index);
	else
		printf("%ld %ld is thinking", milsec, index);
	pthread_mutex_unlock(t->prnt_lck);
}

t_philo	*init_philosopher(t_philo *l_philo, t_philo *r_philo, int index)
{
	t_philo	*new_philo;
	int		ret;

	new_philo = malloc(sizeof(t_philo));
	if (!new_philo)
		return (NULL);
	new_philo->state = sleeping;
	new_philo->index = index;
	new_philo->l_philo = l_philo;
	new_philo->r_philo = r_philo;
	if (index == 0)
		;
	else
		new_philo->l_fork = l_philo->r_fork;
	new_philo->r_fork = malloc(sizeof(pthread_mutex_t));
	ret = pthread_mutex_init(new_philo->r_fork, NULL);
	if (ret > 0)
		return (NULL);
	return (new_philo);
}

t_philo	**init_philosophers(int n_philos)
{
	t_philo	**philosophers_db;
	t_philo	*initial_philo;
	t_philo	*philo_1;
	t_philo	*philo_2;
	int		iter;

	philosophers_db = malloc(sizeof(t_philo *) * n_philos);
	if (!philosophers_db)
		return (NULL);
	initial_philo = init_philosopher(NULL, NULL, 0);
	if (!initial_philo)
		return (NULL);
	philo_1 = initial_philo;
	philosophers_db[0] = initial_philo;
	iter = 1;
	while (iter < n_philos)
	{
		philo_2 = init_philosopher(philo_1, NULL, iter);
		if (!philo_2)
			return (NULL);
		else
		{
			philo_1->r_philo = philo_2;
		}
		philo_1 = philo_2;
		philosophers_db[iter] = philo_1;
		iter++;
	}
	philo_1->r_philo = initial_philo;
	initial_philo->l_philo = philo_1;
	initial_philo->l_fork = philo_1->r_fork;
	return (philosophers_db);
}

void	eat(t_philo	*philo)
{
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);

}

/* >be philosopher */
void	be_philosopher(t_philo *philo, t_table *table)
{
	if (philo->index % 2)
		usleep(1024);
	print_state(exact_time(), philo->index, philo->state, table);
}
