/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 06:53:47 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/30 21:38:11 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int	init_mprint(void)
{
	int			error;

	if ((error = mprint(-1, NULL)) < 0)
		return (error);
	return (0);
}

int	delete_mprint(void)
{
	int			error;

	if ((error = mprint(-2, NULL)) < 0)
		return (error);
	return (0);
}

int	mprint(int id, char *action)
{
	static pthread_mutex_t	mutex;
	static t_bool			is_stop;
	u_int64_t				cur_time;

	if (id == -1 && action == NULL && pthread_mutex_init(&mutex, NULL))
		return (MUTEX_ERROR);
	if (id == -2 && action == NULL && pthread_mutex_destroy(&mutex))
		return (MUTEX_ERROR);
	if (is_stop == TRUE || id < 0)
		return (0);
	if (pthread_mutex_lock(&mutex))
		return (MUTEX_ERROR);
	if (get_time(&cur_time) < 0)
		return (TIME_ERROR);
	if (is_stop == FALSE)
		if (printf("%"PRIu64" %d %s\n",
		cur_time - g_data->start_time, id, action) < 0)
			return (PRINTF_ERROR);
	if (action[0] == 'd')
		is_stop = TRUE;
	if (action[0] == 'd')
		return (0);
	if (pthread_mutex_unlock(&mutex))
		return (MUTEX_ERROR);
	return (0);
}
