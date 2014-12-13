CC   = clang++
SRC  = $(wildcard *.cpp) $(wildcard src/*/*.cpp)
OBJ  = $(addprefix obj/,$(notdir $(SRC:.cpp=.o)))
DEPS = $(SRC:%.cpp=obj/%.d)

LDFLAGS = -lGLEW -lSDL2 -lSDL2_image -L/usr/local/lib -lfreetype -lpthread
CPPFLAGS  = -ansi -std=c++11 -Wall -fPIC -g -I/usr/include/freetype2 -I/usr/local/include
CFLAGS = -g -std=c99 -Wall -pedantic -Wno-unused-but-set-variable

EXE = muh

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
	LDFLAGS += -lGL -lm
endif
ifeq ($(UNAME), Darwin)
	LDFLAGS += -framework OpenGL
endif

.PHONY: all clean run

all: $(EXE)

$(EXE): $(OBJ) $(C_OBJ)
	$(CC) $(OBJ) $(C_OBJ) $(LDFLAGS) -o $@

obj/%.o: %.cpp | obj
	$(CC) -MMD -MP -c $< $(CPPFLAGS) -o $@ 

obj:
	mkdir obj

run:
	./$(EXE)

clean:
	rm $(OBJ) $(DEPS)
	rm $(C_OBJ)

-include $(DEPS)
