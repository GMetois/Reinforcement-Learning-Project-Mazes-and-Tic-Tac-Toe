CC=gcc
CFLAGS=-W -Wall
INC=-I include/
SRC=src/
EXEC=dfs

all: $(EXEC) clean

dfs: $(SRC)dfs.o $(SRC)functions.o $(SRC)mazeEnv.o
	$(CC) $(INC) -o $(SRC)$@ $^ $(CFLAGS)

$(SRC)%.o : $(SRC)%.c
	$(CC) $(INC) -o $@ -c $< $(CFLAGS)

clean: 
	rm -rf $(SRC)*.o