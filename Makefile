CC = g++
SRCDIR = src
BUILDDIR = build
TARGET = bin/game

SRCEXT = cpp
#SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
SOURCES = src/main.cpp src/game.cpp src/GLManager.cpp src/Vector3D.cpp src/ECS/ECS.cpp  src/tiny_obj_loader.cpp src/matrices.cpp src/Camera.cpp lib-linux/libglfw3.a
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS = -g -Wall -std=c++11 -I ./include/ 

LIB = `sdl2-config --libs`
LIB += -lSDL2_mixer -lGL -lGLEW -lrt -lm -ldl -lX11 -lpthread -lXrandr -lXinerama -lXxf86vm -lXcursor
INC = `sdl2-config --cflags`

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	$(CC) $^ -o $(TARGET) $(LIB)
      
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(BUILDDIR)/ECS
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<
 
clean:
	@echo " Cleaning.."
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean

run:
	cd bin && ./game

valgrind:
	cd bin && valgrind ./game
