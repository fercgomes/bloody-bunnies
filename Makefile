CC = g++
SRCDIR = src
BUILDDIR = build
TARGET = bin/game

SRCEXT = cpp
#SOURCES = $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
SOURCES = src/main.cpp src/game.cpp src/Camera.cpp src/GLManager.cpp src/Vector3D.cpp src/ECS/ECS.cpp  src/tiny_obj_loader.cpp src/matrices.cpp  lib-linux/libglfw3.a src/stb_image.cpp
OBJECTS = $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS = -g -fpermissive -Wall -std=c++11 -I ./include/ 

LIB += -lrt -lm -ldl -lX11 -lpthread -lXrandr -lXinerama -lXxf86vm -lXcursor

$(TARGET): $(OBJECTS) build/glad.o
	@echo " Linking..."
	$(CC) $^ -o $(TARGET) $(LIB)
      
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(BUILDDIR)/ECS
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

build/glad.o: src/glad.c
	$(CC) $(CFLAGS) $(INC) -c -o build/glad.o src/glad.c
 
clean:
	@echo " Cleaning.."
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)

.PHONY: clean

run:
	cd bin && ./game

valgrind:
	cd bin && valgrind ./game
