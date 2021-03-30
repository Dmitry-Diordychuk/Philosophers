/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 13:33:59 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/30 13:35:14 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int delete_forks(t_fork **forks, size_t n)
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

int delete_philos(t_philo **philos, size_t n)
{
	size_t i;

	i = 0;
	if (i < n)
	{
		i = 0;
		free(philos);
		i++;
	}
	return (MEM_ERROR);
}

int invite_philos(t_philo ***philos)
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

t_fork *serve_fork()
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

int set_table(t_philo **philos, t_fork ***forks)
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
	return (0);
}

int		start_threads(t_philo **philos)
{
	size_t i;

	i = 0;
	while (i < g_data->philos_num)
	{
		if (pthread_create(&philos[i]->thread, NULL, philo_live, philos[i]))
			return (THREAD_ERROR);
		i++;
	}
	return (0);
}

int	exit_handler(int ret, t_philo **philos, t_fork **forks)
{
	delete_mprint();
	if (philos)
		delete_philos(philos, g_data->philos_num);
	if (forks)
		delete_forks(forks, g_data->philos_num);
	if (g_data)
		free(g_data);
	return (ret);
}

int	main(int argc, char **argv)
{
	t_philo	**philos;
	t_fork	**forks;
	int		error;

	philos = NULL;
	if ((error = init_mprint()) < 0)
		return (error);
	if (!(g_data = (t_data*)malloc(sizeof(t_data))))
		return (exit_handler(MEM_ERROR, NULL, NULL));
	if ((error = parse(argc, argv, &g_data)) < 0)
		return (exit_handler(MEM_ERROR, NULL, NULL));
	if ((error = invite_philos(&philos)) < 0)
		return (exit_handler(MEM_ERROR, NULL, NULL));
	if ((error = set_table(philos, &forks)) < 0)
		return (exit_handler(MEM_ERROR, philos, NULL));
	if (start_threads(philos) < 0)
		return (exit_handler(THREAD_ERROR, philos, forks));
	if ((error = monitor(philos)) < 0)
		return (exit_handler(THREAD_ERROR, philos, forks));
	return (exit_handler(TIME_ERROR, philos, forks));
}
