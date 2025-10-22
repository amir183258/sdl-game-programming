objects = LTexture.o LTimer.o Dot.o vector.o main.o  #testVector.o main.o

main: $(objects)
	gcc -o main  $(objects) `sdl2-config --cflags --libs` -L/usr/lib/x86_64-linux-gnu/ -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lm


# Vector
#testVector : vector.o testVector.o
#	gcc -o testVector vector.o testVector.o `sdl2-config --cflags --libs` -L/usr/lib/x86_64-linux-gnu/ -lSDL2_image

#testVector.o: vector.h
#	gcc -c testVector.c `sdl2-config --cflags --libs` -L/usr/lib/x86_64-linux-gnu/ -lSDL2_image

vector.o: vector.h
	gcc -c vector.c `sdl2-config --cflags --libs` -L/usr/lib/x86_64-linux-gnu/ -lSDL2_image

# Dot
Dot.o: Dot.h
	gcc -c Dot.c `sdl2-config --cflags --libs` -L/usr/lib/x86_64-linux-gnu/ -lSDL2_image

# LTimer
LTimer.o:
	gcc -c LTimer.c `sdl2-config --cflags --libs` -L/usr/lib/x86_64-linux-gnu/ -lSDL2_image

#LTexture
LTexture.o: LTexture.h
	gcc -c LTexture.c `sdl2-config --cflags --libs` -L/usr/lib/x86_64-linux-gnu/ -lSDL2_image

main.o: LTexture.h
	gcc -c main.c `sdl2-config --cflags --libs` -L/usr/lib/x86_64-linux-gnu/ -lSDL2_image


.PHONY : clean
clean :
	rm main $(objects)
