#ifndef SYNC_CAR_H
#define SYNC_CAR_H

#include <pthread.h>

struct estacao {
    pthread_mutex_t mutex;

    pthread_cond_t carro_disponivel;
    pthread_cond_t embarque_completo;

    int carro_presente;
    int assentos_livres;

    int passageiros_esperando;
    int passageiros_para_embarcar;
    int passageiros_embarcados;
};

void estacao_init(struct estacao *estacao);

void estacao_carregar_carro(struct estacao *estacao, int quantidade);
void estacao_esperar_carro(struct estacao *estacao);
void estacao_ao_embarcar(struct estacao *estacao);

#endif