//Logan Padon, Project 2

#include <stdlib.h> //atoi
#include <iostream> //cout
//#include <fstream>
#include <stdexcept> //invalid_argument
#include <pthread.h> //pthread_create, pthread_t, pthread_mutex_t, pthread_mutex_lock, pthread_mutex_unlock, pthread_exit
#include <semaphore.h> //sem_t, sem_wait. sem_post

using namespace std;

pthread_mutex_t count_mutex; //Name of the mutex for the tutor
sem_t chair_sem; //Name of the semaphore for the chairs
int visit; //Number of visits before a thread is done, needs to be declared here so it's consistent in threads

void *TutorVisit(void* threadid); //Simulates the visits to the tutor

int main(int argc, char *argv[]){
	if(argc != 4){ //Needs 4 arguments
		cout << "Incorrect number of arguments" << endl;
		return 1; //Error
	}
	int numbers[3];
	for(int i = 1; i < argc; i++){ //All arguments past the name of the program must be ints
		try {
			numbers[i-1] = atoi(argv[i]); //Converts args to numbers
		}
		catch (const invalid_argument& e){
			cout << "Invalid arguments" << endl;
			return 1; //Error
		}
	}
	visit = numbers[2]; //Sets the number of chairs globally
	pthread_t threads[numbers[0]]; //Creates an array of possible threads
	sem_init(&chair_sem, 0, numbers[1]); //Creates a semaphore to represent the chairs
        for (int i = 1; i <= numbers[0]; i++){ //Creates threads with names for the list
		pthread_create(&threads[i], NULL, TutorVisit,(void *)i); //Creates a thread per student
	}

}
void *TutorVisit(void *threadid){
	int count = 0; //Initializes total number of visits to the tutor at 0
        long tid = (long)threadid; //Returns the thread id as a usable number
	cout << "Hello World, Student " << tid << " reporting." << endl; //Thread reports its creation
	do{
		cout << "Student " << tid << " is in the waiting room." << endl;
		sem_wait(&chair_sem);
		cout << "Student " << tid << " is in a chair." << endl;
		pthread_mutex_lock(&count_mutex);
			cout << "The tutor is with student " << tid << endl;
			sem_post(&chair_sem);
			count = count + 1;
		pthread_mutex_unlock(&count_mutex);
		cout << "The tutor is free." << endl;
	} while(count < visit); //Repeats until the count reaches the appropriate number
	cout << "Student " << tid << " has visited the tutor the requisite number of times." << endl;
	pthread_exit(NULL);
}
