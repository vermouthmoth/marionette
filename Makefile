PROGRAM      := marionette
SOURCE       := ./src
INCLUDE      := -I./include/
CONFIGURATOR := gen_config
CONFIGFILE   := ./config
CFLAGS       := $(shell pkg-config --cflags libevdev)
CFLAGS       += $(shell xml2-config --cflags)
CFLAGS       += -Wall
LDFLAGS      := $(shell pkg-config --libs libevdev)
LDFLAGS      += $(shell xml2-config --libs)

all: $(PROGRAM) $(CONFIGURATOR)
$(PROGRAM): $(filter-out $(SOURCE)/$(CONFIGURATOR).c, $(wildcard $(SOURCE)/*.c))
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
$(CONFIGURATOR): $(SOURCE)/$(CONFIGURATOR).c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
clean:
	rm -f $(PROGRAM) $(CONFIGURATOR)
