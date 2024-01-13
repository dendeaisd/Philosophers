/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 18:41:07 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/14 00:14:11 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_dead(t_philo *philo)
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

static void	philo_take_forks(t_env *env, t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (is_dead(philo))
		return ;
	if (philo->right_fork < philo->left_fork)
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	else
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	pthread_mutex_lock(first_fork);
	pthread_mutex_lock(&env->status_mutex);
	philo->state = FORK_TAKEN;
	pthread_mutex_unlock(&env->status_mutex);
	philo_print(env, philo, FORK_TAKEN, 0);
	pthread_mutex_lock(second_fork);
	philo_print(env, philo, FORK_TAKEN, 0);
	pthread_mutex_lock(&philo->env->protect_meals);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&philo->env->protect_meals);
	philo->state = EATING;
	philo_print(env, philo, EATING, 0);
	pthread_mutex_lock(&env->protect_meals);
	philo->env->meals_eaten++;
	pthread_mutex_unlock(&env->protect_meals);
	msleep(philo->env->time_to_eat);
	pthread_mutex_unlock(second_fork);
	pthread_mutex_unlock(first_fork);
}

static void	philo_sleep(t_env *env, t_philo *philo)
{
	if (is_dead(philo))
		return ;
	philo->state = SLEEPING;
	philo_print(env, philo, SLEEPING, 0);
	msleep(philo->env->time_to_sleep);
}

static void	philo_think(t_env *env, t_philo *philo)
{
	if (is_dead(philo))
		return ;
	philo->state = THINKING;
	philo_print(env, philo, THINKING, 0);
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
		return (philo_print(philo->env, philo, FORK_TAKEN, 0), \
			philo_print(philo->env, philo, THINKING, 0), NULL);
	if (philo->id % 2 == 0)
	{
		philo_print(philo->env, philo, THINKING, 0);
		msleep(3);
	}
	if (!running)
		return (NULL);
	while ((philo->env->nb_meals == -1 \
		|| philo->env->meals_eaten < philo->env->nb_meals) \
		&& philo->env->nb_philo > 1)
	{
		pthread_mutex_lock(&philo->env->status_mutex);
		running = philo->env->status;
		pthread_mutex_unlock(&philo->env->status_mutex);
		philo_take_forks(philo->env, philo);
		// philo_eat(philo->env, philo);
		philo_sleep(philo->env, philo);
		philo_think(philo->env, philo);
		if (!running)
			break ;
	}
	return (NULL);
}
