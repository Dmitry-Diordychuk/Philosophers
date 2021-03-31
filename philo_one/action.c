/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 15:33:49 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/31 18:06:20 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

t_bool	search_fork(t_fork *fork, t_hand *hand)
{
	if (pthread_mutex_lock(&fork->mutex))
		return (MUTEX_ERROR);
	if (fork->slot == FORK)
	{
		*hand = FORK;
		fork->slot = EMPTY;
		return (TRUE);
	}
	if (pthread_mutex_unlock(&fork->mutex))
		return (MUTEX_ERROR);
	return (FALSE);
}

int		philo_search_forks(t_philo *philo)
{
	int		error;

	if (philo->id != g_data->philos_num)
		while (!search_fork(philo->left_fork, &philo->left_hand))
			if ((error = mprint(philo->id, "has taken a fork")) < 0)
				return (error);
	while (!search_fork(philo->right_fork, &philo->right_hand))
		if ((error = mprint(philo->id, "has taken a fork")) < 0)
			return (error);
	if (philo->id == g_data->philos_num)
		while (!search_fork(philo->left_fork, &philo->left_hand))
			if ((error = mprint(philo->id, "has taken a fork")) < 0)
				return (error);
	return (0);
}

int		put_forks_down(t_philo *philo)
{
	philo->right_fork->slot = FORK;
	philo->right_hand = EMPTY;
	if (pthread_mutex_unlock(&philo->right_fork->mutex))
		return (MUTEX_ERROR);
	philo->left_fork->slot = FORK;
	philo->left_hand = EMPTY;
	if (pthread_mutex_unlock(&philo->left_fork->mutex))
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
	if (set_meal(philo) < 0)
		return (MUTEX_ERROR);
	if (put_forks_down(philo) < 0)
		return (MUTEX_ERROR);
	philo->meals_counter++;
	if (g_data->last_argument)
		if (!philo->is_counted)
			if (philo->meals_counter > g_data->max_eat)
			{
				if (done_counter() < 0)
					return (MUTEX_ERROR);
				philo->is_counted = TRUE;
			}
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
