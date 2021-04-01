/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 13:33:59 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/01 18:58:45 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

int		start_threads(t_philo **philos, pthread_t **death_timers)
{
	size_t		i;

	if (!(*death_timers = (pthread_t*)malloc(sizeof(pthread_t) *
	g_data->philos_num)))
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
	sem_close(g_data->sem_done);
	sem_close(g_data->sem_print);
	sem_close(g_data->sem_forks);
	return (0);
}

int		exit_handler(int ret, t_philo **philos, pthread_t *dts)
{
	if (philos)
		delete_philos(philos, g_data->philos_num);
	if (dts)
		free(dts);
	if (g_data)
		free(g_data);
	return (ret);
}

int		init_sems(void)
{
	if ((g_data->sem_done = sem_open("pDone", O_CREAT | O_EXCL, 644, 1)) ==
	SEM_FAILED)
		return (SEM_ERROR);
	sem_unlink("pDone");
	if ((g_data->sem_print = sem_open("pPrint", O_CREAT | O_EXCL, 644, 1)) ==
	SEM_FAILED)
		return (SEM_ERROR);
	sem_unlink("pPrint");
	if ((g_data->sem_forks = sem_open("pFork", O_CREAT | O_EXCL, 644,
	g_data->philos_num)) == SEM_FAILED)
		return (SEM_ERROR);
	sem_unlink("pFork");
	return (0);
}

int		main(int argc, char **argv)
{
	t_philo		**philos;
	pthread_t	*death_timers;
	int			error;

	philos = NULL;
	death_timers = NULL;
	if (!(g_data = (t_data*)malloc(sizeof(t_data))))
		return (exit_handler(MEM_ERROR, NULL, NULL));
	if ((error = parse(argc, argv, &g_data)) < 0)
		return (exit_handler(error, NULL, NULL));
	if (init_sems() < 0)
		return (exit_handler(SEM_ERROR, NULL, NULL));
	if ((error = invite_philos(&philos)) < 0)
		return (exit_handler(error, NULL, NULL));
	if ((error = start_threads(philos, &death_timers) < 0))
		return (exit_handler(error, philos, NULL));
	if ((error = wait_threads(death_timers)) < 0)
		return (exit_handler(error, philos, death_timers));
	return (exit_handler(0, philos, death_timers));
}
