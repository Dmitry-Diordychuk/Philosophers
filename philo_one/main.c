/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 13:33:59 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/29 16:42:14 by kdustin          ###   ########.fr       */
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
				return (THREAD_ERROR);
		}
		table = table->right;
	}
	return (0);
}

int	exit_handler(t_data *data, t_table *table, int ret)
{
	delete_mprint();
	if (data)
		free(data);
	if (table)
		delete_table(&table, NULL);
	return (ret);
}

int	main(int argc, char **argv)
{
	t_data	*data;
	t_table	*table;
	int		error;

	if ((error = init_mprint()) < 0)
		return (error);
	if (!(data = (t_data*)malloc(sizeof(t_data))))
		return (exit_handler(NULL, NULL, MEM_ERROR));
	if ((error = parse(argc, argv, &data)) < 0)
		return (exit_handler(data, NULL, error));
	if ((error = init_table(data, &table)) < 0)
		return (exit_handler(data, NULL, error));
	if (gettimeofday(&table->data->start_time, NULL) < 0)
		return (exit_handler(data, table, TIME_ERROR));
	if (start_threads(table) < 0)
		return (exit_handler(data, table, THREAD_ERROR));
	if ((error = monitor_death_status(table)) < 0)
		return (exit_handler(data, table, error));
	if (wait_for_threads(table) < 0)
		return (exit_handler(data, table, THREAD_ERROR));
	if (delete_table(&table, NULL) < 0)
		return (exit_handler(data, table, MUTEX_ERROR));
	if (delete_mprint() < 0)
		return (exit_handler(data, table, TIME_ERROR));
	return (exit_handler(data, NULL, 0));
}
