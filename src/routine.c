/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 18:41:07 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/06 20:51:44 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	philo->state = FORK_TAKEN;
	philo_print(philo, FORK_TAKEN);
	pthread_mutex_lock(philo->right_fork);
	philo_print(philo, FORK_TAKEN);
	philo->last_meal = get_time();
}

static void	philo_eat(t_philo *philo)
{
	philo->state = EATING;
	philo_print(philo, EATING);
	philo->env->meals_eaten++;
	usleep(philo->env->time_to_eat * 1000);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

static void	philo_sleep(t_philo *philo)
{
	philo->state = SLEEPING;
	philo_print(philo, SLEEPING);
	usleep(philo->env->time_to_sleep * 1000);
}

static void	philo_think(t_philo *philo)
{
	philo->state = THINKING;
	philo_print(philo, THINKING);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
	{	
		philo_print(philo, THINKING);
		usleep(1000);
	}
	while (philo->env->nb_meals == 0 || philo->env->meals_eaten < philo->env->nb_meals)
	{
		philo_take_forks(philo);
		philo_eat(philo);
		philo_sleep(philo);
		philo_think(philo);
	}
	return (NULL);
}