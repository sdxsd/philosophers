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

void	free_philosophers(t_philo **p_db, int n_philo)
{
	int	iter;

	iter = 0;
	if (n_philo == 1)
	{
		pthread_mutex_destroy(p_db[0]->r_fork);
		pthread_mutex_destroy(p_db[0]->l_fork);
		free(p_db[0]->r_fork);
		free(p_db[0]->l_fork);
		free(p_db[iter]);
		free(p_db);
		return ;
	}
	while (iter < n_philo)
	{
		pthread_mutex_destroy(p_db[iter]->l_fork);
		free(p_db[iter]->l_fork);
		free(p_db[iter]);
		iter++;
	}
	free(p_db);
}

void	*free_table(t_table *t)
{
	if (t->threads)
		join_threads(t->n_philo, t->threads);
	if (t->threads)
		free(t->threads);
	if (t->philo_db)
		free_philosophers(t->philo_db, t->n_philo);
	if (t->prnt_lck)
	{
		pthread_mutex_destroy(t->prnt_lck);
		free(t->prnt_lck);
	}
	if (t->philo_mutex)
	{
		pthread_mutex_destroy(t->philo_mutex);
		free(t->philo_mutex);
	}
	free(t);
	return (NULL);
}
