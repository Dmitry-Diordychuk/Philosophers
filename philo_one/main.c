/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 13:33:59 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/31 20:31:52 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int		start_threads(t_philo **philos, pthread_t **death_timers)
{
	size_t		i;

	if (!(*death_timers = (pthread_t*)malloc(sizeof(pthread_t))))
		return (MEM_ERROR);
	i = 0;
	if (get_time(&g_data->start_time) < 0)
		return (TIME_ERROR);
	while (i < g_data->philos_num)
	{
		if (pthread_create(&philos[i]->thread, NULL, philo_live, philos[i]))
			return (THREAD_ERROR);
		if (
		pthread_create(&(*death_timers)[i], NULL, run_death_timer, philos[i]))
			return (THREAD_ERROR);
		i++;
	}
	return (0);
}

int		wait_threads(pthread_t *death_timers)
{
	size_t i;

	i = 0;
	while (i < g_data->philos_num)
	{
		if (pthread_join(death_timers[i], NULL))
			return (THREAD_ERROR);
		i++;
	}
	if (pthread_mutex_destroy(&g_data->mutex_print))
		return (MUTEX_ERROR);
	if (pthread_mutex_destroy(&g_data->mutex_done))
		return (MUTEX_ERROR);
	return (0);
}

int		exit_handler(int ret, t_philo **philos, t_fork **forks, pthread_t *dts)
{
	usleep(1000);
	if (philos)
		delete_philos(philos, g_data->philos_num);
	if (forks)
		delete_forks(forks, g_data->philos_num);
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
	if (!(g_data = (t_data*)malloc(sizeof(t_data))))
		return (exit_handler(MEM_ERROR, NULL, NULL, NULL));
	if ((error = parse(argc, argv, &g_data)) < 0)
		return (exit_handler(error, NULL, NULL, NULL));
	if (pthread_mutex_init(&g_data->mutex_done, NULL))
		return (exit_handler(MUTEX_ERROR, NULL, NULL, NULL));
	if (pthread_mutex_init(&g_data->mutex_print, NULL))
		return (exit_handler(MUTEX_ERROR, NULL, NULL, NULL));
	if ((error = invite_philos(&philos)) < 0)
		return (exit_handler(error, NULL, NULL, NULL));
	if ((error = set_table(philos, &forks)) < 0)
		return (exit_handler(error, philos, NULL, NULL));
	if ((error = start_threads(philos, &death_timers) < 0))
		return (exit_handler(error, philos, forks, NULL));
	if ((error = wait_threads(death_timers)) < 0)
		return (exit_handler(error, philos, forks, death_timers));
	return (exit_handler(0, philos, forks, death_timers));
}
