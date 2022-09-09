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
#include <unistd.h>
#include <stdio.h>

void	print_state(size_t milsec, size_t state, size_t index, t_table *t)
{
	check_death(t->philo_db[index - 1], t);
	pthread_mutex_lock(t->prnt_lck);
	if (state == sleeping)
		printf("%ld %ld is sleeping\n", milsec, index);
	else if (state == eating)
		printf("%ld %ld is eating\n", milsec, index);
	else
		printf("%ld %ld is thinking\n", milsec, index);
	pthread_mutex_unlock(t->prnt_lck);
}

void	philo_think(t_philo *philo, t_table *t)
{
	philo->state = thinking;
	print_state(time_since(t->epoch, exact_time()), \
				philo->state, philo->index, t);
}

void	philo_sleep(t_philo *philo, t_table *t)
{
	philo->state = sleeping;
	check_death(philo, t);
	print_state(time_since(t->epoch, exact_time()), \
				philo->state, philo->index, t);
	i_sleep(t->time_to_sleep, philo, t);
}

void	philo_eat(t_philo *philo, t_table *t)
{
	check_death(philo, t);
	pthread_mutex_lock(philo->l_fork);
	pthread_mutex_lock(philo->r_fork);
	philo->state = eating;
	philo->hunger = exact_time();
	print_state(time_since(t->epoch, exact_time()), \
				philo->state, philo->index, t);
	i_sleep(t->time_to_eat, philo, t);
	philo->eat_cnt++;
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
}

/* >be philosopher */
void	*be_philosopher(void *p)
{
	t_philo	*philo;
	t_table	*table;

	philo = (t_philo *)p;
	table = (t_table *)philo->table;
	if (philo->index % 2)
		usleep(128);
	while (TRUE)
	{
		philo_eat(philo, table);
		philo_sleep(philo, table);
		philo_think(philo, table);
	}
	return (NULL);
}
