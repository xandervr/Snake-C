TARGET = Snake
WIDTH = 30
HEIGHT = 30
SDL2LIBS = `sdl2-config --libs`
SDL2FLAGS =  `sdl2-config --cflags`
SDL2TTF = -lSDL2_ttf
SDL2MIXER = -lSDL2_mixer

all: grid.o game.o GUI.o main.o
	gcc grid.o game.o GUI.o main.o -o $(TARGET) $(SDL2LIBS) $(SDL2TTF) $(SDL2MIXER)
	make clean
	./$(TARGET) $(WIDTH) $(HEIGHT)
main.o: main.c game.h
	gcc main.c -c -o main.o $(SDL2FLAGS)

GUI.o: GUI.c GUI.h game.h grid.h
	gcc GUI.c -c -o GUI.o $(SDL2FLAGS)

game.o: game.c GUI.h game.h
	gcc game.c -c -o game.o $(SDL2FLAGS)

grid.o: grid.c grid.h
	gcc grid.c -c -o grid.o

clean:
	$(RM) *.o
