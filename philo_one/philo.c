/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 18:48:25 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/30 18:15:46 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int trap(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutex_pass);
	pthread_mutex_unlock(&philo->mutex_pass);
	return (0);
}

void	*philo_live(void *args)
{
	uint64_t		start_time;
	t_philo			*philo;
	int				error;

	philo = (t_philo*)args;
	if (get_time(&start_time) < 0)
		return (NULL);
	philo->thread_start_time = start_time;
	pthread_mutex_lock(&philo->mutex_meal);
	philo->last_meal_time = start_time;
	pthread_mutex_unlock(&philo->mutex_meal);
	trap(philo);
	while (INFINITE_LOOP)
	{
		if ((error = mprint(philo->thread_start_time, philo->id, "is thinking")) < 0)
			return (NULL);
		status(philo, THINK);
		if ((error = philo_search_forks(philo)) < 0)
			return (NULL);
		if (!(error = philo_eat(philo)))
			break ;
		if (error < 0)
			return (NULL);
		if ((error = philo_sleep(philo)) < 0)
			return (NULL);
		trap(philo);
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
	if (pthread_mutex_init(&new_philo->mutex_pass, NULL))
	{
		free(new_philo);
		return (NULL);
	}
	pthread_mutex_lock(&new_philo->mutex_pass);
	if (pthread_mutex_init(&new_philo->mutex_status, NULL))
	{
		free(new_philo);
		return (NULL);
	}
	new_philo->id = i + 1;
	new_philo->left_hand = EMPTY;
	new_philo->right_hand = EMPTY;
	new_philo->meals = 0;
	new_philo->status = NOT_STARTED;
	i++;
	return (new_philo);
}
