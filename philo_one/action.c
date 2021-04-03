/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 15:33:49 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/03 01:20:10 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int		philo_think(t_philo *philo)
{
	mprint(philo->id, "is thinking", 1);
	pthread_mutex_lock(&philo->left_fork->mutex_pair_one);
	pthread_mutex_lock(&philo->left_fork->mutex_pair_two);
	pthread_mutex_lock(&philo->left_fork->mutex);
	pthread_mutex_lock(&philo->right_fork->mutex);
	philo->right_fork->slot = EMPTY;
	philo->left_fork->slot = EMPTY;
	mprint(philo->id, "has taken a fork", 2);
	return (0);
}

int		philo_eat(t_philo *philo)
{
	mprint(philo->id, "is eating", 1);
	get_time(&philo->last_meal_time);
	usleep(g_data->time_to_eat * 1000);
	philo->right_fork->slot = FORK;
	philo->left_fork->slot = FORK;
	pthread_mutex_unlock(&philo->right_fork->mutex);
	pthread_mutex_unlock(&philo->left_fork->mutex);
	pthread_mutex_unlock(&philo->left_fork->mutex_pair_one);
	pthread_mutex_unlock(&philo->left_fork->mutex_pair_two);
	philo->meals_counter++;
	if (g_data->last_argument)
		if (philo->meals_counter == g_data->max_eat)
			g_data->done_counter++;
	return (TRUE);
}

int		philo_sleep(t_philo *philo)
{
	mprint(philo->id, "is sleeping", 1);
	usleep(g_data->time_to_sleep * 1000);
	return (0);
}
