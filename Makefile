SRCS = main.c game.c score.c
EXE = Ricochet-C

ifeq ($(OS),Windows_NT)
    # Windows (w64devkit + Raylib)
    CC = C:/raylib/w64devkit/bin/gcc.exe
    CFLAGS = -Wall -g -BC:/raylib/w64devkit/bin/ -IC:/raylib/w64devkit/include
    LIBS = -LC:/raylib/w64devkit/lib -lraylib -lopengl32 -lgdi32 -lwinmm -lm
    EXE = Ricochet-C.exe
    RUN = ./Ricochet-C.exe
    RM = del /Q Ricochet-C.exe 2>nul || exit 0
else
    UNAME := $(shell uname -s)
    RUN = ./$(EXE)
    RM = rm -f $(EXE)
    ifeq ($(UNAME),Darwin)
        # macOS (brew install raylib pkg-config)
        CC = clang
        CFLAGS = -Wall -g $(shell pkg-config --cflags raylib)
        LIBS = $(shell pkg-config --libs raylib) -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo -lm
    else
        # Linux
        CC = gcc
        CFLAGS = -Wall -g $(shell pkg-config --cflags raylib 2>/dev/null)
        LIBS = $(shell pkg-config --libs raylib 2>/dev/null || echo -lraylib) -lm
    endif
endif

all: $(EXE)

$(EXE): $(SRCS) game.h score.h
	$(CC) $(CFLAGS) -o $(EXE) $(SRCS) $(LIBS)

run: all
	$(RUN)

clean:
	$(RM)

.PHONY: all run clean