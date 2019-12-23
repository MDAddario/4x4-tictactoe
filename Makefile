SRCS = bit_ops.c tictactoe.c tree_traversal.c
OBJS = ${SRCS:.c=.o}
HDS = ${SRCS:.c=.h}

DIMENSION = 3
DEFINE = -DDIMENSION=${DIMENSION}

all: main

main: main.c ${OBJS}
	gcc main.c ${OBJS} -o main ${DEFINE}

.c.o: ${SRCS} ${HDS}
	gcc -c $< ${DEFINE}

clean:
	rm -f main ${OBJS}
	clear
