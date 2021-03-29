/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 13:33:48 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/29 17:11:44 by kdustin          ###   ########.fr       */
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
	size_t			number_of_philosophers;
	struct timeval	time_to_die;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
	uint64_t		number_of_times_each_philosopher_must_eat;
	t_bool			last_argument;
	struct timeval	start_time;
}					t_data;

/*
**	Libft functions
*/

int					ft_isdigit(int c);
uint64_t			ft_atoi(const char *str);
uint64_t			convert_uint(struct timeval time);
struct timeval		convert(uint64_t ms);
int					sub_time(struct timeval *z, struct timeval x,
												struct timeval y);

/*
**	Parse
*/

int					parse(int argc, char **argv, t_data **ret_data);

/*
**	Philosopher
*/

# define EMPTY 0
# define FORK 1
# define PHILO -1

typedef int			t_place;

# define LAST 1

typedef int			t_flag;
typedef t_bool		t_hand;

typedef struct		s_philo
{
	int				id;
	t_bool			is_dead;
	struct timeval	last_meal_time;
	t_hand			left;
	t_hand			right;
	uint64_t		time_from_last_meal;
}					t_philo;

t_philo				*invite_philo();
void				*philo_live(void *args);

/*
**	Table
*/

typedef struct		s_table
{
	t_data			*data;
	t_philo			*philo;
	pthread_mutex_t	mutex;
	t_place			place;
	t_flag			flag;
	struct s_table	*left;
	struct s_table	*right;
	pthread_t		thread;
}					t_table;

typedef t_table		t_null;

int					start_threads(t_table *table);
int					init_table(t_data *data, t_table **ret_table);
int					delete_table(t_table **table, t_table **cur);
int					delete_list_reverse(t_table **table);
int					set_position(int i, t_table **position, t_data *data);

/*
**	Print
*/

int					init_mprint();
int					delete_mprint();
int					mprint(struct timeval start_time, int id, char *action);

/*
**	Death monitor
*/

int					monitor_death_status(t_table *table);

/*
**	Action
*/

int		left_hand_search(t_table *position, struct timeval start_time);
int		philo_search_forks(t_table *position, struct timeval start_time);
int		put_forks_down(t_table *position);
int		philo_eat(t_table *position, struct timeval start_time);
int		philo_sleep(t_table *position, struct timeval start_time);

/*
**	Arbitration
*/

void	*arbitrate(void *args);

#endif
