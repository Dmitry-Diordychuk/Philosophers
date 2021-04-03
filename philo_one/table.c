/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/31 16:30:17 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/03 01:29:22 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int		delete_forks(t_fork **forks, size_t n)
{
	size_t i;

	if (forks)
	{
		i = 0;
		while (i < n)
		{
			pthread_mutex_destroy(&forks[i]->mutex);
			free(forks[i++]);
		}
		free(forks);
	}
	return (MEM_ERROR);
}

t_fork	*serve_fork(void)
{
	static int	i;
	t_fork		*new_fork;

	if (!(new_fork = (t_fork*)malloc(sizeof(t_fork))))
		return (NULL);
	if (pthread_mutex_init(&new_fork->mutex, NULL))
	{
		free(new_fork);
		return (NULL);
	}
	new_fork->slot = FORK;
	new_fork->id = i + 1;
	i++;
	return (new_fork);
}

int		init_pair(t_philo **philos, size_t n)
{
	size_t i;

	i = 0;
	while (i < g_data->philos_num)
	{
		if (philos[i]->id % 2 == 1)
		{
			if (pthread_mutex_init(
			&philos[(i + 1) % n]->right_fork->mutex_pair_one, NULL))
				return (MUTEX_ERROR);
			philos[i]->left_fork->mutex_pair_one =
								philos[(i + 1) % n]->right_fork->mutex_pair_one;
		}
		else
		{
			if (pthread_mutex_init(
			&philos[(i + 1) % n]->right_fork->mutex_pair_two, NULL))
				return (MUTEX_ERROR);
			philos[i]->left_fork->mutex_pair_one =
								philos[(i + 1) % n]->right_fork->mutex_pair_one;
		}
		i++;
	}
	return (0);
}

int		set_table(t_philo **philos, t_fork ***forks)
{
	size_t	i;
	size_t	n;

	n = g_data->philos_num;
	if (!((*forks) = (t_fork**)malloc(sizeof(t_fork*) * n)))
		return (MEM_ERROR);
	i = 0;
	while (i < g_data->philos_num)
	{
		if (!((*forks)[i] = serve_fork()))
			return (delete_forks((*forks), i));
		philos[i]->right_fork = (*forks)[i];
		philos[(i + 1) % n]->left_fork = (*forks)[i];
		i++;
	}
	init_pair(philos, n);
	return (0);
}
