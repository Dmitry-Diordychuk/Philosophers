/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 18:48:25 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/03 01:52:39 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

void	*philo_live(void *args)
{
	t_philo			*philo;

	philo = (t_philo*)args;
	get_time(&philo->last_meal_time);
	if (philo->id % 2)
		usleep(3000);
	while (!g_data->is_done)
	{
		philo_think(philo);
		philo_eat(philo);
		philo_sleep(philo);
	}
	return (NULL);
}

t_philo	*invite_philo(void)
{
	t_philo		*new_philo;
	static int	i;

	if (!(new_philo = (t_philo*)malloc(sizeof(t_philo))))
		return (NULL);
	sem_unlink("pMeal");
	new_philo->id = i + 1;
	new_philo->meals_counter = 0;
	i++;
	return (new_philo);
}

int		delete_philos(t_philo **philos, size_t n)
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

int		invite_philos(t_philo ***philos)
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
