/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waiter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 01:50:57 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/31 13:49:57 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int count_forks(t_fork **forks)
{
	size_t counter;
	size_t i;

	i = 0;
	counter = 0;
	while (i < g_data->philos_num)
	{
		pthread_mutex_lock(&forks[i]->mutex);
		counter += forks[i]->slot;
		pthread_mutex_unlock(&forks[i]->mutex);
		i++;
	}
	return (counter);
}

int waiter_serve(t_philo **philos, t_fork **forks)
{
	size_t	forks_counter;
	size_t	i;

	while (!get_done())
	{
		i = 0;
		while (i < g_data->philos_num)
		{
			pthread_mutex_lock(&philos[i]->mutex_request);
			if (philos[i]->request == LEFT || philos[i]->request == RIGHT)
			{
				forks_counter = count_forks(forks);
				if (forks_counter > 1)
					philos[i]->request = ALLOW;
				else if (forks_counter > 0 && philos[i]->request == RIGHT)
					philos[i]->request = ALLOW;
				else
					philos[i]->request = DENY;
			}
			pthread_mutex_unlock(&philos[i]->mutex_request);
			i++;
		}
	}
	return (0);
}
