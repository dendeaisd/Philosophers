/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 18:41:07 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/26 20:20:35 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	main_routine_loop(t_philo *philo, int running, \
			int meals_eaten, int nb_meals)
{
	while (running && (nb_meals == -1 || meals_eaten < nb_meals))
	{
		philo_eat(philo->env, philo);
		philo_sleep(philo->env, philo);
		philo_think(philo->env, philo);
		pthread_mutex_lock(&philo->env->protect_meals);
		meals_eaten = philo->env->meals_eaten;
		pthread_mutex_unlock(&philo->env->protect_meals);
		pthread_mutex_lock(&philo->env->status_mutex);
		running = philo->env->status;
		pthread_mutex_unlock(&philo->env->status_mutex);
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	int		running;
	int		meals_eaten;
	int		nb_meals;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->env->status_mutex);
	running = philo->env->status;
	pthread_mutex_unlock(&philo->env->status_mutex);
	pthread_mutex_lock(&philo->env->protect_meals);
	meals_eaten = philo->env->meals_eaten;
	nb_meals = philo->env->nb_meals;
	pthread_mutex_unlock(&philo->env->protect_meals);
	if (philo->env->nb_philo == 1)
		return (philo_print(philo->env, philo, FORK_TAKEN), \
				NULL);
	main_routine_loop(philo, running, meals_eaten, nb_meals);
	return (NULL);
}
