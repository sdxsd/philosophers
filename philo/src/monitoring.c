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
#include <stdio.h>

static int	death_occurred(t_philo *p, t_table *t)
{
	size_t	starvation;

	starvation = ts(p->t_since_meal, exact_time());
	if (starvation >= t->time_to_die)
	{
		pthread_mutex_lock(&t->tbl_lck);
		t->death = TRUE;
		pthread_mutex_unlock(&t->tbl_lck);
		printf("%ld %ld has died\n", ts(t->epoch, exact_time()), p->idx + 1);
		pthread_mutex_unlock(&p->self_lck);
		return (TRUE);
	}
	return (FALSE);
}

void	big_brother(t_table *t, t_philo *nietzche)
{
	size_t	sated_count;
	while (nietzche->r_philo)
	{
		pthread_mutex_lock(&nietzche->self_lck);
		if (nietzche->t_eaten > t->p_to_eat)
			sated_count++;
		else
			sated_count = 0;
		if (death_occurred(nietzche, t))
			return ;
		pthread_mutex_unlock(&nietzche->self_lck);
		nietzche = nietzche->r_philo;
		if (sated_count == t->n_philo && t->meal_limit)
		{
			pthread_mutex_lock(&t->tbl_lck);
			t->death = TRUE;
			pthread_mutex_unlock(&t->tbl_lck);
			return ;
		}
	}
}

void	check_death(t_philo *p, t_table *t)
{
	pthread_mutex_lock(&t->tbl_lck);
	if (t->death)
	{
		pthread_mutex_unlock(&t->tbl_lck);
		if (p->state == EATING)
		{
			pthread_mutex_unlock(p->l_fork);
			pthread_mutex_unlock(&p->r_fork);
		}
		pthread_exit(0);
	}
	pthread_mutex_unlock(&t->tbl_lck);
}
