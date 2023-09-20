PROGRAM := marionette
SOURCE  := ./src
INCLUDE := -I./include/
CFLAGS  := $(shell pkg-config --cflags libevdev) -Wall
LDFLAGS := $(shell pkg-config --libs libevdev)

$(PROGRAM): $(wildcard $(SOURCE)/*.c)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
clean:
	rm -f $(PROGRAM)
