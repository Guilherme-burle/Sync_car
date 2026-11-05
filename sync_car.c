#include "sync_car.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void station_init(struct station *station)
{
    if (station == NULL)
        return;

    int result = pthread_mutex_init(&station->mutex, NULL);

    if (result != 0) {
        fprintf(stderr, "Erro ao inicializar mutex: %s\n", strerror(result));
        exit(EXIT_FAILURE);
    }

    result = pthread_cond_init(&station->car_available, NULL);

    if (result != 0) {
        fprintf(stderr, "Erro ao inicializar car_available: %s\n",
                strerror(result));
        pthread_mutex_destroy(&station->mutex);
        exit(EXIT_FAILURE);
    }

    result = pthread_cond_init(&station->boarding_complete, NULL);

    if (result != 0) {
        fprintf(stderr, "Erro ao inicializar boarding_complete: %s\n",
                strerror(result));
        pthread_cond_destroy(&station->car_available);
        pthread_mutex_destroy(&station->mutex);
        exit(EXIT_FAILURE);
    }

    station->car_present = 0;
    station->free_seats = 0;
    station->waiting_passengers = 0;
    station->passengers_to_board = 0;
    station->passengers_admitted = 0;
    station->passengers_boarded = 0;
}

void station_load_car(struct station *station, int count)
{
    if (station == NULL || count <= 0)
        return;

    pthread_mutex_lock(&station->mutex);

    if (station->waiting_passengers == 0) {
        pthread_mutex_unlock(&station->mutex);
        return;
    }

    station->car_present = 1;
    station->free_seats = count;

    if (station->waiting_passengers < count)
        station->passengers_to_board = station->waiting_passengers;
    else
        station->passengers_to_board = count;

    station->passengers_admitted = 0;
    station->passengers_boarded = 0;

    pthread_cond_broadcast(&station->car_available);

    while (station->passengers_boarded < station->passengers_to_board)
        pthread_cond_wait(&station->boarding_complete, &station->mutex);

    station->car_present = 0;
    station->free_seats = 0;
    station->passengers_to_board = 0;
    station->passengers_admitted = 0;
    station->passengers_boarded = 0;

    pthread_mutex_unlock(&station->mutex);
}

void station_wait_for_car(struct station *station)
{
    if (station == NULL)
        return;

    pthread_mutex_lock(&station->mutex);

    station->waiting_passengers++;

    while (!station->car_present ||
           station->passengers_admitted >= station->passengers_to_board) {
        pthread_cond_wait(&station->car_available, &station->mutex);
    }

    station->waiting_passengers--;
    station->passengers_admitted++;
    station->free_seats--;

    pthread_mutex_unlock(&station->mutex);
}

void station_on_board(struct station *station)
{
    if (station == NULL)
        return;

    pthread_mutex_lock(&station->mutex);

    if (station->passengers_boarded < station->passengers_to_board) {
        station->passengers_boarded++;

        if (station->passengers_boarded == station->passengers_to_board)
            pthread_cond_signal(&station->boarding_complete);
    }

    pthread_mutex_unlock(&station->mutex);
}