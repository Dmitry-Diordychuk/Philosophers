/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 13:33:48 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/01 17:14:28 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_THREE_H
# define PHILO_THREE_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <fcntl.h>
# include <pthread.h>
# include <inttypes.h>
# include <errno.h>
# include <semaphore.h>

typedef int			t_bool;

# define TRUE 1
# define FALSE 0

# define PARSE_ERROR	-1
# define PRINTF_ERROR	-2
# define MEM_ERROR		-3
# define TIME_ERROR		-4
# define SEM_ERROR	-5
# define THREAD_ERROR	-6
# define MATH_ERROR		-8
# define SLEEP_ERROR	-9
# define ERROR			-10
# define INFINITE_LOOP	1

typedef struct		s_data
{
	size_t			philos_num;
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;
	uint64_t		max_eat;
	t_bool			last_argument;
	uint64_t		start_time;
	sem_t			sem_done;
	sem_t			sem_print;
	sem_t			sem_forks;
	sem_t			sem_ration;
}					t_data;

t_bool g_is_done;

/*
**	Libft functions
*/

int					ft_isdigit(int c);
uint64_t			ft_atoi(const char *str);
int					get_time(uint64_t *result);

/*
**	Parse
*/

int					parse(int argc, char **argv, t_data *ret_data);

/*
**	Philosopher
*/

# define EMPTY 0
# define FORK 1
# define LEFT 2
# define RIGHT 3
# define ALLOW 4
# define DENY 5

typedef struct		s_philo
{
	size_t			id;
	pid_t			pid;
	sem_t			sem_meal;
	uint64_t		last_meal_time;
	size_t			meals_counter;
	uint64_t		time_to_die;
	sem_t			sem_done;
	uint64_t		start_time;
	sem_t			sem_print;
}					t_philo;

t_philo				*invite_philo(t_data data);
int					invite_philos(t_philo ***philos, t_data data);
int					delete_philos(t_philo **philos, size_t n);
int					philo_live(t_philo philo, t_data data);

/*
**	Print
*/

int					mprint(int id, char *action, uint64_t start_time,
													sem_t *sem_print);

/*
**	Action
*/

int					philo_search_forks(t_philo *philo, t_data *data);
int					put_forks_down(t_data *data);
int					philo_eat(t_philo *philo, t_data *data);
int					philo_sleep(t_philo *philo, t_data *data);

/*
**	Death timer
*/

void				*run_death_timer(void *args);
void				*run_counter(void *args);

/*
**	Get set
*/

int					get_done(t_philo *philo);
int					set_done(t_bool bool, sem_t *sem_done);
uint64_t			get_meal_time(t_philo *philo);
int					set_meal(t_philo *philo);
t_bool				done_counter(t_data *data);

int					exit_handler(int ret, t_philo **philos, t_data data);

#endif
