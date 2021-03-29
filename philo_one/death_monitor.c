/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_monitor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 20:26:44 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/29 12:24:18 by kdustin          ###   ########.fr       */
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
		pthread_mutex_lock(&table->mutex);
		last_meal = table->philo->last_meal_time;
		gettimeofday(&cur_time, NULL);
		if (sub_time(&sub_result, cur_time, last_meal))
			return (ERROR);
		if (!sub_time(NULL, sub_result, time_to_die))
		{
			mprint(table->data->start_time, table->philo->id, "died");
			pthread_mutex_unlock(&table->mutex);
			kill_all(table);
			return (0);
		}
		pthread_mutex_unlock(&table->mutex);
		table = table->right->right;
	}
	return (ERROR);
}
