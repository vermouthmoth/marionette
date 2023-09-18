CFLAGS = $(shell pkg-config --cflags libevdev) -Wall
LDFLAGS = $(shell pkg-config --libs libevdev)

marionette: marionette.o config.h
marionette.o: config.h
clean:
	rm -f marionette.o marionette
