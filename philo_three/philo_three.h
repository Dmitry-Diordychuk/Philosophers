/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 13:33:48 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/03 02:16:10 by kdustin          ###   ########.fr       */
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

/*
**	Philosopher
*/

typedef struct		s_philo
{
	size_t			id;
	int				pid;
	uint64_t		last_meal_time;
	size_t			meals_counter;
}					t_philo;

t_philo				*invite_philo();
int					invite_philos(t_philo ***philos);
int					delete_philos(t_philo **philos, size_t n);
int					philo_live(t_philo *philo);

/*
**	Data
*/

typedef struct		s_data
{
	size_t			philos_num;
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;
	uint64_t		max_eat;
	t_bool			last_argument;
	uint64_t		start_time;
	t_bool			is_done;
	sem_t			*sem_print;
	sem_t			*sem_ration;
	sem_t			*sem_death;
	sem_t			*sem_forks;
	t_philo			**philos;
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

int					parse(int argc, char **argv, t_data *ret_data);

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
void				*run_death_detector(void *args);
void				*run_counter(void *args);

/*
**	Processe
*/

int					start_processes(t_philo **philos, t_data data);
int					wait_processes(t_philo **philos, t_data data);

#endif
