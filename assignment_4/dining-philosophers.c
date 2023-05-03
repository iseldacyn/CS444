/* Operating Systems - CS444
 * Spring 2023
 * Iselda Aiello
 * Assignment info:
 * Solve the Dining Philosophers Problem without deadlocks or resource starvation
 * Program Desc:
 * Uses pthreads and pthread_mutex_t to simulate philosophers and their forks.
 * After a cycle (~3 seconds), a philosopher changes (or attempts to) change a state.
 * States: thinking-no movement; hungry-tries to pick up forks and eat; eating-eats then puts down forks; dead-close program
 * Each cycle, a philosophers hunger decreases (except for when they are eating).
 * When a philosopher dies, the program exits.
 * Last Modified: 5/2/23
 * Code Review Link https://drive.google.com/file/d/1mVXAD5G-L_AeUrenfTlWbbsGPMJJiLF1/view?usp=sharing 
 */

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/* constants */
#define NUM 5
#define HUNGER_MAX 10
#define CYCLE 3
#define EAT_TIME 2

/* global vars */
pthread_mutex_t forks[NUM];

/* states enum */
enum {
	THINKING,
	EATING,
	HUNGRY,
	DEAD
} states[NUM];

/* functions */
void *philosopher( void * );

/* main */
int main()
{
	// initialize mutex
	for ( int i = 0; i < NUM; i++ ) {
		pthread_mutex_init( &forks[i], 0 );
	}

	// initialize threads and begin simulation
	pthread_t philosophers[NUM];
	int args[NUM];
	for ( int i = 0; i < NUM; i++ ) {
		args[i] = i;
		pthread_create( &philosophers[i], NULL, philosopher, &args[i] );
	}

	// join threads
	for ( int i = 0; i < NUM; i++ )
		pthread_join( philosophers[i], 0 );

	// destroy mutex
	for ( int i = 0; i < NUM; i++ ) {
		pthread_mutex_destroy( &forks[i] );
	}

	return 0;
}

void *philosopher( void *args )
{
	// initialize philosopher data
	int num = *( int *)args;
	int left = num;
	int right = (num + 1) % NUM;
	int hunger = HUNGER_MAX;
	int eating = EAT_TIME;
	states[num] = THINKING;
	int cycle = 1;

	do {
		switch( states[num] ) {
			// hunger is FULL, thinking
			case THINKING:
				fprintf( stderr, "(%d)T{%d}: philosopher is thinking...\n", cycle, num);
				break;

			// hunger is not FULL, looking for food
			case HUNGRY:
				// pick up left fork
				if ( pthread_mutex_trylock( &forks[left] ) == 0 )
				{ while ( pthread_mutex_trylock( &forks[right] ) != 0 ) {
					// right fork is unavailable, so put down left fork
					pthread_mutex_unlock( &forks[left] );
					// try again some time later
					sleep(CYCLE);
					// continue cycle
					hunger--;
					// return if dead
					if ( hunger == 0 ) break;
					fprintf( stderr, "(%d)H{%d}: philosopher is hungry! (%d cycles left)\n", cycle, num, hunger );
					cycle++;
					// lock left fork again to prevent stealing
					pthread_mutex_lock( &forks[left] );
				}
					// right fork was successfully picked up
					if ( hunger != 0 ) {
						fprintf( stderr, "(%d)F{%d}: philosopher has picked up forks{%d %d}.\n", cycle, num, left, right );
						states[num] = EATING;
					}
				}
				// could not pick up left fork
				else {
					fprintf( stderr, "(%d)H{%d}: philosopher is hungry! (%d cycles left)\n", cycle, num, hunger );
					break;
				}
				break;

			// philosopher has both forks, and begins eating
			case EATING:
				fprintf( stderr, "(%d)E{%d}: philosopher is eating. (%d cycles left)\n", cycle, num, eating);
				eating--;
				break;

			// philospher has starved and DIED
			case DEAD:
				fprintf( stderr, "(%d)D{%d}: philosopher has died!\n", cycle, num );
				exit(1);
				break;

			// philosopher state not defined
			default:
				fprintf( stderr, "Unknown state{%d} for philosopher{%d}\n", states[num], num);
				exit(1);
				break;
		}
		// cycle timer
		sleep(CYCLE);
		// hunger increases after every cycle
		hunger--;

		// if philosopher is eating, reset hunger
		if ( states[num] == EATING ) {
			hunger = HUNGER_MAX;
			// if philosopher is done eating, go back to thinking
			if ( eating <= 0 ) {
				eating = EAT_TIME;
				pthread_mutex_unlock( &forks[left] );
				pthread_mutex_unlock( &forks[right] );
				states[num] = THINKING;
			}
		}
		// if philosopher's hunger reaches 0, they die
		else if ( hunger <= 0 )
			states[num] = DEAD;
		// philosopher is hungry after thinking
		else if ( hunger < HUNGER_MAX )
			states[num] = HUNGRY;
		cycle++;
	} while (1);
}
