objects = LTexture.o LTimer.o Dot.o main.o

main: $(objects)
	gcc -o main  $(objects) `sdl2-config --cflags --libs` -L/usr/lib/x86_64-linux-gnu/ -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm

Dot.o: Dot.h
	gcc -c Dot.c `sdl2-config --cflags --libs` -L/usr/lib/x86_64-linux-gnu/ -lSDL2_image

LTimer.o:
	gcc -c LTimer.c `sdl2-config --cflags --libs` -L/usr/lib/x86_64-linux-gnu/ -lSDL2_image

LTexture.o: LTexture.h
	gcc -c LTexture.c `sdl2-config --cflags --libs` -L/usr/lib/x86_64-linux-gnu/ -lSDL2_image

main.o: LTexture.h
	gcc -c main.c `sdl2-config --cflags --libs` -L/usr/lib/x86_64-linux-gnu/ -lSDL2_image


.PHONY : clean
clean :
	rm main $(objects)
