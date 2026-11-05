#ifndef SYNC_CAR_H
#define SYNC_CAR_H

#include <pthread.h>

struct station {
    pthread_mutex_t mutex;
    pthread_cond_t car_available;
    pthread_cond_t boarding_complete;

    int car_present;
    int free_seats;
    int waiting_passengers;
    int passengers_to_board;
    int passengers_admitted;
    int passengers_boarded;
};

void station_init(struct station *station);
void station_load_car(struct station *station, int count);
void station_wait_for_car(struct station *station);
void station_on_board(struct station *station);

#endif