/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 17:03:04 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/07 17:28:58 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_env	*init_env(int ac, char **av)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	if (!env)
		error(MALLOC_ERROR);
	env->nb_philo = atoi(av[1]);
	env->time_to_die = atoi(av[2]);
	env->time_to_eat = atoi(av[3]);
	env->time_to_sleep = atoi(av[4]);
	env->start_time = get_time();
	env->nb_meals = -1;
	if (ac == 6)
		env->nb_meals = atoi(av[5]) * env->nb_philo;
	env->meals_eaten = 0;
	if (pthread_mutex_init(&env->protect_meals, NULL))
		error(MUTEX_ERROR);
	env->forks = malloc(sizeof(pthread_mutex_t) * env->nb_philo);
	if (!(env->forks))
		error(MALLOC_ERROR);
	if (env->time_to_die < 1 || env->time_to_eat < 1 || env->time_to_sleep < 1)
		error(ARG_ERROR);
	if (pthread_mutex_init(&env->status, NULL))
		error(MUTEX_ERROR);
	return (env);
}

t_philo	*init_philos(t_env *env)
{
	int		i;
	t_philo	*philos;

	i = -1;
	philos = malloc(sizeof(t_philo) * (env->nb_philo));
	if (!philos)
		error(MALLOC_ERROR);
	while (++i < env->nb_philo)
	{
		philos[i].id = i + 1;
		philos[i].last_meal = get_time();
		philos[i].env = env;
		philos[i].left_fork = &env->forks[i];
		philos[i].right_fork = &env->forks[(i + 1) % env->nb_philo];
		philos[i].thread_id = 0;
		if (pthread_mutex_init(&env->forks[i], NULL))
			error(MUTEX_ERROR);
	}
	return (philos);
}
