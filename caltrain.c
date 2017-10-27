#include <pthread.h>
#include "caltrain.h"



void
station_init(struct station *station)
{
    station->maxPassengers = 0;                                         //free seats available.
    station->onBoardPassengers = 0;                                     //onboard passengers attribute to.
    station->waitingPassengers = 0;                                     //waiting passengers attribute to.
    station->realBoarding = 0;                                          //passengers who exited the while loop waiting for train arrival.
    pthread_mutex_init(&(station->mutex) , NULL);                       //initialize train pthread mutex.
    pthread_cond_init(&(station->trainArrived), NULL);                  //initialize train arrival pthread condition.
    pthread_cond_init(&(station->trainLeaving) , NULL);                 //initialize train departure pthread condition.
}

void
station_load_train(struct station *station, int count)
{

    pthread_mutex_lock(&station->mutex);                                  //enters critical section for a train. makes sure only one train arrive at once.

	if(count == 0 || station->waitingPassengers == 0 ){
            pthread_mutex_unlock(&station->mutex);                        //exits the critical section of the train if no waiting passengers or no available -
            return;                                                         //-seats on the train
    }

    station->maxPassengers = count;                                       //initialize maximum passengers attribute with the value sent to function.
    pthread_cond_broadcast(&station->trainArrived);                       //broadcasts to all threads the arrival of the train.
    while(station->waitingPassengers != 0 && (station->maxPassengers - station->onBoardPassengers) > 0 ){
            //train mutex will wait while number of waiting passengers haven't reached 0 and while free seats are available.
            pthread_cond_wait(&station->trainLeaving,&station->mutex);

    }
    station->realBoarding = 0;
    station->onBoardPassengers = 0;                                        //resets onboard passengers attribute to 0 to be ready for another train.
    station->maxPassengers = 0;                                            //resets maxPassengers attribute to 0 to be ready for another train,without this line threads may not wait if new train arrived
    pthread_mutex_unlock(&station->mutex);                                 //exits the critical section of the train.
}

void
station_wait_for_train(struct station *station)
{
    pthread_mutex_lock(&station->mutex);                                    //enters critical section.
    station->waitingPassengers++;                                           //waiting passengers increased by one.
    while( (station->maxPassengers - ((station->realBoarding)+1)) < 0 ){    //wait while adding one more passenger is not acceptable

            pthread_cond_wait(&station->trainArrived,&station->mutex);      //mutex will wait for the train arrival condition.
    }
    station->realBoarding++;                                               //use extra counter to indicate the real boarded passengers so that no extra passengers exit while loop
    pthread_mutex_unlock(&station->mutex);                                  //exits the critical section.
}
void
station_on_board(struct station *station)
{
    pthread_mutex_lock(&station->mutex);                                   //enters critical section.
    station->onBoardPassengers++;                                          //onBoard passengers increased by one.
    station->waitingPassengers--;                                          //waiting passengers decreased by one.

    if(station->waitingPassengers == 0 || station->maxPassengers == station->onBoardPassengers){        //check if waiting passengers =0 or train is full for the train to leave
            pthread_cond_signal(&station->trainLeaving);
    }
    pthread_mutex_unlock(&station->mutex);                                 //exits the critical section.
}
