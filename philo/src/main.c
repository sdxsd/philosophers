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

int	death_occurred(t_philo *p, t_table *t)
{
	size_t	starvation;

	starvation = time_since(p->t_since_meal, exact_time());
	if (starvation > t->time_to_die)
	{
		printf("%ld %ld has died\n", time_since(t->epoch, exact_time()), p->index + 1);
		pthread_mutex_unlock(&p->self_lck);
		return (TRUE);
	}
	return (FALSE);
}

int	check_philo(t_table *t)
{
	size_t	sated_count;
	t_philo	*nietzche;

	nietzche = t->philo_db[0];
	while (nietzche->r_philo)
	{
		pthread_mutex_lock(&nietzche->self_lck);
		if (nietzche->t_eaten > t->p_to_eat)
			sated_count++;
		else
			sated_count = 0;
		if (death_occurred(nietzche, t))
			return (DEATH);
		pthread_mutex_unlock(&nietzche->self_lck);
		nietzche = nietzche->r_philo;
		if (sated_count == t->n_philo)
			break ;
	}
	if (t->meal_limit)
		return (SATED);
	else
		return (CONTINUE);
}

/* Big brother is always watching  */
void	big_brother(t_table *table)
{
	int		ret;

	while (TRUE)
	{
		ret = check_philo(table);
		if (ret == SATED || ret == DEATH)
		{
			pthread_mutex_lock(&table->tbl_lck);
			table->death= TRUE;
			pthread_mutex_unlock(&table->tbl_lck);
			free_table(table);
			break ;
		}
	}
}

int	construct_table(t_table *table, int args, char **argv)
{
	if (args > 6 || args < 5)
		exit (0);
	if (!chk_args(argv + 1))
		exit (0);
	table->death = FALSE;
	table->n_philo = ft_atoi(argv[0]);
	table->time_to_die = ft_atoi(argv[1]);
	table->time_to_eat = ft_atoi(argv[2]);
	table->time_to_sleep = ft_atoi(argv[3]);
	table->meal_limit = args - 3;
	if (args)
		table->p_to_eat = ft_atoi(argv[4]);
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
	return (SUCCESS);
}

int	main(int argc, char	*argv[])
{
	t_table	table;

	construct_table(&table, argc - 1, argv + 1);
	table.philo_db = init_philosophers(table.n_philo);
	if (!table.philo_db && table.n_philo > 2)
		return (FAILURE);
	if (!init_threads(table.n_philo, table.philo_db, &table))
	{
		free_table(&table);
		return (-1);
	}
	big_brother(&table);
	return (0);
}
