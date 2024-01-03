/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 22:06:36 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/03 23:03:09 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	parse_args(int ac, char **av)
{
	int	i;
	int	j;

	i = 0;
	if (!(*av) || *av[0] == '0')
		return (write(2, "Invalid argument\n", 18), false);
	while (++i < ac)
	{
		j = -1;
		while (av[i][++j])
			if (!ft_isdigit(av[i][j]))
				return (write(2, "Invalid argument\n", 18), false);
	}
	return (true);
}

int	main(int ac, char **av)
{
	if (ac != 5 || ac != 6)
	{
		write(2, "Wrong number of arguments\n", 27);
		return (1);
	}
	if (parse_args(ac, av) == true)
	{
		//init
		//create threads
		//join threads
		//destroy mutexes
	}
	return (0);
}
