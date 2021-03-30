/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 15:33:49 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/30 22:27:24 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int		hand_search(t_philo *philo, t_fork *fork, int type)
{
	int		error;

	if (pthread_mutex_lock(&fork->mutex))
		return (MUTEX_ERROR);
	if (fork->slot == FORK)
	{
		if ((error = mprint(philo->id, "has taken a fork")) < 0)
			return (error);
		fork->slot = EMPTY;
		if (type == LEFT)
			philo->left_hand = FORK;
		else
			philo->right_hand = FORK;
	}
	if (pthread_mutex_unlock(&fork->mutex))
		return (MUTEX_ERROR);
	return (0);
}

int		philo_search_forks(t_philo *philo)
{
	int		error;

	if ((error = hand_search(philo, philo->left_fork, LEFT)) < 0)
		return (error);
	if ((error = hand_search(philo, philo->right_fork, RIGHT)) < 0)
		return (error);
	return (0);
}

int		put_forks_down(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->left_fork->mutex))
		return (MUTEX_ERROR);
	philo->left_fork->slot = FORK;
	philo->left_hand = EMPTY;
	if (pthread_mutex_unlock(&philo->left_fork->mutex))
		return (MUTEX_ERROR);
	if (pthread_mutex_lock(&philo->right_fork->mutex))
		return (MUTEX_ERROR);
	philo->right_fork->slot = FORK;
	philo->right_hand = EMPTY;
	if (pthread_mutex_unlock(&philo->right_fork->mutex))
		return (MUTEX_ERROR);
	return (0);
}

int set_meal(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->mutex_meal))
		return (MUTEX_ERROR);
	if (get_time(&philo->last_meal_time) < 0)
		return (TIME_ERROR);
	philo->meals_counter++;
	if (pthread_mutex_unlock(&philo->mutex_meal))
		return (MUTEX_ERROR);
	return (0);
}

int		philo_eat(t_philo *philo)
{
	int	error;

	if ((error = mprint(philo->id, "is eating")) < 0)
		return (error);
	if (usleep(g_data->time_to_eat * 1000))
		return (SLEEP_ERROR);
	set_meal(philo);
	if (g_data->last_argument && philo->meals_counter > g_data->max_eat)
	{
		write(2, "Done\n", 5);
		return (ERROR);
	}
	if (put_forks_down(philo) < 0)
		return (MUTEX_ERROR);
	return (TRUE);
}

int		philo_sleep(t_philo *philo)
{
	int		error;

	if ((error = mprint(philo->id, "is sleeping")) < 0)
		return (error);
	if (usleep(g_data->time_to_sleep * 1000))
		return (TIME_ERROR);
	return (0);
}
