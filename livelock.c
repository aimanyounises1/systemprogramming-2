#include <pthread.h> 
#include <semaphore.h> 
#include <stdio.h> 
#include <unistd.h>

#define N 5 
#define THINKING 2 
#define HUNGRY 1 
#define EATING 0 
#define LEFT (phnum + 4) % N 
#define RIGHT (phnum + 1) % N 

int state[N]; 
int phil[N] = { 0, 1, 2, 3, 4 }; 

sem_t mutex; 
sem_t S[N];
/*
The Philosphers won't eat because every philospher will pich up the left chopsticks
then there is no philospher can eat the will wait until one of this philosphers leave the chopsticks 
to eat,The philosphers will keep trying to pick up the right chopsticks but they won't and here is a deadlock
We imagine that the left chopsticks is the source R1 to hold then  he needs the R2 and it's in our case is
The right chopsticks to start eating    
Here is an example of Output:
Philosopher 1 is thinking
Philosopher 2 is thinking
Philosopher 3 is thinking
Philosopher 4 is thinking
Philosopher 5 is thinking
Philosopher 1 is Hungry
Philosopher 1 takes fork 5 
Philosopher 3 takes fork 2 
Philosopher 2 is Hungry
Philosopher 2 takes fork 1 
Philosopher 4 takes fork 3 
Philosopher 3 is Hungry
Philosopher 5 takes fork 4 
Philosopher 4 is Hungry
Philosopher 5 is Hungry
Philosopher 2 putting fork 1 down
Philosopher 2 is thinking
Philosopher 4 putting fork 3 down
Philosopher 4 is thinking
Philosopher 3 putting fork 2 down
Philosopher 3 is thinking
Philosopher 2 is Hungry
Philosopher 5 putting fork 4 down
Philosopher 5 is thinking
Philosopher 2 putting fork 1 down
Philosopher 2 is thinking
Philosopher 4 is Hungry
Philosopher 3 is Hungry
Philosopher 5 is Hungry
Philosopher 2 is Hungry
Philosopher 4 putting fork 3 down
Philosopher 4 is thinking
Philosopher 3 putting fork 2 down
Philosopher 3 is thinking
Philosopher 5 putting fork 4 down
Philosopher 5 is thinking
Philosopher 3 is Hungry
Philosopher 4 is Hungry
Philosopher 2 putting fork 1 down
Philosopher 2 is thinking
Philosopher 5 is Hungry
Philosopher 2 is Hungry
Philosopher 4 putting fork 3 down
Philosopher 4 is thinking
Philosopher 3 putting fork 2 down
Philosopher 3 is thinking
Philosopher 5 putting fork 4 down
Philosopher 5 is thinking
Philosopher 4 is Hungry
Philosopher 2 putting fork 1 down
Philosopher 2 is thinking
Philosopher 3 is Hungry
Philosopher 5 is Hungry
Philosopher 2 is Hungry
Philosopher 3 putting fork 2 down
Philosopher 3 is thinking
Philosopher 4 putting fork 3 down
Philosopher 4 is thinking
Philosopher 5 putting fork 4 down
Philosopher 5 is thinking
*/
void test(int phnum) 
{ 
	if (state[LEFT] != EATING) { 
		// state that eating 
		state[LEFT] = EATING; 

		sleep(2); 

		//pick up left fork
		printf("Philosopher %d takes fork %d \n", RIGHT+1, phnum + 1);

		// sem_post(&S[phnum]) has no effect 
		// during takefork 
		// used to wake up hungry philosophers 
		// during putfork 
		sem_post(&S[LEFT+1]); 
	} 

	if (state[phnum-1] != EATING ) { 
		// state that eating 
		state[phnum-1] = EATING; 

		sleep(2); 

		//pick up right fork 
		printf("Philosopher %d takes fork %d \n", phnum, phnum);
		printf("Philosopher %d is Eating\n", phnum + 1); 

		// sem_post(&S[phnum]) has no effect 
		// during takefork 
		// used to wake up hungry philosophers 
		// during putfork 
		sem_post(&S[RIGHT+1]);  
	}	
} 

// take up chopsticks 
void take_fork(int phnum) 
{ 

	sem_wait(&mutex); 

	// state that hungry 
	//state[phnum] = HUNGRY; 

	printf("Philosopher %d is Hungry\n", phnum + 1);
 

	// eat if neighbours are not eating 
	test(LEFT);
	//sleep(2);
	test(RIGHT); 

	sem_post(&mutex); 

	// if unable to eat wait to be signalled 
	sem_wait(&S[phnum]); 

	sleep(1); 
} 

// put down chopsticks 
void put_fork(int phnum) 
{ 

	sem_wait(&mutex); 

	// state that thinking 
	//state[phnum] = THINKING; 

	//put left fork down
	printf("Philosopher %d putting fork %d down\n", phnum + 1, LEFT + 1);
	//put right fork down
	//printf("Philosopher %d putting fork %d down\n", phnum + 1, phnum + 1); 
	printf("Philosopher %d is thinking\n", phnum + 1);
	
	sem_post(&S[phnum]); 

	test(RIGHT);
	//sleep(2); 
	test(LEFT); 

	sem_post(&mutex); 
} 

void* philospher(void* num) 
{ 

	while (1) { 

		int* i = num; 

		sleep(1); 

		take_fork(*i);
		//take_fork((*i + 1) % N); 

		sleep(0); 

		//put_fork((*i + 1) % N);
		put_fork(*i);  
	} 
} 

int main() 
{ 

	int i; 
	pthread_t thread_id[N]; 

	// initialize the mutexes 
	sem_init(&mutex, 0, 1); 

	for (i = 0; i < N; i++) 

		sem_init(&S[i], 0, 0); 

	for (i = 0; i < N; i++) { 

		// create philosopher processes 
		pthread_create(&thread_id[i], NULL, 
					philospher, &phil[i]);
		state[i] = THINKING; 

		printf("Philosopher %d is thinking\n", i + 1); 
	} 

	for (i = 0; i < N; i++) 

		pthread_join(thread_id[i], NULL); 
} 