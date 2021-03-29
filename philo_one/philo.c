/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 18:48:25 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/29 11:55:55 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void philo_search_forks(t_table *position, struct timeval start_time)
{
	t_philo	*philo;

	philo = position->philo;
	while (philo->left == EMPTY || philo->right == EMPTY)
	{
		if (philo->id == 1 && philo->right == EMPTY)
		pthread_mutex_lock(&position->left->mutex);
		if (position->left->place == FORK)
		{
			mprint(start_time, philo->id, "has taken a fork");
			position->left->place = EMPTY;
			philo->left = FORK;
		}
		pthread_mutex_unlock(&position->left->mutex);
		pthread_mutex_lock(&position->right->mutex);
		if (position->right->place == FORK)
		{
			mprint(start_time, philo->id, "has taken a fork");
			position->right->place = EMPTY;
			philo->right = FORK;
		}
		pthread_mutex_unlock(&position->right->mutex);
	}
}

t_bool philo_eat(t_table *position, struct timeval start_time)
{
	static uint64_t	counter;
	t_philo			*philo;

	philo = position->philo;
	if (position->data->last_argument &&
	counter >= position->data->number_of_times_each_philosopher_must_eat)
	{
		pthread_mutex_lock(&position->left->mutex);
		position->left->place = FORK;
		philo->left = EMPTY;
		pthread_mutex_unlock(&position->left->mutex);
		pthread_mutex_lock(&position->right->mutex);
		position->right->place = FORK;
		position->right = EMPTY;
		pthread_mutex_unlock(&position->right->mutex);
		return (FALSE);
	}
	philo = position->philo;
	mprint(start_time, philo->id, "is eating");
	usleep(position->data->time_to_eat * 1000);
	pthread_mutex_lock(&position->mutex);
	gettimeofday(&position->philo->last_meal_time, NULL);
	pthread_mutex_unlock(&position->mutex);
	counter++;
	pthread_mutex_lock(&position->left->mutex);
	position->left->place = FORK;
	pthread_mutex_unlock(&position->left->mutex);
	pthread_mutex_lock(&position->right->mutex);
	position->right->place = FORK;
	pthread_mutex_unlock(&position->right->mutex);
	return (TRUE);
}

void philo_sleep(t_table *position, struct timeval start_time)
{
	t_philo	*philo;

	philo = position->philo;
	mprint(start_time, philo->id, "is sleeping");
	usleep(position->data->time_to_sleep * 1000);
	mprint(start_time, philo->id, "is thinking");
}

void *philo_live(void *args)
{
	struct timeval	start_time;
	t_table			*position;

	position = (t_table*)args;
	gettimeofday(&start_time, NULL);
	start_time = position->data->start_time;
	position->philo->last_meal_time = start_time;
	while (INFINITE_LOOP)
	{
		philo_search_forks(position, start_time);
		if (!philo_eat(position, start_time))
			break ;
		if (position->philo->is_dead)
			break ;
		philo_sleep(position, start_time);
	}
	return (NULL);
}

t_philo *invite_philo()
{
	t_philo		*new_philo;
	static int	i;

	if (!(new_philo = (t_philo*)malloc(sizeof(t_philo))))
		return (NULL);
	new_philo->id = i + 1;
	new_philo->left = EMPTY;
	new_philo->right = EMPTY;
	i++;
	return (new_philo);
}
