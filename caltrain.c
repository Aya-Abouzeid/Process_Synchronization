#include <pthread.h>
#include "caltrain.h"




void
station_init(struct station *station)
{

station -> currentPassengers = 0;
}

void
station_load_train(struct station *station, int count)
{
	if(count == 0){
        printf("heeere\n");
        return;
    }
    else{
        printf("heere2\n");
    }
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
