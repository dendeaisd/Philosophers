/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/03 22:08:07 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/10 16:04:49 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <string.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>

typedef enum e_pstate
{
	THINKING,
	EATING,
	SLEEPING,
	FORK_TAKEN,
	DIED
}	t_pstate;

typedef struct s_env
{
	int					nb_philo;
	long				time_to_die;
	size_t				time_to_eat;
	size_t				time_to_sleep;
	long				start_time;
	int					nb_meals;
	int					meals_eaten;
	pthread_mutex_t		protect_meals;
	pthread_mutex_t		*forks;
	pthread_mutex_t		status_mutex;
	pthread_t			supervisor;
	int					status;			
}	t_env;

typedef struct s_philo
{
	int					id;
	long				last_meal;
	t_pstate			state;
	t_env				*env;
	pthread_t			thread_id;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
}	t_philo;

typedef enum e_error
{
	ARG_ERROR,
	ARG_NB,
	MALLOC_ERROR,
	THREAD_ERROR,
	MUTEX_ERROR,
	PHIL_NB	
}	t_error;

/*INIT*/
t_env	*init_env(int ac, char **av);
t_philo	*init_philos(t_env *env);

/*ROUTINE*/
void	*philo_routine(void *arg);

/*SUPERVISOR*/
void	*supervisor(void *arg);

/**UTILS**/
int		ft_atoi(const char *str);
int		ft_isdigit(int c);
long	get_time(void);
void	philo_print(t_env *env, t_philo *philo, t_pstate state);
void	*error(t_error error);
void	msleep(long sleep);

#endif