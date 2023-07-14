all: main

# SOURCEDIR := ~/Projects/Gaem_Engiene
SOURCEDIR := .
# rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRCS = main.cpp src/game.cpp
# SRCS  := $(call rwildcard,.,*.c)
# SRCS  := $(shell Get-ChildItem -recurse | Select-String -pattern "*.c" | group path | select name)
OBJS = obj/main.o obj/game.o
OUT := main.exe
CC := g++
CXXFLAGS := -std=c17 -Wall -lmingw32 -lSDL2main -lSDL2 -Imingw32
INCLUDES := -I $(SOURCEDIR)/SDL2/include
LIBS := -L $(SOURCEDIR)/SDL2/lib 

main: $(OBJS)
#	echo $(OBJS)
	$(CC) $(OBJS) $(INCLUDES) $(LIBS) $(CXXFLAGS) -o $(OUT)

obj/main.o: main.cpp
	$(CC) -c main.cpp $(INCLUDES) $(LIBS) $(CXXFLAGS) -o obj/main.o

obj/game.o: src/game.cpp
	$(CC) -c src/game.cpp $(INCLUDES) $(LIBS) $(CXXFLAGS) -o obj/game.o

obj:
	mkdir -p $@

# Maybe later to make use of object files and combat build times. But for now, I'm too bad of a programmer to use real makefiles
# all: main

# OBJS = obj/main.o obj/game.o
# SRCS = main.c src/game.c
# OUT = main.exe
# CC = gcc
# CXXFLAGS = -std=c17 -Wall -lmingw32 -lSDL2main -lSDL2 -g
# INCLUDES = -I C:\Users\teofo\Projects\Gaem_Engiene\SDL2\include
# LIBS = -L C:\Users\teofo\Projects\Gaem_Engiene\SDL2\lib 


# # Link
# main: $(OBJS)
# 	$(CC) $(CXXFLAGS) $(INCLUDES) $(LIBS) $(OBJS) -o $(OUT)

# # Compile Commands

# obj/game.o: src/game.c
# 	$(CC) $(CXXFLAGS) $(INCLUDES) $(LIBS) $(CXXFLAGS) -c src/game.c -o obj/game.o

# obj/main.o: main.c
# 	$(CC) $(CXXFLAGS) $(INCLUDES) $(LIBS) $(CXXFLAGS) -c main.c -o obj/main.o


