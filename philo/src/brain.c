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

/* Short for: PRINT INTERNAL STATE STATUS */
void	piss(size_t milsec, size_t state, size_t index, t_table *t)
{
	pthread_mutex_lock(&t->tbl_lck);
	if (!t->death)
	{
		if (state == SLEEPING)
			printf("%ld %ld is sleeping\n", milsec, index + 1);
		else if (state == EATING)
			printf("%ld %ld is eating\n", milsec, index + 1);
		else if (state == TKE_FORK)
			printf("%ld %ld has taken a fork\n", milsec, index + 1);
		else
			printf("%ld %ld is thinking\n", milsec, index + 1);
	}
	pthread_mutex_unlock(&t->tbl_lck);
}

void	philo_think(t_philo *philo, t_table *t)
{
	philo->state = THINKING;
	piss(ts(t->epoch, exact_time()), philo->state, philo->idx, t);
	check_death(philo, t);
}

void	philo_sleep(t_philo *philo, t_table *t)
{
	philo->state = SLEEPING;
	piss(ts(t->epoch, exact_time()), philo->state, philo->idx, t);
	check_death(philo, t);
	acc_usleep(philo, t, t->time_to_sleep);
}

void	philo_eat(t_philo *philo, t_table *t)
{
	pthread_mutex_lock(&philo->r_fork);
	piss(ts(t->epoch, exact_time()), TKE_FORK, philo->idx, t);
	pthread_mutex_lock(philo->l_fork);
	piss(ts(t->epoch, exact_time()), TKE_FORK, philo->idx, t);
	philo->state = EATING;
	piss(ts(t->epoch, exact_time()), philo->state, philo->idx, t);
	pthread_mutex_lock(&philo->self_lck);
	philo->t_since_meal = exact_time();
	philo->t_eaten++;
	pthread_mutex_unlock(&philo->self_lck);
	acc_usleep(philo, t, t->time_to_eat);
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(&philo->r_fork);
}

/* >be philosopher */
void	*be_philosopher(void *p)
{
	t_philo	*philo;
	t_table	*table;

	philo = (t_philo *)p;
	table = (t_table *)philo->table;
	if (philo->idx % 2)
		usleep(512);
	while (TRUE)
	{
		philo_eat(philo, table);
		philo_sleep(philo, table);
		philo_think(philo, table);
	}
	return (NULL);
}
