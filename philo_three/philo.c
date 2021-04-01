/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 18:48:25 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/01 14:18:47 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void	*philo_live(void *args)
{
	uint64_t		time;
	t_philo			*philo;
	int				error;

	philo = (t_philo*)args;
	if (sem_wait(philo->sem_meal) < 0)
		set_done(ERROR);
	if (get_time(&time) < 0)
		set_done(ERROR);
	philo->last_meal_time = time;
	if (sem_post(philo->sem_meal) < 0)
		set_done(ERROR);
	while (!get_done())
	{
		if ((error = mprint(philo->id, "is thinking")) < 0)
			set_done(ERROR);
		if ((error = philo_search_forks(philo)) < 0)
			set_done(ERROR);
		if (!(error = philo_eat(philo)))
			return (NULL);
		error < 0 ? set_done(ERROR) : FALSE;
		if ((error = philo_sleep(philo)) < 0)
			set_done(ERROR);
	}
	return (NULL);
}

t_philo	*invite_philo(void)
{
	t_philo		*new_philo;
	static int	i;

	if (!(new_philo = (t_philo*)malloc(sizeof(t_philo))))
		return (NULL);
	if ((new_philo->sem_meal = sem_open("pMeal", O_CREAT | O_EXCL, 644, 1)) ==
	SEM_FAILED)
	{
		free(new_philo);
		return (NULL);
	}
	sem_unlink("pMeal");
	new_philo->id = i + 1;
	new_philo->meals_counter = 0;
	i++;
	return (new_philo);
}

int		delete_philos(t_philo **philos, size_t n)
{
	size_t i;

	i = 0;
	if (i < n)
	{
		i = 0;
		sem_close(philos[i]->sem_meal);
		free(philos[i]);
		i++;
	}
	free(philos);
	return (MEM_ERROR);
}

int		invite_philos(t_philo ***philos)
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
