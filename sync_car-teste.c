#include <stdio.h>
#include "sync_car.h"

int main(void)
{
    struct estacao estacao;

    estacao_init(&estacao);

    if (pthread_mutex_lock(&estacao.mutex) != 0) {
        printf("Erro ao utilizar o mutex.\n");
        return 1;
    }

    if (estacao.carro_presente != 0 ||
        estacao.assentos_livres != 0 ||
        estacao.passageiros_esperando != 0 ||
        estacao.passageiros_para_embarcar != 0 ||
        estacao.passageiros_embarcados != 0) {

        printf("Erro: estado inicial incorreto.\n");
        pthread_mutex_unlock(&estacao.mutex);
        return 1;
    }

    pthread_mutex_unlock(&estacao.mutex);

    pthread_cond_signal(&estacao.carro_disponivel);
    pthread_cond_broadcast(&estacao.embarque_completo);

    pthread_mutex_destroy(&estacao.mutex);
    pthread_cond_destroy(&estacao.carro_disponivel);
    pthread_cond_destroy(&estacao.embarque_completo);

    printf("Estacao inicializada com sucesso.\n");
    return 0;
}