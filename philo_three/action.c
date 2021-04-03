/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 15:33:49 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/03 02:23:10 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

int		philo_think(t_philo *philo)
{
	int		error;

	if ((error = mprint(philo->id, "is thinking", 1)) < 0)
		return (error);
	if (sem_wait(g_data->sem_forks) < 0)
		return (SEM_ERROR);
	if (sem_wait(g_data->sem_forks) < 0)
		return (SEM_ERROR);
	if ((error = mprint(philo->id, "has taken a fork", 2)) < 0)
		return (error);
	return (0);
}

int		philo_eat(t_philo *philo)
{
	int	error;

	if ((error = mprint(philo->id, "is eating", 1)) < 0)
		return (error);
	if (get_time(&philo->last_meal_time) < 0)
		return (TIME_ERROR);
	if (usleep(g_data->time_to_eat * 1000))
		return (SLEEP_ERROR);
	if (sem_post(g_data->sem_forks) < 0)
		return (SEM_ERROR);
	if (sem_post(g_data->sem_forks) < 0)
		return (SEM_ERROR);
	philo->meals_counter++;
	if (g_data->last_argument)
		if (philo->meals_counter == g_data->max_eat)
			sem_post(g_data->sem_ration);
	return (TRUE);
}

int		philo_sleep(t_philo *philo)
{
	int		error;

	if ((error = mprint(philo->id, "is sleeping", 1)) < 0)
		return (error);
	if (usleep(g_data->time_to_sleep * 1000))
		return (TIME_ERROR);
	return (0);
}
