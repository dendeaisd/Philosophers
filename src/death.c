/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 00:37:01 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/20 00:56:28 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->env->status_mutex);
	if (!philo->env->status)
	{
		pthread_mutex_unlock(&philo->env->status_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->env->status_mutex);
	return (0);
}

static int	death_check(t_philo *philos)
{
	int		i;
	long	till_next_meal;

	till_next_meal = 0;
	while (1)
	{
		i = -1;
		while (++i < philos->env->nb_philo)
		{
			pthread_mutex_lock(&philos->env->protect_meals);
			till_next_meal = (get_time() - philos[i].last_meal);
			pthread_mutex_unlock(&philos->env->protect_meals);
			if (till_next_meal > philos->env->time_to_die)
			{
				pthread_mutex_lock(&philos->env->status_mutex);
				philos[i].state = DIED;
				philo_print(philos->env, &philos[i], DIED, 1);
				philos->env->status = 0;
				pthread_mutex_unlock(&philos->env->status_mutex);
				i = -1;
				while (++i < philos->env->nb_philo)
				{
					pthread_mutex_lock(&philos->env->status_mutex);
					philos->env->status = 0;
					pthread_mutex_unlock(&philos->env->status_mutex);
				}
				return (1);
			}
		}
		msleep(1);
	}
	return (0);
}
// static int death_check(t_philo *philos) {
//     int i;
//     long till_next_meal;
//     int someone_died = 0;

//     while (1) {
//         i = -1;
//         while (++i < philos->env->nb_philo) {
//             pthread_mutex_lock(&philos->env->protect_meals);
//             till_next_meal = (get_time() - philos[i].last_meal);
//             pthread_mutex_unlock(&philos->env->protect_meals);

//             if (till_next_meal > philos->env->time_to_die) {
//                 pthread_mutex_lock(&philos->env->status_mutex);
//                 if (!someone_died) {  // Print and update status only for the first death
//                     philos[i].state = DIED;
//                     philo_print(philos->env, &philos[i], DIED, 1);
//                     philos->env->status = 0;
//                     someone_died = 1;
//                 }
//                 pthread_mutex_unlock(&philos->env->status_mutex);
//             }
//         }

//         if (someone_died) {
//             // Break out of the while loop if someone has died
//             break;
//         }

//         msleep(1);
//     }

//     return someone_died ? 1 : 0;
// }

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
