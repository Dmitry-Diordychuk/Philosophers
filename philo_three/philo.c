/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 18:48:25 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/01 19:14:08 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

int		run_death_time(t_philo *philo)
{
	pthread_t		death_timer;

	if (pthread_create(&death_timer, NULL, run_death_timer, philo))
		return (THREAD_ERROR);
	if (pthread_detach(death_timer))
		return (THREAD_ERROR);
	return (0);
}

int		philo_live(t_philo philo, t_data data)
{
	uint64_t		time;
	int				error;

	philo.start_time = data.start_time;
	if (run_death_time(&philo) < 0)
		return (THREAD_ERROR);
	if (sem_wait(&philo.sem_meal) < 0)
		return (SEM_ERROR);
	if (get_time(&time) < 0)
		return (TIME_ERROR);
	philo.last_meal_time = time;
	if (sem_post(&philo.sem_meal) < 0)
		return (SEM_ERROR);
	while (!get_done(&philo))
	{
		if ((error = mprint(philo.id, "is thinking", data.start_time,
		&data.sem_print)) < 0)
			return (error);
		if ((error = philo_search_forks(&philo, &data)) < 0)
			return (error);
		if ((error = philo_eat(&philo, &data)) <= 0 ||
		(error = philo_sleep(&philo, &data)) < 0)
			return (error);
	}
	return (0);
}

t_philo	*invite_philo(t_data data)
{
	t_philo		*new_philo;
	static int	i;
	sem_t		*temp;

	if (!(new_philo = (t_philo*)malloc(sizeof(t_philo))))
		return (NULL);
	if ((temp = sem_open("pMeal", O_CREAT | O_EXCL, 644, 1)) == SEM_FAILED)
	{
		free(new_philo);
		return (NULL);
	}
	new_philo->sem_meal = *temp;
	sem_unlink("pMeal");
	new_philo->id = i + 1;
	new_philo->meals_counter = 0;
	new_philo->time_to_die = data.time_to_die;
	new_philo->sem_done = data.sem_done;
	new_philo->sem_print = data.sem_print;
	i++;
	return (new_philo);
}

int		delete_philos(t_philo **philos, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
	{
		free(philos[i]);
		i++;
	}
	free(philos);
	return (MEM_ERROR);
}

int		invite_philos(t_philo ***philos, t_data data)
{
	size_t	i;

	if (!((*philos) = (t_philo**)malloc(sizeof(t_philo*) * data.philos_num)))
		return (MEM_ERROR);
	i = 0;
	while (i < data.philos_num)
	{
		if (!((*philos)[i] = invite_philo(data)))
			return (delete_philos((*philos), i));
		i++;
	}
	return (0);
}
