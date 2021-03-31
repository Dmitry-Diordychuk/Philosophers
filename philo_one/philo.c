/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 18:48:25 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/31 16:29:31 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void	*philo_live(void *args)
{
	uint64_t		time;
	t_philo			*philo;
	int				error;

	philo = (t_philo*)args;
	pthread_mutex_lock(&philo->mutex_meal);
	if (get_time(&time) < 0)
		return (NULL);
	philo->last_meal_time = time;
	pthread_mutex_unlock(&philo->mutex_meal);
	while (!get_done())
	{
		if ((error = mprint(philo->id, "is thinking")) < 0)
			return (NULL);
		if ((error = philo_search_forks(philo)) < 0)
			return (NULL);
		if (!(error = philo_eat(philo)))
			break ;
		if (error < 0)
			return (NULL);
		if ((error = philo_sleep(philo)) < 0)
			return (NULL);
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
	if (pthread_mutex_init(&new_philo->mutex_request, NULL))
	{
		free(new_philo);
		return (NULL);
	}
	new_philo->request = 0;
	new_philo->id = i + 1;
	new_philo->left_hand = EMPTY;
	new_philo->right_hand = EMPTY;
	new_philo->meals_counter = 0;
	new_philo->is_counted = FALSE;
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
		free(philos);
		i++;
	}
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
