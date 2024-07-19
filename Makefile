FLAGS=-O2 -Wall -std=c2x

all: clean tree

cache: tree.o
	gcc ${FLAGS} $^ -o $@ ${LDFLAGS}

tree.o: tree.c
	gcc ${FLAGS} -c $^

clean:
	rm -rf *.o tree
