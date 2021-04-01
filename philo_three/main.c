/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 13:33:59 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/01 14:17:33 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

int		exit_handler(int ret, t_philo **philos)
{
	usleep(1000);
	if (philos)
		delete_philos(philos, g_data->philos_num);
	if (g_data)
		free(g_data);
	return (ret);
}

int		start_processes(t_philo **philos)
{
	size_t		i;
	pthread_t	death_timer;

	i = 0;
	get_time(&g_data->start_time);
	while (i < g_data->philos_num)
	{
		philos[i]->pid = fork();
		if (philos[i]->pid == 0)
		{
			if (!(death_timer = (pthread_t)malloc(sizeof(pthread_t))))
				return (MEM_ERROR);
			if (pthread_create(&death_timer, NULL, run_death_timer, philos[i]))
				return (THREAD_ERROR);
			philo_live(philos[i]);
			pthread_join(death_timer, NULL);
			exit(exit_handler(42, philos));
		}
		i++;
	}
	return (0);
}

int		wait_processes(t_philo **philos)
{
	int			status;
	size_t		i;
	pthread_t	counter;

	if (g_data->last_argument)
	{
		if (pthread_create(&counter, NULL, run_counter, NULL))
			return (THREAD_ERROR);
		if (pthread_detach(counter))
			return (THREAD_ERROR);
	}
	i = 0;
	while (!waitpid(-1, &status, WNOHANG) && !get_done())
		;
	i = 0;
	while (i < g_data->philos_num)
	{
		kill(philos[i]->pid, SIGKILL);
		i++;
	}
	sem_close(g_data->sem_done);
	sem_close(g_data->sem_print);
	sem_close(g_data->sem_forks);
	sem_close(g_data->sem_ration);
	return (0);
}

int		init_sems(void)
{
	if ((g_data->sem_done = sem_open("pDone", O_CREAT | O_EXCL, 644, 1)) ==
	SEM_FAILED)
		return (exit_handler(SEM_ERROR, NULL));
	sem_unlink("pDone");
	if ((g_data->sem_print = sem_open("pPrint", O_CREAT | O_EXCL, 644, 1)) ==
	SEM_FAILED)
		return (exit_handler(SEM_ERROR, NULL));
	sem_unlink("pPrint");
	if ((g_data->sem_forks = sem_open("pFork", O_CREAT | O_EXCL, 644,
	g_data->philos_num)) == SEM_FAILED)
		return (exit_handler(SEM_ERROR, NULL));
	sem_unlink("pFork");
	if ((g_data->sem_ration = sem_open("pRation", O_CREAT | O_EXCL, 644, 0)) ==
	SEM_FAILED)
		return (exit_handler(SEM_ERROR, NULL));
	sem_unlink("pRation");
	return (0);
}

int		main(int argc, char **argv)
{
	t_philo		**philos;
	int			error;

	philos = NULL;
	if (!(g_data = (t_data*)malloc(sizeof(t_data))))
		return (exit_handler(MEM_ERROR, NULL));
	if ((error = parse(argc, argv, &g_data)) < 0)
		return (exit_handler(error, NULL));
	if (init_sems() < 0)
		return (exit_handler(SEM_ERROR, NULL));
	if ((error = invite_philos(&philos)) < 0)
		return (exit_handler(error, NULL));
	if ((error = start_processes(philos) < 0))
		return (exit_handler(error, philos));
	if ((error = wait_processes(philos)) < 0)
		return (exit_handler(error, philos));
	return (exit_handler(21, philos));
}
