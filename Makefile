CC_WIN = C:/raylib/w64devkit/bin/gcc.exe
CC_LIN = gcc
TOOLCHAIN_BIN = C:/raylib/w64devkit/bin/
RAYLIB_INCLUDE = C:/raylib/w64devkit/include
RAYLIB_LIB = C:/raylib/w64devkit/lib

CFLAGS_WIN = -Wall -g -B$(TOOLCHAIN_BIN) -I$(RAYLIB_INCLUDE)
LIBS_WIN = -L$(RAYLIB_LIB) -lraylib -lopengl32 -lgdi32 -lwinmm -lm

CFLAGS_LIN = -Wall -g
LIBS_LIN = -lraylib -lm

SRCS = main.c game.c score.c

# Windows
windows:
	$(CC_WIN) $(CFLAGS_WIN) -o Ricochet-C.exe $(SRCS) $(LIBS_WIN)

run-windows:
	./Ricochet-C.exe

# Linux
linux:
	$(CC_LIN) $(CFLAGS_LIN) -o Ricochet-C $(SRCS) $(LIBS_LIN)

run-linux:
	./Ricochet-C

# Limpar
clean-windows:
	del /Q Ricochet-C.exe 2>nul || exit 0

clean-linux:
	rm -f Ricochet-C
	