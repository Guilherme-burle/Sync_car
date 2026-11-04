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
    estacao->passageiros_admitidos = 0;
    estacao->passageiros_embarcados = 0;
}

void estacao_esperar_carro(struct estacao *estacao)
{
    pthread_mutex_lock(&estacao->mutex);

    estacao->passageiros_esperando++;

    while (!estacao->carro_presente ||
           estacao->passageiros_admitidos >= estacao->passageiros_para_embarcar) {
        pthread_cond_wait(&estacao->carro_disponivel, &estacao->mutex);
    }

    estacao->passageiros_esperando--;
    estacao->passageiros_admitidos++;

    pthread_mutex_unlock(&estacao->mutex);
}

void estacao_carregar_carro(struct estacao *estacao, int quantidade)
{
    pthread_mutex_lock(&estacao->mutex);

    estacao->carro_presente = 0;
    estacao->assentos_livres = 0;
    estacao->passageiros_para_embarcar = 0;
    estacao->passageiros_admitidos = 0;
    estacao->passageiros_embarcados = 0;

    if (quantidade > 0 && estacao->passageiros_esperando > 0) {
        estacao->assentos_livres = quantidade;

        estacao->passageiros_para_embarcar =
            estacao->passageiros_esperando < quantidade ?
            estacao->passageiros_esperando : quantidade;

        estacao->carro_presente = 1;

        pthread_cond_broadcast(&estacao->carro_disponivel);
    }

    pthread_mutex_unlock(&estacao->mutex);
}

void estacao_ao_embarcar(struct estacao *estacao)
{
    pthread_mutex_lock(&estacao->mutex);

    estacao->passageiros_embarcados++;

    pthread_mutex_unlock(&estacao->mutex);
}