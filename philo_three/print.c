/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 06:53:47 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/03 02:00:27 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

int	mprint(int id, char *action, int n)
{
	u_int64_t		cur_time;
	u_int64_t		time;

	sem_wait(g_data->sem_print);
	get_time(&cur_time);
	time = cur_time - g_data->start_time;
	if (!g_data->is_done && n == 2)
		printf("%"PRIu64" %d %s\n%"PRIu64" %d %s\n", time, id, action,
														time, id, action);
	else if (!g_data->is_done)
		printf("%"PRIu64" %d %s\n", time, id, action);
	sem_post(g_data->sem_print);
	return (0);
}
