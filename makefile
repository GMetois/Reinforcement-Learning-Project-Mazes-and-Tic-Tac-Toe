CC=gcc
CFLAGS=-W -Wall -lm
INC=-I include/
SRC=src/
EXEC=QLearning

all: $(EXEC)

QLearning: $(SRC)QLearning.o $(SRC)functions.o $(SRC)mazeEnv.o
	$(CC) $(INC) -g -o $(SRC)$@ $^ $(CFLAGS)

$(SRC)%.o : $(SRC)%.c
	$(CC) $(INC) -g -o $@ -c $< $(CFLAGS)

clean: 
	rm -rf $(SRC)*.o