CC = gcc
CFLAGS = -Wall -Werror -Wextra -O2 -isystem lib
CFLAGS_DEBUG = -Wall -Werror -Wextra -g -fsanitize=address,undefined -isystem lib
COMPILE_OBJ = @mkdir -p $(BLDDIR) && $(CC) $(CFLAGS) -I$(SRCDIR) -c $< -o $@
COMPILE_EXC = @mkdir -p $(BLDDIR) && $(CC) $(CFLAGS) -I$(SRCDIR) -o $@ $^ $(LIBS)
COMPILE_LIB = @mkdir -p $(BLDDIR) && ar rcs $@ $^
BLDDIR = build
SRCDIR = src
INCDIR = include
TSTDIR = tests
PREFIX = /usr/local
LIBS = -lraylib -lm
WINCC = x86_64-w64-mingw32-gcc
WINDIR = lib-win
WINLIBS = -L$(WINDIR)/lib -static -lraylib -lopengl32 -lgdi32 -lwinmm -lm

$(BLDDIR)/bingo: $(BLDDIR)/main.o $(BLDDIR)/draw.o $(BLDDIR)/work.o
	$(COMPILE_EXC)

$(BLDDIR)/main.o: $(SRCDIR)/main.c $(SRCDIR)/draw.h $(SRCDIR)/work.h
	$(COMPILE_OBJ)

$(BLDDIR)/draw.o: $(SRCDIR)/draw.c $(SRCDIR)/draw.h $(SRCDIR)/work.h
	$(COMPILE_OBJ)

$(BLDDIR)/work.o: $(SRCDIR)/work.c $(SRCDIR)/draw.h $(SRCDIR)/work.h
	$(COMPILE_OBJ)

.PHONY: check debug windows install uninstall clean

check:
	LC_ALL=C ./checkpatch.pl --strict --ignore LONG_LINE_STRING --ignore SPDX_LICENSE_TAG --no-tree -f $(SRCDIR)/*.c $(SRCDIR)/*.h

debug: CFLAGS = $(CFLAGS_DEBUG)
debug: $(BLDDIR)/bingo

# Needs lib-win/ with include/ and lib/ from raylib's win64_mingw-w64 zip:
# https://github.com/raysan5/raylib/releases
windows:
	$(MAKE) BLDDIR=build-win CC=$(WINCC) CFLAGS="$(CFLAGS) -isystem $(WINDIR)/include" LIBS="$(WINLIBS)" build-win/bingo

install: $(BLDDIR)/bingo
	install -D -m 755 $< $(DESTDIR)$(PREFIX)/bin/bingo

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/bingo

clean:
	rm -rf $(BLDDIR) build-win