/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fvoicu <fvoicu@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 18:41:07 by fvoicu            #+#    #+#             */
/*   Updated: 2024/01/19 23:17:14 by fvoicu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
//TODO: divide philo_routinne, maybe restructure a bit

static int is_dead(t_philo *philo)
{	
	pthread_mutex_lock(&philo->env->status_mutex);
	if (!philo->env->status)
	{
		pthread_mutex_unlock(&philo->env->status_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->env->status_mutex);
	return (0);
}

// //
// static int check_forks(pthread_mutex_t *first_fork, pthread_mutex_t *second_fork) {
//     if (pthread_mutex_trylock(first_fork) == 0) {
//         if (pthread_mutex_trylock(second_fork) == 0) {
//             return 1; // Successfully locked both forks
//         }
//         pthread_mutex_unlock(first_fork); // Unlock the first fork if the second one is not available
// 		printf("second fork not available\n");
// 		printf("first fork unlocked\n" );
//     }
//     return 0; // Could not lock both forks
// }

// static void philo_take_forks(t_env *env, t_philo *philo) {
//     pthread_mutex_t *first_fork;
//     pthread_mutex_t *second_fork;

//     if (is_dead(philo))
//         return;

//     first_fork = philo->id % 2 == 0 ? philo->left_fork : philo->right_fork;
//     second_fork = philo->id % 2 == 0 ? philo->right_fork : philo->left_fork;

//     while (!check_forks(first_fork, second_fork)) {
//         // If unable to pick up both forks, release any held fork and try again after a short delay
//         msleep(3); // Small delay to prevent busy waiting
//     }

//     // Successfully picked up both forks
//     pthread_mutex_lock(&env->status_mutex);
//     philo->state = FORK_TAKEN;
//     pthread_mutex_unlock(&env->status_mutex);
//     philo_print(env, philo, FORK_TAKEN, 0);

//     pthread_mutex_lock(&env->protect_meals);
//     philo->last_meal = get_time();
//     pthread_mutex_unlock(&env->protect_meals);

//     philo->state = EATING;
//     philo_print(env, philo, EATING, 0);

//     msleep(philo->env->time_to_eat);

//     pthread_mutex_unlock(first_fork);
//     pthread_mutex_unlock(second_fork);
// }

static void	philo_take_forks(t_env *env, t_philo *philo)
{
	pthread_mutex_t *first_fork;
	pthread_mutex_t *second_fork;
	
	if (is_dead(philo))
		return ;
	if (philo->id % 2 != 0)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	if (philo->id == philo->env->nb_philo)
		msleep(3);
	pthread_mutex_lock(first_fork);

	pthread_mutex_lock(&env->status_mutex);
	philo->state = FORK_TAKEN;
	pthread_mutex_unlock(&env->status_mutex);
	philo_print(env, philo, FORK_TAKEN, 0);
	pthread_mutex_lock(second_fork);
	philo_print(env, philo, FORK_TAKEN, 0);
	pthread_mutex_lock(&env->protect_meals);
	philo->last_meal = get_time();
	pthread_mutex_unlock(&env->protect_meals);
	
	philo->state = EATING;
	philo_print(env, philo, EATING, 0);
	pthread_mutex_lock(&env->protect_meals);
	philo->env->meals_eaten++;
	pthread_mutex_unlock(&env->protect_meals);
	
	msleep(philo->env->time_to_eat);
	
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
}


// static void	philo_eat(t_env *env, t_philo *philo)
// {
// 	philo->state = EATING;
// 	philo_print(env, philo, EATING, 0);
// 	pthread_mutex_lock(&env->protect_meals);
// 	philo->env->meals_eaten++;
// 	pthread_mutex_unlock(&env->protect_meals);
	
// 	msleep(philo->env->time_to_eat);
// 	if (philo->left_fork)
// 		pthread_mutex_unlock(philo->left_fork);
// 	pthread_mutex_unlock(philo->right_fork);
// }

static void	philo_sleep(t_env *env, t_philo *philo)
{
	if (is_dead(philo))
		return ;
	philo->state = SLEEPING;
	philo_print(env, philo, SLEEPING, 0);
	msleep(philo->env->time_to_sleep);
}

static void	philo_think(t_env *env, t_philo *philo)
{
	if (is_dead(philo))
		return ;
	philo->state = THINKING;
	philo_print(env, philo, THINKING, 0);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	int		running;

	philo = (t_philo *)arg;
	pthread_mutex_lock(&philo->env->status_mutex);
	running = philo->env->status;
	pthread_mutex_unlock(&philo->env->status_mutex);
	if (philo->env->nb_philo == 1)
		return (philo_print(philo->env, philo, FORK_TAKEN, 0), \
				NULL);
	if (!running)
		return (NULL);
	while (philo->env->nb_meals == -1 \
		|| philo->env->meals_eaten < philo->env->nb_meals)
	{
		pthread_mutex_lock(&philo->env->status_mutex);
		running = philo->env->status;
		pthread_mutex_unlock(&philo->env->status_mutex);
		philo_take_forks(philo->env, philo);
		philo_sleep(philo->env, philo);
		philo_think(philo->env, philo);
		if (!running)
			break ;
	}
	return (NULL);
}
