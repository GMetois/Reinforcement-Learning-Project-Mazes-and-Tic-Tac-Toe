CC=gcc
CFLAGS=-W -Wall -lm
INC=-I include/
SRC=src/
EXEC=SARSA

all: $(EXEC) clean

SARSA: $(SRC)SARSA.o $(SRC)TTTEnv.o
	$(CC) $(INC) -g -o $(SRC)$@ $^ $(CFLAGS)

$(SRC)%.o : $(SRC)%.c
	$(CC) $(INC) -g -o $@ -c $< $(CFLAGS)

clean: 
	rm -rf $(SRC)*.o