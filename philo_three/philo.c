/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 18:48:25 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/03 02:26:36 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

static int	start_threads(t_philo *philo)
{
	pthread_t		thread;

	if (pthread_create(&thread, NULL, run_death_timer, philo))
		return (THREAD_ERROR);
	if (pthread_detach(thread))
		return (THREAD_ERROR);
	return (0);
}

int			philo_live(t_philo *philo)
{
	int				error;

	if (start_threads(philo) < 0)
		return (THREAD_ERROR);
	if (philo == NULL)
		return (MEM_ERROR);
	if (get_time(&philo->last_meal_time) < 0)
		return (TIME_ERROR);
	if (philo->id % 2)
		if (usleep(3000))
			return (TIME_ERROR);
	while (!g_data->is_done)
	{
		if ((error = philo_think(philo)) < 0)
			return (error);
		if ((error = philo_eat(philo)) <= 0)
			return (error);
		if ((error = philo_sleep(philo)) < 0)
			return (error);
	}
	return (ERROR);
}

t_philo		*invite_philo(void)
{
	t_philo		*new_philo;
	static int	i;

	if (!(new_philo = (t_philo*)malloc(sizeof(t_philo))))
		return (NULL);
	new_philo->id = i + 1;
	new_philo->meals_counter = 0;
	i++;
	return (new_philo);
}

int			delete_philos(t_philo **philos, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
	{
		free(philos[i]);
		i++;
	}
	free(philos);
	return (MEM_ERROR);
}

int			invite_philos(t_philo ***philos)
{
	size_t	i;

	if (!((*philos) = (t_philo**)malloc(sizeof(t_philo*) * g_data->philos_num)))
		return (MEM_ERROR);
	i = 0;
	while (i < g_data->philos_num)
	{
		if (!((*philos)[i] = invite_philo()))
			return (delete_philos((*philos), i));
		i++;
	}
	return (0);
}
