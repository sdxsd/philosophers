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

void	join_threads(int n_philo, pthread_t *thread_db)
{
	while (n_philo-- > 0)
		pthread_join(thread_db[n_philo], NULL);
	free(thread_db);
}

int	init_threads(int n_philo, t_philo **p_db, t_table *t)
{
	int			iter;
	int			ret;

	t->threads = malloc(sizeof(pthread_t) * n_philo);
	if (!t->threads)
		return (FALSE);
	iter = 0;
	t->epoch = exact_time();
	while (iter < n_philo)
	{
		p_db[iter]->table = t;
		p_db[iter]->t_since_meal = t->epoch;
		ret = pthread_create(&t->threads[iter], NULL, \
							be_philosopher, p_db[iter]);
		if (ret > 0)
		{
			free(t->threads);
			return (FALSE);
		}
		iter++;
	}
	return (TRUE);
}
