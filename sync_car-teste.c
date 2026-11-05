#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "sync_car.h"

#define TOTAL_PASSAGEIROS 2

struct argumentos_passageiro {
    struct estacao *estacao;
    int id;
};

struct argumentos_carro {
    struct estacao *estacao;
};

int carro_retornou = 0;

void *thread_passageiro(void *arg)
{
    struct argumentos_passageiro *args = arg;

    printf("Passageiro %d aguardando.\n", args->id);

    estacao_esperar_carro(args->estacao);

    printf("Passageiro %d autorizado a embarcar.\n", args->id);

    usleep(300000);

    estacao_ao_embarcar(args->estacao);

    printf("Passageiro %d embarcou.\n", args->id);

    return NULL;
}

void *thread_carro(void *arg)
{
    struct argumentos_carro *args = arg;

    printf("Carro chegou com 2 assentos.\n");

    estacao_carregar_carro(args->estacao, 2);

    __atomic_store_n(&carro_retornou, 1, __ATOMIC_SEQ_CST);

    printf("Carro partiu.\n");

    return NULL;
}

int main(void)
{
    struct estacao estacao;
    pthread_t passageiros[TOTAL_PASSAGEIROS];
    pthread_t carro;

    struct argumentos_passageiro dados_passageiros[TOTAL_PASSAGEIROS];
    struct argumentos_carro dados_carro = {&estacao};

    estacao_init(&estacao);

    for (int i = 0; i < TOTAL_PASSAGEIROS; i++) {
        dados_passageiros[i].estacao = &estacao;
        dados_passageiros[i].id = i + 1;

        if (pthread_create(&passageiros[i], NULL, thread_passageiro, &dados_passageiros[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    usleep(200000);

    if (pthread_create(&carro, NULL, thread_carro, &dados_carro) != 0) {
        perror("pthread_create");
        return 1;
    }

    usleep(50000);

    if (__atomic_load_n(&carro_retornou, __ATOMIC_SEQ_CST)) {
        printf("ERRO: o carro retornou antes do embarque.\n");
        return 1;
    }

    printf("Teste: carro continua aguardando o embarque.\n");

    for (int i = 0; i < TOTAL_PASSAGEIROS; i++) {
        pthread_join(passageiros[i], NULL);
    }

    pthread_join(carro, NULL);

    pthread_mutex_destroy(&estacao.mutex);
    pthread_cond_destroy(&estacao.carro_disponivel);
    pthread_cond_destroy(&estacao.embarque_completo);

    printf("Teste concluido com sucesso.\n");

    return 0;
}