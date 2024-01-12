/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 00:37:01 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/12 16:19:10 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	death_check(t_philo *philos)
{
	int		i;
	long	till_next_meal;

	till_next_meal = 0;
	while (1)
	{
		pthread_mutex_lock(&philos->env->status_mutex);
		i = -1;
		while (++i < philos->env->nb_philo)
		{
			till_next_meal = (get_time() - philos[i].last_meal);
			if (till_next_meal > philos->env->time_to_die)
			{
				philos[i].state = DIED;
				philo_print(philos->env, &philos[i], DIED, 1);
				philos->env->status = 0;
				pthread_mutex_unlock(&philos->env->status_mutex);
				return (1);
			}
		}
		pthread_mutex_unlock(&philos->env->status_mutex);
		msleep(1);
	}
	return (0);
}

void	*supervisor(void *arg)
{
	t_philo	*philos;
	int		result;	
	int		i;

	philos = (t_philo *)arg;
	result = death_check(philos);
	i = -1;
	if (result)
		while (++i < philos->env->nb_philo)
			pthread_join(philos[i].thread_id, NULL);
	pthread_mutex_destroy(&philos->env->status_mutex);
	pthread_mutex_destroy(&philos->env->protect_meals);
	return (NULL);
}
