/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/02 17:22:00 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/03 02:30:27 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

static int	fork_error(t_philo **philos, t_data data, size_t i)
{
	while (--i > 0)
		kill(philos[i]->pid, SIGKILL);
	kill(philos[i]->pid, SIGKILL);
	sem_close(data.sem_print);
	sem_close(data.sem_forks);
	sem_close(data.sem_ration);
	return (ERROR);
}

static int	fork_main(t_philo **philos, t_data data, size_t i)
{
	int error;

	if (!(g_data = (t_data*)malloc(sizeof(t_data))))
		exit(MEM_ERROR);
	g_data = &data;
	g_data->is_done = FALSE;
	g_data->philos = philos;
	if ((error = philo_live(philos[i])) < 0)
		exit(error);
	delete_philos(philos, g_data->philos_num);
	sem_close(g_data->sem_forks);
	sem_close(g_data->sem_print);
	free(g_data);
	exit(0);
}

int			start_processes(t_philo **philos, t_data data)
{
	size_t		i;

	if (get_time(&data.start_time) < 0)
		return (TIME_ERROR);
	i = -1;
	while (++i < data.philos_num)
	{
		philos[i]->pid = fork();
		if (philos[i]->pid == -1)
			return (fork_error(philos, data, i));
		if (philos[i]->pid == 0)
			fork_main(philos, data, i);
	}
	return (0);
}

int			wait_processes(t_philo **philos, t_data data)
{
	int			status;
	pthread_t	counter;
	size_t		i;

	g_data->is_done = FALSE;
	if (data.last_argument)
	{
		if (pthread_create(&counter, NULL, run_counter, NULL))
			return (THREAD_ERROR);
		if (pthread_detach(counter))
			return (THREAD_ERROR);
	}
	if (pthread_create(&counter, NULL, run_death_detector, NULL))
		return (THREAD_ERROR);
	if (pthread_detach(counter))
		return (THREAD_ERROR);
	while (!waitpid(-1, &status, WNOHANG) && !g_data->is_done)
		;
	i = -1;
	while (++i < g_data->philos_num)
		kill(philos[i]->pid, SIGKILL);
	sem_close(data.sem_print);
	sem_close(data.sem_forks);
	sem_close(data.sem_ration);
	return (0);
}
