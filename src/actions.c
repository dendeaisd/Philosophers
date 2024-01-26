/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 20:17:04 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/26 20:20:23 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	eating_routine(t_env *env, t_philo *philo, \
			pthread_mutex_t *first_fork, pthread_mutex_t *second_fork)
{
	pthread_mutex_lock(first_fork);
	philo_print(env, philo, FORK_TAKEN);
	if (is_dead(philo))
		return ((void)pthread_mutex_unlock(first_fork));
	pthread_mutex_lock(second_fork);
	philo_print(env, philo, FORK_TAKEN);
	pthread_mutex_lock(&env->protect_meals);
	philo->last_meal = get_time();
	philo->env->meals_eaten++;
	pthread_mutex_unlock(&env->protect_meals);
	if (is_dead(philo))
		return (pthread_mutex_unlock(first_fork), \
			(void)pthread_mutex_unlock(second_fork));
	pthread_mutex_lock(&env->status_mutex);
	philo->state = EATING;
	pthread_mutex_unlock(&env->status_mutex);
	philo_print(env, philo, EATING);
	msleep(philo->env->time_to_eat);
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
}

void	philo_eat(t_env *env, t_philo *philo)
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
		msleep(5);
	eating_routine(env, philo, first_fork, second_fork);
}

void	philo_sleep(t_env *env, t_philo *philo)
{
	if (is_dead(philo))
		return ;
	pthread_mutex_lock(&env->status_mutex);
	philo->state = SLEEPING;
	pthread_mutex_unlock(&env->status_mutex);
	philo_print(env, philo, SLEEPING);
	msleep(philo->env->time_to_sleep);
}

void	philo_think(t_env *env, t_philo *philo)
{
	if (is_dead(philo))
		return ;
	pthread_mutex_lock(&env->status_mutex);
	philo->state = THINKING;
	pthread_mutex_unlock(&env->status_mutex);
	philo_print(env, philo, THINKING);
	if (is_dead(philo))
		return ;
}
