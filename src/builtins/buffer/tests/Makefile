MAIN = buffer_test

CC = gcc
CFLAGS = -I ../../../ -Wall -Wextra -Werror -std=c99 -export-dynamic -lm -g

OBJ = buffer_test.o test.o \
	test_destructor.o test_constructor.o \
	../public/constructor.o ../public/destructor.o  ../public/operations.o ../public/getter.o \
	../private/operations.o ../private/destructor.o \
	../../../utils/base64.o ../../../utils/hex.o

all: $(MAIN)

$(MAIN): $(OBJ)
	$(CC) -o $(MAIN) $(OBJ) $(CFLAGS)

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean

clean:
	${RM} ${MAIN} *.o