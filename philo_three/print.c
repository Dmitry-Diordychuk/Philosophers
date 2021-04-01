/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/28 06:53:47 by kdustin           #+#    #+#             */
/*   Updated: 2021/04/01 16:35:37 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

int	mprint(int id, char *action, uint64_t start_time, sem_t *sem_print)
{
	u_int64_t		cur_time;

	if (sem_wait(sem_print) < 0)
		return (SEM_ERROR);
	if (get_time(&cur_time) < 0)
		return (TIME_ERROR);
	if (
	printf("%"PRIu64" %d %s\n", cur_time - start_time, id, action) < 0)
		return (PRINTF_ERROR);
	if (action[0] == 'd')
		return (0);
	if (sem_post(sem_print) < 0)
		return (SEM_ERROR);
	return (0);
}
