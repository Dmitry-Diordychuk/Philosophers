/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 06:53:47 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/29 12:21:10 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int init_mprint()
{
	struct timeval time;

	gettimeofday(&time, NULL);
	if (mprint(time, -1, NULL) < 0)
		return (ERROR);
	return (0);
}

int delete_mprint()
{
	struct timeval time;

	gettimeofday(&time, NULL);
	if (mprint(time, -2, NULL) < 0)
		return (ERROR);
	return (0);
}

int mprint(struct timeval start_time, int id, char *action)
{
	static pthread_mutex_t	mutex;
	static t_bool			is_stop;
	struct timeval			cur_time;
	struct timeval			result;

	if (id == -1 && action == NULL)
	{
		if (pthread_mutex_init(&mutex, NULL))
			return (ERROR);
	}
	else if (id == -2 && action == NULL)
	{
		if (pthread_mutex_destroy(&mutex))
			return (ERROR);
	}
	else if (is_stop == FALSE)
	{
		pthread_mutex_lock(&mutex);
		if (is_stop == FALSE)
		{
			gettimeofday(&cur_time, NULL);
			if (!sub_time(&result, cur_time, start_time))
				printf("%"PRIu64" %d %s\n", convert_uint(result), id, action);
			if (action[0] == 'd')
				is_stop = TRUE;
		}
		pthread_mutex_unlock(&mutex);
	}
	return (0);
}
