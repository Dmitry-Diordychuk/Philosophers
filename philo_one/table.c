/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   table.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kdustin <kdustin@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/27 20:13:43 by kdustin           #+#    #+#             */
/*   Updated: 2021/03/29 09:33:33 by kdustin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

int	start_threads(t_table *table)
{
	int status;

	if (table->flag == LAST)
		table = table->right;
	while (table->flag != LAST)
	{
		if (table->place == PHILO)
		{
			status = pthread_create(&table->thread, NULL, philo_live, table);
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

int	set_position(int i, t_table **pos, t_data *data)
{
	if (i % 2)
	{
		if (!((*pos)->philo = invite_philo()))
			return (ERROR);
		(*pos)->place = PHILO;
		if (pthread_mutex_init(&(*pos)->mutex, NULL))
		{
			free((*pos)->philo);
			return (ERROR);
		}
		(*pos)->data = data;
		return (0);
	}
	else
	{
		(*pos)->flag = EMPTY;
		if (i == 0)
			(*pos)->flag = LAST;
		(*pos)->philo = NULL;
		(*pos)->place = FORK;
		if (pthread_mutex_init(&(*pos)->mutex, NULL))
			return (ERROR);
		(*pos)->data = data;
	}
	return (0);
}

t_null	*delete_list_reverse(t_table **table)
{
	t_table *temp;

	while (*table != NULL)
	{
		temp = (*table)->left;
		if ((*table)->place == PHILO)
			free((*table)->philo);
		pthread_mutex_destroy(&(*table)->mutex);
		free(*table);
		*table = temp;
	}
	(*table) = NULL;
	return (NULL);
}

t_null	*delete_table(t_table **table, t_table **cur)
{
	if (table && *table)
	{
		(*table)->right->left = NULL;
		(*table)->right = NULL;
		delete_list_reverse(table);
	}
	if (cur && *cur)
		free(*cur);
	return (NULL);
}

t_table *init_table(t_data *data)
{
	t_table		*cur;
	t_table		*prev;
	t_table		*last;
	size_t		i;

	prev = NULL;
	i = 0;
	while (i < (data->number_of_philosophers * 2))
	{
		if (!(cur = (t_table*)malloc(sizeof(t_table))))
			return (delete_table(&prev, NULL));
		if (set_position(i++, &cur, data) < 0)
			return (delete_table(&prev, &cur));
		cur->right = NULL;
		cur->left = prev;
		if (prev)
			prev->right = cur;
		prev = cur;
	}
	last = cur;
	while (cur->left != NULL)
		cur = cur->left;
	cur->left = last;
	last->right = cur;
	return (cur);
}
