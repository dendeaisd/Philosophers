/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 00:37:01 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/07 17:32:00 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*death_check(t_philo *philos)
{
	int		i;

	while (1)
	{
		pthread_mutex_lock(&philos->env->status);
		i = -1;
		while (++i < philos->env->nb_philo)
		{
			if (get_time() - philos[i].last_meal > philos->env->time_to_die)
				return (philos[i].state = DIED, \
					philo_print(philos->env, &philos[i], DIED), \
					pthread_mutex_unlock(&philos->env->status), (void *)1);
		}
		pthread_mutex_unlock(&philos->env->status);
		msleep(1);
	}
}

void	*supervisor(void *arg)
{
	t_philo	*philos;
	void	*result;
	int		i;

	philos = (t_philo *)arg;
	i = philos->env->nb_philo;
	result = death_check(philos);
	if (result)
		while (i--)
			pthread_join(philos[i].thread_id, NULL);
	return (NULL);
}
