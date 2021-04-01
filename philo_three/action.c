/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 15:33:49 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/01 18:55:27 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

int		philo_search_forks(t_philo *philo, t_data *data)
{
	int		error;

	if (philo->id != data->philos_num)
	{
		if (sem_wait(&data->sem_forks) < 0)
			return (SEM_ERROR);
		if ((error = mprint(philo->id, "has taken a fork", data->start_time,
		&data->sem_print)) < 0)
			return (error);
	}
	if (sem_wait(&data->sem_forks) < 0)
		return (SEM_ERROR);
	if ((error = mprint(philo->id, "has taken a fork", data->start_time,
	&data->sem_print)) < 0)
		return (error);
	if (philo->id == data->philos_num)
	{
		if (sem_wait(&data->sem_forks) < 0)
			return (SEM_ERROR);
		if ((error = mprint(philo->id, "has taken a fork", data->start_time,
		&data->sem_print)) < 0)
			return (error);
	}
	return (0);
}

int		put_forks_down(t_data *data)
{
	if (sem_post(&data->sem_forks) < 0)
		return (SEM_ERROR);
	if (sem_post(&data->sem_forks) < 0)
		return (SEM_ERROR);
	return (0);
}

int		philo_eat(t_philo *philo, t_data *data)
{
	int	error;

	if ((error = mprint(philo->id, "is eating", data->start_time,
	&data->sem_print)) < 0)
		return (error);
	if (usleep(data->time_to_eat * 1000))
		return (SLEEP_ERROR);
	if (set_meal(philo) < 0)
		return (SEM_ERROR);
	if (put_forks_down(data) < 0)
		return (SEM_ERROR);
	philo->meals_counter++;
	if (data->last_argument)
		if (philo->meals_counter == data->max_eat)
			if (sem_post(&data->sem_ration) < 0)
				return (SEM_ERROR);
	return (TRUE);
}

int		philo_sleep(t_philo *philo, t_data *data)
{
	int		error;

	if ((error = mprint(philo->id, "is sleeping", data->start_time,
	&data->sem_print)) < 0)
		return (error);
	if (usleep(data->time_to_sleep * 1000))
		return (TIME_ERROR);
	return (0);
}
