#include <pthread.h>

struct station {

    int maxPassengers;                  //maximum number of seats in a train
    int onBoardPassengers;              //number of passengers seated
    int waitingPassengers;              //number of waiting passengers in the station
    pthread_mutex_t trainMutex;
    pthread_cond_t trainArrived;        //condition variable to indicate the arrival of a train
    pthread_cond_t trainLeaving;           //condition variable to indicate the departure of a train

};

void station_init(struct station *station);

void station_load_train(struct station *station, int count);

void station_wait_for_train(struct station *station);

void station_on_board(struct station *station);
