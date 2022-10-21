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
# define SUCCESS 1
# define FAILURE 0

enum e_states {
	EATING,
	THINKING,
	SLEEPING,
	TKE_FORK,
};

enum e_philo_status {
	CONTINUE,
	DEATH,
	SATED,
};

typedef struct s_philo {
	struct s_philo	*l_philo;
	struct s_philo	*r_philo;
	size_t			t_eaten;
	size_t			t_since_meal;
	size_t			state;
	size_t			idx;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	r_fork;
	pthread_mutex_t	self_lck;
	void			*table;
}	t_philo;

typedef struct s_table {
	pthread_mutex_t	tbl_lck;
	pthread_t		*threads;
	t_philo			**philo_db;
	size_t			epoch;
	size_t			n_philo;
	size_t			time_to_die;
	size_t			time_to_sleep;
	size_t			time_to_eat;
	size_t			p_to_eat;
	int				meal_limit;
	bool			death;
}	t_table;

/* THREAD FUNCTIONS */
int		init_threads(int n_philo, t_philo **p_db, t_table *t);
void	join_threads(int n_philo, pthread_t *thread_db);

/* PHILOSOPHER FUNCTIONS */
t_philo	**init_philosophers(int n_philos);
void	ps(size_t milsec, size_t state, size_t index, t_table *t);
void	*be_philosopher(void *p);
void	check_death(t_philo *p, t_table *t);

/* MONITORING FUNCTIONS */
void	big_brother(t_table *t, t_philo *nietzche);

/* TIME FUNCTIONS */
size_t	exact_time(void);
size_t	ts(size_t epoch, size_t current);
void	acc_usleep(t_philo *p, t_table *t, size_t milliseconds);

/* DEALLOCATION FUNCTIONS */
void	free_philosophers(t_philo **p_db, int n_philo);
int		free_table(t_table *t);

/* UTILITY FUNCTIONS */
int		chk_args(char **args);
long	ft_atoi(const char *str);

#endif // PHILOSOPHERS_H
