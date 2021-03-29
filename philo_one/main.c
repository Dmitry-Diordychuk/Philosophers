/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 13:33:59 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/29 11:55:16 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int	wait_for_threads(t_table *table)
{
	int status;

	if (table->flag == LAST)
		table = table->right;
	while (table->flag != LAST)
	{
		if (table->place == PHILO)
		{
			status = pthread_join(table->thread, NULL);
			if (status != 0)
			{
				printf("Can't create thread, status = %d\n", status);
				exit(ERROR);
			}
		}
		table = table->right;
	}
	return (0);
}

int main(int argc, char **argv)
{
	t_data			*data;
	t_table			*table;

	if (init_mprint() < 0)
		return (ERROR);
	if (!(data = (t_data*)malloc(sizeof(t_data))))
	{
		delete_mprint();
		return (ERROR);
	}
	if (parse(argc, argv, &data) < 0)
	{
		delete_mprint();
		free(data);
		return (ERROR);
	}
	if (!(table = init_table(data)))
	{
		delete_mprint();
		free(data);
		return (ERROR);
	}
	gettimeofday(&table->data->start_time, NULL);
	start_threads(table);
	monitor_death_status(table);
	wait_for_threads(table);
	delete_table(&table, NULL);
	delete_mprint();
	free(data);
	return (0);
}
