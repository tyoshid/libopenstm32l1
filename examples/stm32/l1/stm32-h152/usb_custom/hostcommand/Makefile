#

NAME = usbgpio

OBJECTS = $(NAME).o

CC = gcc
CFLAGS = $(CPPFLAGS) -O -g -Wall

PROGRAM = $(NAME)


all: $(PROGRAM)

.c.o:
	$(CC) $(CFLAGS) -c $<

$(PROGRAM): $(OBJECTS)
	$(CC) -o $(PROGRAM) $(OBJECTS) -lusb-1.0

clean:
	rm -f *.o $(PROGRAM)
