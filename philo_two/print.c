/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 06:53:47 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/02 01:36:33 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

int	mprint(int id, char *action)
{
	u_int64_t		cur_time;

	if (sem_wait(g_data->sem_print) < 0)
		return (SEM_ERROR);
	if (get_time(&cur_time) < 0)
		return (TIME_ERROR);
	if (!get_done() &&
	printf("%"PRIu64" %d %s\n", cur_time - g_data->start_time, id, action) < 0)
		return (PRINTF_ERROR);
	if (sem_post(g_data->sem_print) < 0)
		return (SEM_ERROR);
	return (0);
}
