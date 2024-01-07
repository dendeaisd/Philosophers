/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 22:55:49 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/07 17:26:36 by fvoicu           ###   ########.fr       */
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

long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

void	msleep(long sleep)
{
	long	start;

	start = get_time();
	while ((get_time() - start) < sleep)
		usleep(1000);
}

void	philo_print(t_env *env, t_philo *philo, t_pstate state)
{
	long	time;

	time = get_time() - env->start_time;
	if (state == FORK_TAKEN)
		printf("%zu %d has taken a fork\n", time, philo->id);
	else if (state == EATING)
		printf("%zu %d is eating\n", time, philo->id);
	else if (state == SLEEPING)
		printf("%zu %d is sleeping\n", time, philo->id);
	else if (state == THINKING)
		printf("%zu %d is thinking\n", time, philo->id);
	else if (state == DIED)
		printf("%zu %d is dead\n", time, philo->id);
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
