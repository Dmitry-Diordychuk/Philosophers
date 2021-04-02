/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 18:48:25 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/02 17:07:16 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void	*philo_live(void *args)
{
	uint64_t		time;
	t_philo			*philo;
	int				error;

	philo = (t_philo*)args;
	if (pthread_mutex_lock(&philo->mutex_meal))
		set_done(ERROR);
	if (get_time(&time) < 0)
		set_done(ERROR);
	philo->last_meal_time = time;
	if (pthread_mutex_unlock(&philo->mutex_meal))
		set_done(ERROR);
	philo->id % 2 ? go_sleep(1000) : FALSE;
	while (!get_done())
	{
		if ((error = mprint(philo->id, "is thinking")) < 0)
			set_done(ERROR);
		if ((error = philo_search_forks(philo)) < 0)
			set_done(ERROR);
		if ((error = philo_eat(philo)) <= 0)
			set_done(ERROR);
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
	if (pthread_mutex_init(&new_philo->mutex_meal, NULL))
	{
		free(new_philo);
		return (NULL);
	}
	new_philo->id = i + 1;
	new_philo->left_hand = EMPTY;
	new_philo->right_hand = EMPTY;
	new_philo->meals_counter = 0;
	i++;
	return (new_philo);
}

int		delete_philos(t_philo **philos, size_t n)
{
	size_t i;

	i = 0;
	while (i < n)
	{
		pthread_mutex_destroy(&philos[i]->mutex_meal);
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
