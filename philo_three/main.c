/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 13:33:59 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/01 18:58:15 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

int		exit_handler(int ret, t_philo **philos, t_data data)
{
	if (philos)
		delete_philos(philos, data.philos_num);
	return (ret);
}

int		start_processes(t_philo **philos, t_data data)
{
	size_t		i;
	t_philo		in_philo;

	if (get_time(&data.start_time) < 0)
		return (TIME_ERROR);
	i = -1;
	while (++i < data.philos_num)
	{
		philos[i]->pid = fork();
		if (philos[i]->pid == 0)
		{
			in_philo = *(philos[i]);
			in_philo.sem_print = philos[i]->sem_print;
			in_philo.sem_done = philos[i]->sem_done;
			in_philo.sem_meal = philos[i]->sem_meal;
			delete_philos(philos, data.philos_num);
			philo_live(in_philo, data);
			sem_close(&philos[i]->sem_meal);
			sem_close(&philos[i]->sem_print);
			sem_close(&philos[i]->sem_done);
			free(philos[i]);
			exit(sem_close(&data.sem_forks));
		}
	}
	return (0);
}

int		wait_processes(t_philo **philos, t_data data)
{
	int			status;
	size_t		i;
	pthread_t	counter;

	if (data.last_argument)
	{
		if (pthread_create(&counter, NULL, run_counter, &data))
			return (THREAD_ERROR);
		if (pthread_detach(counter))
			return (THREAD_ERROR);
	}
	i = 0;
	while (!waitpid(-1, &status, WNOHANG) && !get_done(philos[0]))
		;
	i = -1;
	while (++i < data.philos_num)
	{
		kill(philos[i]->pid, SIGKILL);
		sem_close(&philos[i]->sem_meal);
	}
	sem_close(&data.sem_done);
	sem_close(&data.sem_print);
	sem_close(&data.sem_forks);
	sem_close(&data.sem_ration);
	return (0);
}

int		init_sems(t_data *data)
{
	sem_t *temp;

	if ((temp = sem_open("pDone", O_CREAT | O_EXCL, 644, 1)) ==
	SEM_FAILED)
		return (SEM_ERROR);
	sem_unlink("pDone");
	data->sem_done = *temp;
	if ((temp = sem_open("pPrint", O_CREAT | O_EXCL, 644, 1)) == SEM_FAILED)
		return (SEM_ERROR);
	sem_unlink("pPrint");
	data->sem_print = *temp;
	if ((temp = sem_open("pFork", O_CREAT | O_EXCL, 644, data->philos_num)) ==
	SEM_FAILED)
		return (SEM_ERROR);
	sem_unlink("pFork");
	data->sem_forks = *temp;
	if ((temp = sem_open("pRation", O_CREAT | O_EXCL, 644, 0)) ==
	SEM_FAILED)
		return (SEM_ERROR);
	sem_unlink("pRation");
	data->sem_ration = *temp;
	return (0);
}

int		main(int argc, char **argv)
{
	t_philo		**philos;
	int			error;
	t_data		data;

	philos = NULL;
	if ((error = parse(argc, argv, &data)) < 0)
		return (exit_handler(error, NULL, data));
	if (init_sems(&data) < 0)
		return (exit_handler(SEM_ERROR, NULL, data));
	if ((error = invite_philos(&philos, data)) < 0)
		return (exit_handler(error, NULL, data));
	if ((error = start_processes(philos, data) < 0))
		return (exit_handler(error, philos, data));
	if ((error = wait_processes(philos, data)) < 0)
		return (exit_handler(error, philos, data));
	return (exit_handler(21, philos, data));
}
