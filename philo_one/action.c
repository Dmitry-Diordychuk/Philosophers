/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 15:33:49 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/31 13:54:19 by kdustin          ###   ########.fr       */
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
		if (pthread_mutex_unlock(&fork->mutex))
			return (MUTEX_ERROR);
		return (TRUE);
	}
	if (pthread_mutex_unlock(&fork->mutex))
		return (MUTEX_ERROR);
	return (FALSE);
}

void	set_request(t_philo *philo, int request)
{
	pthread_mutex_lock(&philo->mutex_request);
	philo->request = request;
	pthread_mutex_unlock(&philo->mutex_request);
}

int		get_response(t_philo *philo)
{
	int res;

	pthread_mutex_lock(&philo->mutex_request);
	res = philo->request;
	if (res == ALLOW || res == DENY)
	{
		pthread_mutex_unlock(&philo->mutex_request);
		return (res);
	}
	pthread_mutex_unlock(&philo->mutex_request);
	return (0);
}

int		philo_search_forks(t_philo *philo)
{
	int		error;
	int		response;

	while (philo->left_hand == EMPTY && philo->right_hand == EMPTY)
	{
		while (philo->left_hand == EMPTY)
		{
			set_request(philo, LEFT);
			while (!(response = get_response(philo)))
			{
				if (get_done())
					return (ERROR);
			}
			if (response == ALLOW)
			{
				if (search_fork(philo->left_fork, &philo->left_hand))
					if ((error = mprint(philo->id, "has taken a fork")) < 0)
						return (error);
			}
			else
				break ;
		}
		while (philo->right_hand == EMPTY)
		{
			set_request(philo, RIGHT);
			while (!(response = get_response(philo)))
			{
				if (get_done())
					return (ERROR);
			}
			if (response == ALLOW)
			{
				if (search_fork(philo->right_fork, &philo->right_hand))
					if ((error = mprint(philo->id, "has taken a fork")) < 0)
						return (error);
			}
			else
				break ;
		}
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
	if (g_data->done_counter == g_data->max_eat)
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
	int				error;

	if ((error = mprint(philo->id, "is eating")) < 0)
		return (error);
	if (usleep(g_data->time_to_eat * 1000))
		return (SLEEP_ERROR);
	set_meal(philo);
	if (!philo->is_counted && g_data->last_argument && philo->meals_counter > g_data->max_eat)
	{
		philo->is_counted = TRUE;
		if (done_counter())
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
