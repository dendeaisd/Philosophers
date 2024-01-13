/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 22:55:49 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/13 19:36:17 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int					i;
	int					sign;
	unsigned long int	final_nbr;

	i = 0;
	sign = 1;
	final_nbr = 0;
	while (str[i] == ' ' || str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r' || str[i] == '\t')
		++i;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i++] == '-')
			sign = -1;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		final_nbr = final_nbr * 10 + str[i] - '0';
		i++;
	}
	if (final_nbr > 2147483647 && sign == 1)
		return (-1);
	if (final_nbr > 2147483648 && sign == -1)
		return (0);
	return (final_nbr * sign);
}

time_t	get_time(void)
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

void	philo_print(t_env *env, t_philo *philo, t_pstate state, int locked)
{
	long	time;
	int		id;
	int		env_status;
	
	if (!locked)
		pthread_mutex_lock(&philo->env->status_mutex);
	env_status = env->status;
	if(!locked)
		pthread_mutex_unlock(&philo->env->status_mutex);
	if (env_status == 0)
		return ;
	id = philo->id;
	time = get_time() - env->start_time;
	pthread_mutex_lock(&env->logging_mutex);
	if (state == FORK_TAKEN)
		printf("%zu %d \033[38;5;68mhas taken a fork\033[0;97m\n", time, id);
	else if (state == EATING)
		printf("%zu %d \033[38;5;87mis eating\033[0;97m\n", time, id);
	else if (state == SLEEPING)
		printf \
		("%zu %d \033[38;2;177;156;217mis sleeping\033[0;97m\n", time, id);
	else if (state == THINKING)
		printf("%zu %d \033[38;5;156mis thinking\033[0;97m\n", time, id);
	else if (state == DIED)
		printf("%zu %d \033[0;31mdied\033[0;97m\n", time, id);
	pthread_mutex_unlock(&env->logging_mutex);
}
