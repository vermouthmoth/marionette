CFLAGS = $(shell pkg-config --cflags libevdev) -Wall
LDFLAGS = $(shell pkg-config --libs libevdev)

marionette: $(wildcard *.c)
clean:
	rm -f marionette
