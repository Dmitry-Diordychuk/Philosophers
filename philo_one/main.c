/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 13:33:59 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/30 22:07:34 by kdustin          ###   ########.fr       */
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

t_fork	*serve_fork()
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
	return (0);
}

int		start_threads(t_philo **philos, pthread_t **death_timers)
{
	size_t		i;

	if (!(*death_timers = (pthread_t*)malloc(sizeof(pthread_t))))
		return (MEM_ERROR);
	set_done(FALSE);
	i = 0;
	get_time(&g_data->start_time);
	while (i < g_data->philos_num)
	{
		if (pthread_create(&philos[i]->thread, NULL, philo_live, philos[i]))
			return (THREAD_ERROR);
		if (pthread_create(&(*death_timers)[i], NULL, run_death_timer, philos[i]))
			return (THREAD_ERROR);
		i++;
	}
	return (0);
}

int		exit_handler(int ret, t_philo **philos, t_fork **forks, pthread_t *dts)
{
	delete_mprint();
	if (philos)
		delete_philos(philos, g_data->philos_num);
	if (forks)
		delete_forks(forks, g_data->philos_num);
	size_t i;
	i = -1;
	while (++i < g_data->philos_num)
		pthread_detach(dts[i]);
	free(dts);
	if (g_data)
		free(g_data);
	return (ret);
}

int		main(int argc, char **argv)
{
	t_philo		**philos;
	t_fork		**forks;
	pthread_t	*death_timers;
	int			error;

	philos = NULL;
	if ((error = init_mprint()) < 0)
		return (error);
	if (!(g_data = (t_data*)malloc(sizeof(t_data))))
		return (exit_handler(MEM_ERROR, NULL, NULL, NULL));
	if ((error = parse(argc, argv, &g_data)) < 0)
		return (exit_handler(error, NULL, NULL, NULL));
	if (pthread_mutex_init(&g_data->mutex_done, NULL))
		return (exit_handler(MUTEX_ERROR, NULL, NULL, NULL));
	if ((error = invite_philos(&philos)) < 0)
		return (exit_handler(error, NULL, NULL, NULL));
	if ((error = set_table(philos, &forks)) < 0)
		return (exit_handler(error, philos, NULL, NULL));
	if ((error = start_threads(philos, &death_timers) < 0))
		return (exit_handler(error, philos, forks, NULL));
	while (!get_done())
		usleep(10000);
	if (pthread_mutex_destroy(&g_data->mutex_done))
		return (exit_handler(MUTEX_ERROR, NULL, NULL, NULL));
	return (exit_handler(0, philos, forks, death_timers));
}
