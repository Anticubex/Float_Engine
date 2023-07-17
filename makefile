all: main

# SOURCEDIR := ~/Projects/Gaem_Engiene
SOURCEDIR := .
OBJDIR := obj
# rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRCS := main.cpp src/game.cpp src/core/core.cpp src/core/gameobject/gameobject.cpp src/core/vector/vector.cpp
# SRCS  := $(call rwildcard,.,*.c)
# SRCS  := $(shell Get-ChildItem -recurse | Select-String -pattern "*.c" | group path | select name)
OBJS := $(addprefix $(OBJDIR)/,$($($(notdir SRCS):.cpp=.o)))
OUT := main.exe
CC := g++
CXXFLAGS := -std=c++17 -Wall -lmingw32 -lSDL2main -lSDL2 -Imingw32
INCLUDES := -I $(SOURCEDIR)/SDL2/include -I $(SOURCEDIR)/src/core
LIBS := -L $(SOURCEDIR)/SDL2/lib 

main: $(OBJS)
	echo $(OBJS)
	$(CC) $(OBJS) $(INCLUDES) $(LIBS) $(CXXFLAGS) -o $(OUT)

obj/%.o: %.cpp
	echo $(OBJS)
	echo -c $^ -o $@
	$(CC) -c $^ $(INCLUDES) $(LIBS) $(CXXFLAGS) -o $@
