#include "sync_car.h"

void estacao_init(struct estacao *estacao)
{
    pthread_mutex_init(&estacao->mutex, NULL);

    pthread_cond_init(&estacao->carro_disponivel, NULL);
    pthread_cond_init(&estacao->embarque_completo, NULL);

    estacao->carro_presente = 0;
    estacao->assentos_livres = 0;

    estacao->passageiros_esperando = 0;
    estacao->passageiros_para_embarcar = 0;
    estacao->passageiros_embarcados = 0;
}