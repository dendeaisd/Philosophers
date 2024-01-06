/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 22:55:49 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/06 20:32:14 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
//TODO: add ft_atoi

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

size_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}


void	philo_print(t_philo *philo, t_pstate state)
{
	if(state == FORK_TAKEN)
		printf("%zu %d has taken a fork\n", get_time(), philo->id);
	else if (state == EATING)
		printf("%zu %d is eating\n", get_time(), philo->id);
	else if (state == SLEEPING)
		printf("%zu %d is sleeping\n", get_time(), philo->id);
	else if (state == THINKING)
		printf("%zu %d is thinking\n", get_time(), philo->id);
	else if (state == DIED)
		printf("%zu %d died\n", get_time(), philo->id);
}

void*	error(t_error error)
{
	if(error == ARG_ERROR)
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
