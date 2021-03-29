/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arbitration.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/29 16:38:33 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/29 17:35:38 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int get_philos(t_table *table, t_philo ***philos)
{
	size_t	i;

	if (table->flag == LAST)
		table = table->right;
	i = 0;
	while (table->flag != LAST)
	{
		if (table->place == PHILO)
			(*philos)[i++] = table->philo;
		table = table->right;
	}
	return (0);
}

int calc_starvation(size_t n, t_philo **philos)
{
	size_t i;
	struct timeval	cur_time;
	struct timeval	result;

	i = 0;
	while (i < n)
	{
		if (gettimeofday(&cur_time, NULL) < 0)
			return (TIME_ERROR);
		if (sub_time(&result, cur_time, philos[i]->last_meal_time))
			return (MATH_ERROR);
		philos[i]->time_from_last_meal = convert_uint(result);
	}
	return (0);
}

static void		swap(void **d1, void **d2)
{
	void *temp;

	temp = *d1;
	*d1 = *d2;
	*d2 = temp;
}

int sort_starvation(t_philo **philos, size_t pivot, size_t left, size_t right)
{
	while (1)
	{
		while (philos[left] < philos[pivot])
			left++;
		while (philos[right] >= philos[pivot] || left == right)
			right--;
		if (left == right)
		{
			swap(&philos[pivot], &philos[right]);
			continue ;
		}
		swap(&philos[left], &philos[right]);
	}
}

void *arbitrate(void *args)
{
	size_t	n;
	t_philo	**philos;
	t_table	*table;

	n = table->data->number_of_philosophers;
	table = (t_table*)args;
	if (!(philos = (t_philo**)malloc(sizeof(t_philo*) * n)))
		return (NULL);
	get_philos(table, &philos);
	while (1)
	{
		calc_starvation(n, philos);
		sort_starvation(philos, n - 1, 0, n - 2);
	}
}
