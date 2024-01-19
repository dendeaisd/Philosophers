/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 18:41:07 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/20 00:54:46 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	eating_routine(t_env *env, t_philo *philo, \
			pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	if (is_dead(philo))
		return ;
	pthread_mutex_lock(first_fork);
	pthread_mutex_lock(&env->status_mutex);
	philo->state = FORK_TAKEN;
	pthread_mutex_unlock(&env->status_mutex);
	philo_print(env, philo, FORK_TAKEN, env->status);
		if (is_dead(philo))
		return ;
	pthread_mutex_lock(second_fork);
	philo_print(env, philo, FORK_TAKEN, env->status);
	pthread_mutex_lock(&env->protect_meals);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&env->protect_meals);
	if (is_dead(philo))
		return ;
	philo->state = EATING;
	philo_print(env, philo, EATING, env->status);
	pthread_mutex_lock(&env->protect_meals);
	philo->env->meals_eaten++;
	pthread_mutex_unlock(&env->protect_meals);
	msleep(philo->env->time_to_eat);
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
}

static void	philo_eat(t_env *env, t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (is_dead(philo))
		return ;
	if (philo->id % 2 != 0)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	if (philo->id == philo->env->nb_philo)
		msleep(3);
	eating_routine(env, philo, first_fork, second_fork);
	if (is_dead(philo))
		return ;
}

static void	philo_sleep(t_env *env, t_philo *philo)
{
	if (is_dead(philo))
		return ;
	philo->state = SLEEPING;
	philo_print(env, philo, SLEEPING, env->status);
	msleep(philo->env->time_to_sleep);
}

static void	philo_think(t_env *env, t_philo *philo)
{
	if (is_dead(philo))
		return ;
	philo->state = THINKING;
	philo_print(env, philo, THINKING, env->status);
	if (is_dead(philo))
		return ;
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
		return (philo_print(philo->env, philo, FORK_TAKEN, philo->env->status), \
				NULL);
	if (!running)
		return (NULL);
	while (philo->env->nb_meals == -1 \
		|| philo->env->meals_eaten < philo->env->nb_meals)
	{
		pthread_mutex_lock(&philo->env->status_mutex);
		running = philo->env->status;
		pthread_mutex_unlock(&philo->env->status_mutex);
		philo_eat(philo->env, philo);
		philo_sleep(philo->env, philo);
		philo_think(philo->env, philo);
		if (!running)
			break ;
	}
	return (NULL);
}
