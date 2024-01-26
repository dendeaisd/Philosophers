/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 22:06:36 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/26 20:50:32 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	parse_args(int ac, char **av)
{
	int	i;
	int	j;

	i = 0;
	if (!(*av) || *av[0] == '0')
		return (error(ARG_ERROR), false);
	while (++i < ac)
	{
		j = -1;
		if (!av[i][0])
			return (error(ARG_ERROR), false);
		while (av[i][++j])
			if (!(av[i][j] >= '0' && av[i][j] <= '9'))
				return (error(ARG_ERROR), false);
	}
	if (ft_atoi(av[1]) > 200)
		return (error(PHIL_NB), false);
	return (true);
}

static bool	check_invalid_input(t_env *env)
{
	if (env->nb_philo <= 0 || env->time_to_die <= 0 || env->time_to_eat <= 0 \
		|| env->time_to_sleep <= 0)
		return (error(ARG_ERROR), false);
	return (true);
}

int	main(int ac, char **av)
{
	t_env	*env;
	t_philo	*philos;
	int		i;

	i = -1;
	if (ac != 5 && ac != 6)
		return (error(ARG_NB), 1);
	if (parse_args(ac, av))
	{
		env = init_env(ac, av);
		if (!env || !check_invalid_input(env))
			return (free(env), 1);
		philos = init_philos(env);
		if (pthread_create(&env->supervisor, NULL, &supervisor, philos))
			error(THREAD_ERROR);
		while (++i < env->nb_philo)
			if (pthread_create \
				(&philos[i].thread_id, NULL, &philo_routine, &philos[i]))
				error(THREAD_ERROR);
		pthread_join(env->supervisor, NULL);
		free(env->forks);
		free(philos);
		free(env);
	}
	return (0);
}
