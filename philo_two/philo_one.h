/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 13:33:48 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/27 15:14:15 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <stdio.h>

typedef int t_bool;

# define TRUE 1
# define FALSE 0

# define ERROR -1

typedef struct	s_data
{
	int number_of_philosophers;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int number_of_times_each_philosopher_must_eat;
}				s_data;

/*
**	Libft functions
*/

int	ft_isdigit(int c);
int	ft_isspace(int c);
int	ft_atoi(const char *str);

/*
**	Parse
*/

int parse(int argc, char **argv, s_data *data);

#endif
