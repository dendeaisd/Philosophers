/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 22:55:49 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/20 01:37:45 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

#define LAVANDER	"\033[38;2;177;156;217m"
#define BLUE		"\033[38;5;68m"
#define L_BLUE		"\033[38;5;159m"
#define GREEN		"\033[38;5;156m"
#define RED			"\033[0;31m"
#define WHITE		"\033[0;97m"

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

	id = philo->id;
	time = get_time() - env->start_time;
	// pthread_mutex_lock(&env->print_mutex);
	if (!locked)
		pthread_mutex_lock(&env->status_mutex);
	if (state == FORK_TAKEN && env->status)
		printf("%zu %d %shas taken a fork%s\n", time, id, BLUE, WHITE);
	else if (state == EATING && env->status)
		printf("%zu %d %sis eating%s\n", time, id, L_BLUE, WHITE);
	else if (state == SLEEPING && env->status)
		printf("%zu %d %sis sleeping%s\n", time, id, LAVANDER, WHITE);
	else if (state == THINKING && env->status)
		printf("%zu %d %sis thinking%s\n", time, id, GREEN, WHITE);
	else if (state == DIED)
		printf("%zu %d %sdied%s\n", time, id, RED, WHITE);
	if (!locked)
		pthread_mutex_unlock(&env->status_mutex);
	// pthread_mutex_unlock(&env->print_mutex);
}
