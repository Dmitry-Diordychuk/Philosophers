/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 13:33:48 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/03 02:00:53 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_TWO_H
# define PHILO_TWO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/time.h>
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
	sem_t			*sem_print;
	t_bool			is_done;
	size_t			done_counter;
	sem_t			*sem_forks;
}					t_data;

t_data				*g_data;

/*
**	Libft functions
*/

int					ft_isdigit(int c);
uint64_t			ft_atoi(const char *str);
int					get_time(uint64_t *result);

/*
**	Parse
*/

int					parse(int argc, char **argv, t_data **ret_data);

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
	pthread_t		thread;
	uint64_t		last_meal_time;
	size_t			meals_counter;
}					t_philo;

t_philo				*invite_philo();
int					invite_philos(t_philo ***philos);
int					delete_philos(t_philo **philos, size_t n);
void				*philo_live(void *args);

/*
**	Print
*/

int					mprint(int id, char *action, int n);

/*
**	Action
*/

int					philo_think(t_philo *philo);
int					philo_eat(t_philo *philo);
int					philo_sleep(t_philo *philo);

/*
**	Death timer
*/

void				*run_death_timer(void *args);

#endif
