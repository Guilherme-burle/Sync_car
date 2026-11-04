#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "sync_car.h"

#define TOTAL_PASSAGEIROS 3

struct argumentos_passageiro {
    struct estacao *estacao;
    int id;
};

void *thread_passageiro(void *arg)
{
    struct argumentos_passageiro *args = arg;

    printf("Passageiro %d aguardando.\n", args->id);

    estacao_esperar_carro(args->estacao);

    printf("Passageiro %d autorizado a embarcar.\n", args->id);

    estacao_ao_embarcar(args->estacao);

    return NULL;
}

int main(void)
{
    struct estacao estacao;
    pthread_t threads[TOTAL_PASSAGEIROS];
    struct argumentos_passageiro args[TOTAL_PASSAGEIROS];

    estacao_init(&estacao);

    for (int i = 0; i < TOTAL_PASSAGEIROS; i++) {
        args[i].estacao = &estacao;
        args[i].id = i + 1;

        if (pthread_create(&threads[i], NULL, thread_passageiro, &args[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    usleep(200000);

    printf("\nCarro chegou com 2 assentos.\n");
    estacao_carregar_carro(&estacao, 2);

    usleep(200000);

    printf("\nSegundo carro chegou com 1 assento.\n");
    estacao_carregar_carro(&estacao, 1);

    for (int i = 0; i < TOTAL_PASSAGEIROS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\nTodos os passageiros foram liberados.\n");

    pthread_mutex_destroy(&estacao.mutex);
    pthread_cond_destroy(&estacao.carro_disponivel);
    pthread_cond_destroy(&estacao.embarque_completo);

    return 0;
}