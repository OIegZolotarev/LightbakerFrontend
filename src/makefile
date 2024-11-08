IDIR = -I../include -I../include/imgui -I/usr/include/SDL2 -I../include/json/include/ -I../include/xatlas/source/ -I../include/glm/ -I../lib/


LIBS  = -lGL -lm -ldl -lstdc++fs  -lSDL2 -lstdc++
CFLAGS = -w $(IDIR) --std=c++20 -ggdb -DLINUX




CC = g++

SRC = $(wildcard *.cpp) \
	  ../include/ImGuiFileDialog/ImGuiFileDialog.cpp \
	  ../include/imgui/backends/imgui_impl_opengl3.cpp \
	  ../include/imgui/backends/imgui_impl_sdl2.cpp \
	  $(wildcard ../include/imgui/*.cpp) \
	  $(wildcard ../include/glad/*.c) \
	  $(wildcard ../include/ImGuizmo/*.cpp) 

all: $(SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm program