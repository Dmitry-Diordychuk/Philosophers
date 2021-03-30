/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 06:53:47 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/31 01:33:04 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int	mprint(int id, char *action)
{
	u_int64_t		cur_time;

	if (pthread_mutex_lock(&g_data->mutex_print))
		return (MUTEX_ERROR);
	if (get_time(&cur_time) < 0)
		return (TIME_ERROR);
	if (printf("%"PRIu64" %d %s\n", cur_time - g_data->start_time, id, action) < 0)
		return (PRINTF_ERROR);
	if (pthread_mutex_unlock(&g_data->mutex_print))
		return (MUTEX_ERROR);
	return (0);
}
