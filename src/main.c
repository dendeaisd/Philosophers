/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 22:06:36 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/07 19:42:49 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
//TODO: add ft_atoi

static bool	parse_args(int ac, char **av)
{
	int	i;
	int	j;

	i = 0;
	if (!(*av) || *av[0] == '0')
		error(ARG_ERROR);
	while (++i < ac)
	{
		j = -1;
		while (av[i][++j])
			if (!ft_isdigit(av[i][j]))
				error(ARG_ERROR);
	}
	if (atoi(av[1]) > 200)
		error(PHIL_NB);
	return (true);
}

int	main(int ac, char **av)
{
	t_env	*env;
	t_philo	*philos;
	int		i;

	i = -1;
	if (ac != 5 && ac != 6)
		error(ARG_NB);
	env = init_env(ac, av);
	philos = init_philos(env);
	if (parse_args(ac, av))
	{
		while (++i < env->nb_philo)
		{
			if (pthread_create \
				(&philos[i].thread_id, NULL, &philo_routine, &philos[i]))
				error(THREAD_ERROR);
		}
		if (pthread_create(&env->supervisor, NULL, &supervisor, &philos))
			error(THREAD_ERROR);
		pthread_join(philos->thread_id, NULL);
		free(philos);
		free(env);
	}
	return (0);
}
