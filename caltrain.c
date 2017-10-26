#include <pthread.h>
#include "caltrain.h"




void
station_init(struct station *station)
{
    station -> onBoardPassengers = 0;                       //initialize onboard passengers attribute to 0.
    station -> waitingPassengers = 0;                       //initialize waiting passengers attribute to 0.
    pthread_mutex_init(&(station -> trainMutex) , NULL);    //initialize train pthread mutex.
    pthread_cond_init(&(station -> trainArrived), NULL);    //initialize train arrival pthread condition.
    pthread_cond_init(&(station -> trainLeaving) , NULL);      //initialize train departure pthread condition.
}

void
station_load_train(struct station *station, int count)
{
    pthread_mutex_lock(&station -> trainMutex);             //enters critical section for a train. makes sure only one train arrive at once.
	if(count == 0 || station -> waitingPassengers == 0 ){
	    pthread_mutex_unlock(&station -> trainMutex);       //exits the critical section of the train if no waiting passengers or no available -
        return;                                                //-seats on the train
    }
    station -> maxPassengers = count;                       //initialize maximum passengers attribute with the value sent to function.
    pthread_cond_broadcast(&station -> trainArrived);       //broadcasts to all threads the arrival of the train.
    while(station -> waitingPassengers != 0 && (station -> maxPassengers - station -> onBoardPassengers) > 0 ){
        //train mutex will wait while number of waiting passengers haven't reached 0 and while free seats are available.
        pthread_cond_wait(&station -> trainLeaving,&station -> trainMutex);
    }
    station -> onBoardPassengers = 0;                       //resets onboard passengers attribute to 0 to be ready for another train.
    pthread_mutex_unlock(&station -> trainMutex);           //exits the critical section of the train.

}

void
station_wait_for_train(struct station *station)
{
//	pthread_mutex_t mutex;
//	pthread_cond_wait (train,mutex);



}

void
station_on_board(struct station *station)
{
	// FILL ME IN
}
