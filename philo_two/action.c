/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 15:33:49 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/01 22:57:35 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

int		philo_search_forks(t_philo *philo)
{
	int		error;

	if (sem_wait(g_data->sem_forks) < 0)
		return (SEM_ERROR);
	if ((error = mprint(philo->id, "has taken a fork")) < 0)
		return (error);
	if (sem_wait(g_data->sem_forks) < 0)
		return (SEM_ERROR);
	if ((error = mprint(philo->id, "has taken a fork")) < 0)
		return (error);
	return (0);
}

int		put_forks_down(void)
{
	if (sem_post(g_data->sem_forks) < 0)
		return (SEM_ERROR);
	if (sem_post(g_data->sem_forks) < 0)
		return (SEM_ERROR);
	return (0);
}

int		philo_eat(t_philo *philo)
{
	int	error;

	if ((error = mprint(philo->id, "is eating")) < 0)
		return (error);
	if (go_sleep(g_data->time_to_eat * 1000))
		return (SLEEP_ERROR);
	if (set_meal(philo) < 0)
		return (SEM_ERROR);
	if (put_forks_down() < 0)
		return (SEM_ERROR);
	philo->meals_counter++;
	if (g_data->last_argument)
		if (philo->meals_counter == g_data->max_eat)
		{
			if (done_counter() < 0)
				return (SEM_ERROR);
		}
	return (TRUE);
}

int		philo_sleep(t_philo *philo)
{
	int		error;

	if ((error = mprint(philo->id, "is sleeping")) < 0)
		return (error);
	if (go_sleep(g_data->time_to_sleep * 1000))
		return (TIME_ERROR);
	return (0);
}
