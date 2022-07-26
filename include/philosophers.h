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

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <pthread.h>
# include <stdbool.h>
# define TRUE 1
# define FALSE 0

enum e_states {
	eating,
	thinking,
	sleeping,
};

typedef struct s_philo {
	size_t			eat_cnt;
	size_t			state;
	size_t			index;
	size_t			hunger;
	struct s_philo	*l_philo;
	struct s_philo	*r_philo;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	void			*table;
} t_philo;

typedef struct	s_table {
	pthread_mutex_t	*prnt_lck;
	t_philo			**philo_db;
	size_t			n_philo;
	size_t			time_to_die;
	size_t			time_to_sleep;
	size_t			time_to_eat;
	size_t			eat_count;
	bool			death;
} t_table;

/* THREAD FUNCTIONS */
int	init_threads(int n_philo, t_philo **p_db, t_table *t);

/* PHILOSOPHER FUNCTIONS */
t_philo	**init_philosophers(int n_philos);
t_philo	*init_philosopher(t_philo *l_philo, t_philo *r_philo, int index);
void	print_state(size_t milsec, size_t state, size_t index, t_table *t);
void	*be_philosopher(void *p);

/* TIME FUNCTIONS */
size_t	exact_time(void);
size_t	sec_to_mil(int seconds);
size_t	mic_to_mil(int mic_seconds);

/* UTILITY FUNCTIONS */
int	ft_atoi(const char *str);

# endif // PHILOSOPHERS_H
