#

NAME = radio

OBJECTS = $(NAME).o

CC = gcc
CFLAGS = $(CPPFLAGS) -O -g -Wall

PROGRAM = $(NAME)


all: $(PROGRAM)

.c.o:
	$(CC) $(CFLAGS) -c $<

$(PROGRAM): $(OBJECTS)
	$(CC) -o $(PROGRAM) $(OBJECTS) -ljack

clean:
	rm -f *.o $(PROGRAM)
