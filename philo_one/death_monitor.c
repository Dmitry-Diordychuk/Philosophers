/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 20:26:44 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/29 14:46:10 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int	kill_all(t_table *table)
{
	t_table *cur;

	cur = table;
	if (table->place == PHILO)
		table->philo->is_dead = TRUE;
	table = table->right;
	while (table != cur)
	{
		if (table->place == PHILO)
			table->philo->is_dead = TRUE;
		table = table->right;
	}
	return (0);
}

int	die(t_table *table)
{
	int error;

	if ((error = mprint(table->data->start_time, table->philo->id, "died")) < 0)
		return (PRINTF_ERROR);
	if (pthread_mutex_unlock(&table->mutex))
		return (MUTEX_ERROR);
	return (kill_all(table));
}

int	monitor_death_status(t_table *table)
{
	struct timeval	cur_time;
	struct timeval	last_meal;
	struct timeval	sub_result;
	struct timeval	time_to_die;

	if (table->place != PHILO)
		table = table->right;
	time_to_die = table->data->time_to_die;
	while (table)
	{
		if (pthread_mutex_lock(&table->mutex))
			return (MUTEX_ERROR);
		last_meal = table->philo->last_meal_time;
		if (gettimeofday(&cur_time, NULL) < 0)
			return (TIME_ERROR);
		if (sub_time(&sub_result, cur_time, last_meal))
			return (MATH_ERROR);
		if (!sub_time(NULL, sub_result, time_to_die))
			return (die(table));
		if (pthread_mutex_unlock(&table->mutex))
			return (MUTEX_ERROR);
		table = table->right->right;
	}
	return (ERROR);
}
