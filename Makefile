CC      := clang
PKGS	:= alsa
CFLAGS	:= -Wall -Wextra -std=gnu99 -pedantic $(shell pkg-config --cflags $(PKGS))
LDFLAGS := -lm $(shell pkg-config --libs $(PKGS))
SRC     := $(wildcard src/*.c)
HDR     := $(wildcard src/*.h)

ifdef DEBUG
CFLAGS += -DDEBUG
endif

midihack: $(SRC) $(HDR)
	$(CC) $(CFLAGS) -o $@ $(SRC) $(LDFLAGS)

.PHONY: clean
clean: ; rm -f midihack

.PHONY: install
install: midihack
	cp midihack $(HOME)/bin/

.PHONY: run
run: midihack; ./midihack
