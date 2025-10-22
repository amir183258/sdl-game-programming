objects = LTexture.o LButton.o main.o

main: $(objects)
	gcc -o main  $(objects) `sdl2-config --cflags --libs` -L/usr/lib/x86_64-linux-gnu/ -lSDL2_image -lSDL2_ttf

LButton.o: LButton.h LTexture.h
	gcc -c LButton.c `sdl2-config --cflags --libs` -L/usr/lib/x86_64-linux-gnu/ -lSDL2_image

LTexture.o: LTexture.h
	gcc -c LTexture.c `sdl2-config --cflags --libs` -L/usr/lib/x86_64-linux-gnu/ -lSDL2_image


main.o: LTexture.h
	gcc -c main.c `sdl2-config --cflags --libs` -L/usr/lib/x86_64-linux-gnu/ -lSDL2_image


.PHONY : clean
clean :
	rm main $(objects)
