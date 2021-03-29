/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 06:53:47 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/29 14:42:03 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int	init_mprint(void)
{
	struct timeval	time;
	int				error;

	if (gettimeofday(&time, NULL) < 0)
		return (TIME_ERROR);
	if ((error = mprint(time, -1, NULL)) < 0)
		return (error);
	return (0);
}

int	delete_mprint(void)
{
	struct timeval	time;
	int				error;

	if (gettimeofday(&time, NULL) < 0)
		return (TIME_ERROR);
	if ((error = mprint(time, -2, NULL)) < 0)
		return (error);
	return (0);
}

int	mprint(struct timeval start_time, int id, char *action)
{
	static pthread_mutex_t	mutex;
	static t_bool			is_stop;
	struct timeval			cur_time;
	struct timeval			result;

	if (id == -1 && action == NULL)
		if (pthread_mutex_init(&mutex, NULL))
			return (MUTEX_ERROR);
	if (id == -2 && action == NULL)
		if (pthread_mutex_destroy(&mutex))
			return (MUTEX_ERROR);
	if (is_stop == TRUE || id < 0)
		return (0);
	if (pthread_mutex_lock(&mutex))
		return (MUTEX_ERROR);
	if (gettimeofday(&cur_time, NULL) < 0)
		return (TIME_ERROR);
	if (is_stop == FALSE && !sub_time(&result, cur_time, start_time))
		if (printf("%"PRIu64" %d %s\n", convert_uint(result), id, action) < 0)
			return (PRINTF_ERROR);
	if (action[0] == 'd')
		is_stop = TRUE;
	if (pthread_mutex_unlock(&mutex))
		return (MUTEX_ERROR);
	return (0);
}
