/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 18:48:25 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/29 16:38:13 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void	*philo_live(void *args)
{
	struct timeval	start_time;
	t_table			*position;
	int				error;

	position = (t_table*)args;
	if (gettimeofday(&start_time, NULL) < 0)
		return (NULL);
	start_time = position->data->start_time;
	position->philo->last_meal_time = start_time;
	while (INFINITE_LOOP)
	{
		if ((error = philo_search_forks(position, start_time)) < 0)
			return (NULL);
		if (!(error = philo_eat(position, start_time)))
			break ;
		if (error < 0)
			return (NULL);
		if (position->philo->is_dead)
			break ;
		if ((error = philo_sleep(position, start_time)) < 0)
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
	new_philo->id = i + 1;
	new_philo->left = EMPTY;
	new_philo->right = EMPTY;
	i++;
	return (new_philo);
}
