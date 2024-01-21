/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 17:03:04 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/21 22:30:48 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_mutexes(t_env *env)
{
	if (pthread_mutex_init(&env->protect_meals, NULL))
		return ((void)error(MUTEX_ERROR));
	if (pthread_mutex_init(&env->status_mutex, NULL))
		return ((void)error(MUTEX_ERROR));
	if (pthread_mutex_init(&env->print_mutex, NULL))
		return ((void)error(MUTEX_ERROR));
}

t_env	*init_env(int ac, char **av)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	if (!env)
		return (error(MALLOC_ERROR), NULL);
	env->nb_philo = ft_atoi(av[1]);
	env->time_to_die = ft_atoi(av[2]);
	env->time_to_eat = ft_atoi(av[3]);
	env->time_to_sleep = ft_atoi(av[4]);
	env->start_time = get_time();
	env->nb_meals = -1;
	if (ac == 6)
		env->nb_meals = ft_atoi(av[5]) * env->nb_philo;
	env->meals_eaten = 0;
	env->forks = malloc(sizeof(pthread_mutex_t) * env->nb_philo);
	if (!(env->forks))
		return (error(MALLOC_ERROR), NULL);
	if (env->time_to_die < 1 || env->time_to_eat < 1 || env->time_to_sleep < 1)
		return (error(ARG_ERROR), NULL);
	env->status = 1;
	init_mutexes(env);
	return (env);
}

t_philo	*init_philos(t_env *env)
{
	int		i;
	t_philo	*philos;

	i = -1;
	philos = malloc(sizeof(t_philo) * (env->nb_philo));
	if (!philos)
		return (error(MALLOC_ERROR), NULL);
	while (++i < env->nb_philo)
	{
		philos[i].id = i + 1;
		philos[i].last_meal = get_time();
		philos[i].env = env;
		philos[i].right_fork = &env->forks[i];
		philos[i].left_fork = &env->forks[(i + 1) % env->nb_philo];
		philos[i].thread_id = 0;
		if (pthread_mutex_init(&env->forks[i], NULL))
			return (error(MUTEX_ERROR), NULL);
	}
	return (philos);
}

void	*error(t_error error)
{
	if (error == ARG_ERROR)
		write(2, "Invalid argument\n", 18);
	else if (error == ARG_NB)
		write(2, "Wrong number of arguments\n", 27);
	else if (error == MALLOC_ERROR)
		write(2, "Error allocating memory\n", 24);
	else if (error == THREAD_ERROR)
		write(2, "Error creating thread\n", 22);
	else if (error == MUTEX_ERROR)
		write(2, "Error initializing mutex\n", 26);
	else if (error == PHIL_NB)
		write(2, "Too many philosophers\n", 22);
	return (NULL);
}
