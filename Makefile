
CC = gcc
CFLAGS = -Wall -Wextra -pthread

PROGRAMA = sync_car-teste
TESTE_LOCAL = sync_car-local

all: $(PROGRAMA)

$(PROGRAMA): sync_car.c sync_car.h sync_car-runner.c
	$(CC) $(CFLAGS) sync_car.c sync_car-runner.c -o $(PROGRAMA)

$(TESTE_LOCAL): sync_car.c sync_car.h sync_car-teste.c
	$(CC) $(CFLAGS) sync_car.c sync_car-teste.c -o $(TESTE_LOCAL)

teste-local: $(TESTE_LOCAL)
	./$(TESTE_LOCAL)

clean:
	rm -f $(PROGRAMA) $(TESTE_LOCAL)

.PHONY: all teste-local clean
