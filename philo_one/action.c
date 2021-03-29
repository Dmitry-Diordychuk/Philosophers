/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 15:33:49 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/29 15:59:12 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int		left_hand_search(t_table *position, struct timeval start_time)
{
	int		error;
	t_philo	*philo;

	philo = position->philo;
	if (pthread_mutex_lock(&position->left->mutex))
		return (MUTEX_ERROR);
	if (position->left->place == FORK)
	{
		if ((error = mprint(start_time, philo->id, "has taken a fork")) < 0)
			return (error);
		position->left->place = EMPTY;
		philo->left = FORK;
	}
	if (pthread_mutex_unlock(&position->left->mutex))
		return (MUTEX_ERROR);
	return (0);
}

int		philo_search_forks(t_table *position, struct timeval start_time)
{
	int		error;
	t_philo	*philo;

	philo = position->philo;
	while (philo->left == EMPTY || philo->right == EMPTY)
	{
		if (philo->id == 1 && philo->right == EMPTY)
			if ((error = left_hand_search(position, start_time)) < 0)
				return (error);
		if (pthread_mutex_lock(&position->right->mutex))
			return (MUTEX_ERROR);
		if (position->right->place == FORK)
		{
			if ((error = mprint(start_time, philo->id, "has taken a fork")) < 0)
				return (error);
			position->right->place = EMPTY;
			philo->right = FORK;
		}
		if (pthread_mutex_unlock(&position->right->mutex))
			return (MUTEX_ERROR);
	}
	return (0);
}

int		put_forks_down(t_table *position)
{
	t_philo			*philo;

	philo = position->philo;
	if (pthread_mutex_lock(&position->left->mutex))
		return (MUTEX_ERROR);
	position->left->place = FORK;
	philo->left = EMPTY;
	if (pthread_mutex_unlock(&position->left->mutex))
		return (MUTEX_ERROR);
	if (pthread_mutex_lock(&position->right->mutex))
		return (MUTEX_ERROR);
	position->right->place = FORK;
	philo->right = EMPTY;
	if (pthread_mutex_unlock(&position->right->mutex))
		return (MUTEX_ERROR);
	return (0);
}

int		philo_eat(t_table *position, struct timeval start_time)
{
	static uint64_t	counter;
	t_philo			*philo;
	int				error;

	philo = position->philo;
	if ((error = mprint(start_time, philo->id, "is eating")) < 0)
		return (error);
	if (usleep(position->data->time_to_eat * 1000))
		return (SLEEP_ERROR);
	if (pthread_mutex_lock(&position->mutex))
		return (MUTEX_ERROR);
	if (gettimeofday(&position->philo->last_meal_time, NULL) < 0)
		return (TIME_ERROR);
	if (pthread_mutex_unlock(&position->mutex))
		return (MUTEX_ERROR);
	counter++;
	if (put_forks_down(position) < 0)
		return (MUTEX_ERROR);
	return (TRUE);
}

int		philo_sleep(t_table *position, struct timeval start_time)
{
	t_philo	*philo;
	int		error;

	philo = position->philo;
	if ((error = mprint(start_time, philo->id, "is sleeping")) < 0)
		return (error);
	if (usleep(position->data->time_to_sleep * 1000))
		return (TIME_ERROR);
	if ((error = mprint(start_time, philo->id, "is thinking")) < 0)
		return (error);
	return (0);
}
