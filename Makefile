CC = gcc
CFLAGS = -Wall -Werror -Wextra -O2 -lraylib -lm -isystem lib
CFLAGS_DEBUG = -Wall -Werror -Wextra -g -fsanitize=address,undefined -lraylib -lm -isystem lib
COMPILE_OBJ = @mkdir -p $(BLDDIR) && $(CC) $(CFLAGS) -I$(SRCDIR) -c $< -o $@
COMPILE_EXC = @mkdir -p $(BLDDIR) && $(CC) $(CFLAGS) -I$(SRCDIR) -o $@ $^ -lsqlite3
COMPILE_LIB = @mkdir -p $(BLDDIR) && ar rcs $@ $^
BLDDIR = build
SRCDIR = src
INCDIR = include
TSTDIR = tests
PREFIX = /usr/local

$(BLDDIR)/main: $(BLDDIR)/main.o $(BLDDIR)/draw.o $(BLDDIR)/work.o
	$(COMPILE_EXC)

$(BLDDIR)/main.o: $(SRCDIR)/main.c $(SRCDIR)/draw.h $(SRCDIR)/work.h
	$(COMPILE_OBJ)

$(BLDDIR)/draw.o: $(SRCDIR)/draw.c $(SRCDIR)/draw.h $(SRCDIR)/work.h
	$(COMPILE_OBJ)

$(BLDDIR)/work.o: $(SRCDIR)/work.c $(SRCDIR)/draw.h $(SRCDIR)/work.h
	$(COMPILE_OBJ)

.PHONY: check debug install uninstall clean

check:
	LC_ALL=C ./checkpatch.pl --strict --ignore LONG_LINE_STRING --ignore SPDX_LICENSE_TAG --no-tree -f $(SRCDIR)/*.c $(SRCDIR)/*.h

debug: CFLAGS = $(CFLAGS_DEBUG)
debug: $(BLDDIR)/main

install:
	#TODO
uninstall:
	#TODO
clean:
	rm -rf $(BLDDIR)