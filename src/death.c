/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/07 00:37:01 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/21 19:19:30 by fvoicu           ###   ########.fr       */
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

// static int	death_check(t_philo *philos)
// {
// 	int		i;
// 	long	till_next_meal;

// 	till_next_meal = 0;
// 	while (1)
// 	{
// 		i = -1;
// 		while (++i < philos->env->nb_philo)
// 		{
// 			pthread_mutex_lock(&philos->env->protect_meals);
// 			till_next_meal = (get_time() - philos[i].last_meal);
// 			pthread_mutex_unlock(&philos->env->protect_meals);
// 			if (till_next_meal > philos->env->time_to_die)
// 			{
// 				pthread_mutex_lock(&philos->env->status_mutex);
// 				if (philos->env->meals_eaten >= philos->env->nb_meals \
// 						&& philos->env->nb_meals != -1)
// 				{
// 					philo_print(philos->env, &philos[i], FULL);
// 					philos->env->status = 0;
// 				}
// 				else	
// 				{
// 					philos[i].state = DIED;
// 					philo_print(philos->env, &philos[i], DIED);
// 					philos->env->status = 0;
// 				}
// 				pthread_mutex_unlock(&philos->env->status_mutex);
// 				return (1);
// 			}
// 		}
// 		msleep(1);
// 	}
// 	return (0);
// }

static int check_philosopher_death(t_philo *philos, int i)
{
	long till_next_meal;
	
	pthread_mutex_lock(&philos->env->protect_meals);
	till_next_meal = get_time() - philos[i].last_meal;
	pthread_mutex_unlock(&philos->env->protect_meals);

	if (till_next_meal > philos->env->time_to_die)
		return 1;
	return 0;
}

static void update_environment_status(t_philo *philos, int i)
{
	pthread_mutex_lock(&philos->env->status_mutex);
	if (philos->env->meals_eaten >= philos->env->nb_meals && philos->env->nb_meals != -1)
		philo_print(philos->env, &philos[i], FULL);
	else 
	{
		philos[i].state = DIED;
		philo_print(philos->env, &philos[i], DIED);
	}
	philos->env->status = 0;
	pthread_mutex_unlock(&philos->env->status_mutex);
}

static int death_check(t_philo *philos)
{
	int i;

	while (1) {
		i = -1;
		while (++i < philos->env->nb_philo) {
			if (check_philosopher_death(philos, i)) {
				update_environment_status(philos, i);
				return 1;
			}
		}
		msleep(1);
	}
	return 0;
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
