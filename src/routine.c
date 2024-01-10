/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 18:41:07 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/10 17:49:00 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	philo_take_forks(t_env *env, t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	philo->state = FORK_TAKEN;
	philo_print(env, philo, FORK_TAKEN);
	if (philo->left_fork != NULL)
	{
		pthread_mutex_lock(philo->left_fork);
		philo_print(env, philo, FORK_TAKEN);
		philo->last_meal = get_time();
	}
}

static void	philo_eat(t_env *env, t_philo *philo)
{
	philo->state = EATING;
	philo_print(env, philo, EATING);
	pthread_mutex_lock(&env->protect_meals);
	philo->env->meals_eaten++;
	pthread_mutex_unlock(&env->protect_meals);
	msleep(philo->env->time_to_eat);
	pthread_mutex_unlock(philo->right_fork);
	if (philo->left_fork != NULL)
		pthread_mutex_unlock(philo->left_fork);
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
	int		running;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->env->status_mutex);
	running = philo->env->status;
	pthread_mutex_unlock(&philo->env->status_mutex);
	if (philo->env->nb_philo == 1)
		return (philo_print(philo->env, philo, THINKING), NULL);
	if (philo->id % 2 == 0)
	{
		philo_print(philo->env, philo, THINKING);
		msleep(3);
	}
	if (!running)
		return (NULL);
	while (philo->env->nb_meals == -1 \
		|| philo->env->meals_eaten < philo->env->nb_meals)
	{
		philo_take_forks(philo->env, philo);
		philo_eat(philo->env, philo);
		philo_sleep(philo->env, philo);
		philo_think(philo->env, philo);
		if (!running)
			break ;
	}
	return (NULL);
}
