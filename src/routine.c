/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 18:41:07 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/07 17:27:08 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_take_forks(t_env *env, t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	philo->state = FORK_TAKEN;
	philo_print(env, philo, FORK_TAKEN);
	pthread_mutex_lock(philo->right_fork);
	philo_print(env, philo, FORK_TAKEN);
	philo->last_meal = get_time();
}

static void	philo_eat(t_env *env, t_philo *philo)
{
	philo->state = EATING;
	philo_print(env, philo, EATING);
	pthread_mutex_lock(&env->protect_meals);
	philo->env->meals_eaten++;
	pthread_mutex_unlock(&env->protect_meals);
	msleep(philo->env->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

static void	philo_sleep(t_env *env, t_philo *philo)
{
	philo->state = SLEEPING;
	philo_print(env, philo, SLEEPING);
	msleep(philo->env->time_to_sleep);
}

static void	philo_think(t_env *env, t_philo *philo)
{
	philo->state = THINKING;
	philo_print(env, philo, THINKING);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
	{
		philo_print(philo->env, philo, THINKING);
		msleep(1);
	}
	while (philo->env->nb_meals == -1 \
		|| philo->env->meals_eaten < philo->env->nb_meals)
	{
		philo_take_forks(philo->env, philo);
		philo_eat(philo->env, philo);
		philo_sleep(philo->env, philo);
		philo_think(philo->env, philo);
	}
	return (NULL);
}
