/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 15:33:49 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/31 01:34:53 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int	search_fork(t_fork *fork, t_hand *hand)
{
	if (pthread_mutex_lock(&fork->mutex))
		return (MUTEX_ERROR);
	if (fork->slot == FORK)
	{
		*hand = FORK;
		fork->slot = EMPTY;
	}
	if (pthread_mutex_unlock(&fork->mutex))
		return (MUTEX_ERROR);
	return (0);
}

int		philo_search_forks(t_philo *philo)
{
	int		error;

	if (philo->id != g_data->philos_num)
	{
		while (philo->left_hand == EMPTY)
			search_fork(philo->left_fork, &philo->left_hand);
		if ((error = mprint(philo->id, "has taken a fork")) < 0)
			return (error);
	}
	while (philo->right_hand == EMPTY)
		search_fork(philo->right_fork, &philo->right_hand);
	if ((error = mprint(philo->id, "has taken a fork")) < 0)
		return (error);
	if (philo->id == g_data->philos_num)
	{
		while (philo->left_hand == EMPTY)
			search_fork(philo->left_fork, &philo->left_hand);
		if ((error = mprint(philo->id, "has taken a fork")) < 0)
			return (error);
	}
	return (0);
}

int		put_forks_down(t_philo *philo)
{
	if (pthread_mutex_lock(&philo->right_fork->mutex))
		return (MUTEX_ERROR);
	philo->right_fork->slot = FORK;
	philo->right_hand = EMPTY;
	if (pthread_mutex_unlock(&philo->right_fork->mutex))
		return (MUTEX_ERROR);
	if (pthread_mutex_lock(&philo->left_fork->mutex))
		return (MUTEX_ERROR);
	philo->left_fork->slot = FORK;
	philo->left_hand = EMPTY;
	if (pthread_mutex_unlock(&philo->left_fork->mutex))
		return (MUTEX_ERROR);
	return (0);
}

int		set_meal(t_philo *philo)
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

t_bool	done_counter()
{
	pthread_mutex_lock(&g_data->mutex_done);
	g_data->done_counter++;
	if (g_data->done_counter == g_data->philos_num)
	{
		g_data->is_done = TRUE;
		pthread_mutex_unlock(&g_data->mutex_done);
		return (TRUE);
	}
	pthread_mutex_unlock(&g_data->mutex_done);
	return (FALSE);
}

int		philo_eat(t_philo *philo)
{
	int	error;

	if ((error = mprint(philo->id, "is eating")) < 0)
		return (error);
	if (usleep(g_data->time_to_eat * 1000))
		return (SLEEP_ERROR);
	set_meal(philo);
	//if (g_data->last_argument && philo->meals_counter > g_data->max_eat)
	//	if (done_counter())
	//		return (ERROR);
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
