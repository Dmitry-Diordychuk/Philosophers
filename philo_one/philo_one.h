/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 13:33:48 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/31 13:03:30 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/time.h>
# include <pthread.h>
# include <inttypes.h>
# include <errno.h>

typedef int			t_bool;

# define TRUE 1
# define FALSE 0

# define PARSE_ERROR	-1
# define PRINTF_ERROR	-2
# define MEM_ERROR		-3
# define TIME_ERROR		-4
# define MUTEX_ERROR	-5
# define THREAD_ERROR	-6
# define MATH_ERROR		-8
# define SLEEP_ERROR	-9
# define ERROR			-10

# define INFINITE_LOOP 1
typedef struct		s_data
{
	size_t			philos_num;
	uint64_t		time_to_die;
	uint64_t		time_to_eat;
	uint64_t		time_to_sleep;
	uint64_t		max_eat;
	t_bool			last_argument;
	uint64_t		start_time;
	pthread_mutex_t	mutex_done;
	pthread_mutex_t	mutex_print;
	t_bool			is_done;
	size_t			done_counter;
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

typedef int			t_slot;

typedef struct		s_fork
{
	size_t			id;
	t_slot			slot;
	pthread_mutex_t	mutex;
}					t_fork;

typedef t_bool		t_hand;

typedef struct		s_philo
{
	size_t			id;
	pthread_t		thread;
	pthread_mutex_t	mutex_meal;
	uint64_t		last_meal_time;
	size_t			meals_counter;
	t_bool			is_counted;
	t_hand			left_hand;
	t_hand			right_hand;
	t_fork			*left_fork;
	t_fork			*right_fork;
	pthread_mutex_t	mutex_request;
	int				request;
}					t_philo;

void	set_request(t_philo *philo, int request);
int		get_response(t_philo *philo);

t_philo				*invite_philo();
int					invite_philos(t_philo ***philos);
int					delete_philos(t_philo **philos, size_t n);
void				*philo_live(void *args);

/*
**	Print
*/

int					mprint(int id, char *action);

/*
**	Action
*/

int					left_hand_search(t_philo *philo);
int					philo_search_forks(t_philo *philo);
int					put_forks_down(t_philo *philo);
int					philo_eat(t_philo *philo);
int					philo_sleep(t_philo *philo);

/*
**	Death timer
*/

void				*run_death_timer(void *args);
int					get_done();
void				set_done(t_bool bool);

int waiter_serve(t_philo **philos, t_fork **forks);

#endif
