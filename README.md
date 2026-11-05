# Sincronização do Metrô

## Descrição

Este projeto implementa a sincronização entre passageiros e carros de uma estação de metrô utilizando threads POSIX (pthreads), mutexes e variáveis de condição.

O carro aguarda o embarque dos passageiros que receberam autorização, enquanto os passageiros esperam até que exista um carro com assentos disponíveis.

## Arquivos

- `sync_car.c`: implementação das funções de sincronização da estação.
- `sync_car.h`: declaração da estrutura da estação e das funções utilizadas.
- `sync_car-test.c`: teste local para verificar o comportamento do embarque.
- `sync_car-runner.c`: arquivo de teste fornecido pelo professor.
- `Makefile`: automatiza a compilação e a limpeza dos executáveis.

## Compilação

Para compilar o teste do professor, execute:

```bash
make
```

Isso gera o executável `sync_car-teste`.

## Execução

Para executar o teste do professor:

```bash
./sync_car-teste
```

Para compilar e executar o teste local:

```bash
make teste-local
```

## Limpeza

Para remover os executáveis gerados:

```bash
make clean
```

## Tecnologias

- Linguagem C
- Pthreads
- Mutexes
- Variáveis de condição
- Makefile
