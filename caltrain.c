#include <pthread.h>
#include "caltrain.h"



void
station_init(struct station *station)
{
    station->maxPassengers = 0;
    station->onBoardPassengers = 0;                       //initialize onboard passengers attribute to 0.
    station->waitingPassengers = 0;                       //initialize waiting passengers attribute to 0.
    pthread_mutex_init(&(station->mutex) , NULL);    //initialize train pthread mutex.
    pthread_cond_init(&(station->trainArrived), NULL);    //initialize train arrival pthread condition.
    pthread_cond_init(&(station->trainLeaving) , NULL);   //initialize train departure pthread condition.
}

void
station_load_train(struct station *station, int count)
{
    printf("cooount= %d \n",count);

    pthread_mutex_lock(&station->mutex);             //enters critical section for a train. makes sure only one train arrive at once.

	    printf("waitiiiiiiiiiiing= %d \n",station->waitingPassengers );

	if(count == 0 || station->waitingPassengers == 0 ){

            pthread_mutex_unlock(&station->mutex);   //exits the critical section of the train if no waiting passengers or no available -
            return;                                           //-seats on the train
    }

    station->maxPassengers = count;                       //initialize maximum passengers attribute with the value sent to function.
	    printf("maxpassengeweeeeeeeeeeeer= %d \n",station->maxPassengers);

    pthread_cond_broadcast(&station->trainArrived);       //broadcasts to all threads the arrival of the train.

    while(station->waitingPassengers != 0 && (station->maxPassengers - station->onBoardPassengers) > 0 ){
            //train mutex will wait while number of waiting passengers haven't reached 0 and while free seats are available.
            pthread_cond_wait(&station->trainLeaving,&station->mutex);

    }

    station->onBoardPassengers = 0;                       //resets onboard passengers attribute to 0 to be ready for another train.
    pthread_mutex_unlock(&station->mutex);           //exits the critical section of the train.

}

void
station_wait_for_train(struct station *station)
{
    pthread_mutex_lock(&station->mutex);              //enters critical section.
    station->waitingPassengers++;                    //waiting passengers increased by one.
    printf("Increasing Waiting = %d \n", station->waitingPassengers);
    printf("maxpassenger= %d \n",station->maxPassengers);
        printf("onboard= %d \n",(station->onBoardPassengers)+1);
    while((station->maxPassengers - ((station->onBoardPassengers)+1)) < 0 ){ //wait while adding one more passenger is not acceptable
            pthread_cond_wait(&station->trainArrived,&station->mutex);      //mutex will wait for the train arrival condition.
        printf("heeeeeere\n");

    }
    station->waitingPassengers--;                    //waiting passengers decreased by one.
    station->onBoardPassengers++;                    //onBoard passengers increased by one.
    pthread_mutex_unlock(&station->mutex);           //exits the critical section.
}

void
station_on_board(struct station *station)
{
        pthread_mutex_lock(&station->mutex);              //enters critical section.

    if(station->waitingPassengers == 0 || (station->maxPassengers - station->onBoardPassengers) == 0){
        pthread_cond_signal(&station->trainLeaving);
    }
        pthread_mutex_unlock(&station->mutex);           //exits the critical section.

}
