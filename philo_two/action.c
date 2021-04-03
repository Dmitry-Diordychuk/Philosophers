/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 15:33:49 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/03 01:47:30 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

int		philo_think(t_philo *philo)
{
	mprint(philo->id, "is thinking", 1);
	sem_wait(g_data->sem_forks);
	sem_wait(g_data->sem_forks);
	mprint(philo->id, "has taken a fork", 2);
	return (0);
}

int		philo_eat(t_philo *philo)
{
	mprint(philo->id, "is eating", 1);
	usleep(g_data->time_to_eat * 1000);
	get_time(&philo->last_meal_time);
	sem_post(g_data->sem_forks);
	sem_post(g_data->sem_forks);
	philo->meals_counter++;
	if (g_data->last_argument)
		if (philo->meals_counter == g_data->max_eat)
			g_data->done_counter++;
	return (0);
}

int		philo_sleep(t_philo *philo)
{
	mprint(philo->id, "is sleeping", 1);
	usleep(g_data->time_to_sleep * 1000);
	return (0);
}
