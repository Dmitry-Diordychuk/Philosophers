/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 13:33:59 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/02 17:43:38 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

int		exit_handler(int ret, t_philo **philos)
{
	if (philos)
		delete_philos(philos, g_data->philos_num);
	return (ret);
}

int		init_sems(t_data *data)
{
	if ((data->sem_done = sem_open("pDone", O_CREAT | O_EXCL, 644, 1)) ==
	SEM_FAILED)
		return (SEM_ERROR);
	sem_unlink("pDone");
	if ((data->sem_print = sem_open("pPrint", O_CREAT | O_EXCL, 644, 1)) ==
	SEM_FAILED)
		return (SEM_ERROR);
	sem_unlink("pPrint");
	if ((data->sem_forks = sem_open("pFork", O_CREAT | O_EXCL, 644,
	data->philos_num)) == SEM_FAILED)
		return (SEM_ERROR);
	sem_unlink("pFork");
	if ((data->sem_ration = sem_open("pRation", O_CREAT | O_EXCL, 644, 0)) ==
	SEM_FAILED)
		return (SEM_ERROR);
	sem_unlink("pRation");
	if ((data->sem_death = sem_open("pDeath", O_CREAT | O_EXCL, 644, 0)) ==
	SEM_FAILED)
		return (SEM_ERROR);
	sem_unlink("pDeath");
	return (0);
}

int		main(int argc, char **argv)
{
	int			error;
	t_data		data;
	t_philo		**philos;

	if ((error = parse(argc, argv, &data)) < 0)
		return (exit_handler(error, NULL));
	g_data = &data;
	if (init_sems(&data) < 0)
		return (exit_handler(SEM_ERROR, NULL));
	if ((error = invite_philos(&philos)) < 0)
		return (exit_handler(SEM_ERROR, NULL));
	if ((error = start_processes(philos, data) < 0))
		return (exit_handler(error, philos));
	if ((error = wait_processes(philos, data)) < 0)
		return (exit_handler(error, philos));
	return (exit_handler(21, philos));
}
