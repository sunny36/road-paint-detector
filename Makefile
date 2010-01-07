CC = gcc
INCL = -I/usr/local/include/opencv
LIBS = -L/usr/local/lib -lcxcore -lcv -lhighgui -lcvaux -lml 
ARCH = -arch i386

main: main.c convolution.o camera.o io.o 
	$(CC) $(INCL) $(LIBS) $(ARCH) main.c convolution.o camera.o io.o -o main 

convolution.o: convolution.c convolution.h
	$(CC) $(INCL) $(ARCH) -g -c convolution.c -o $@

camera.o: camera.c camera.h 
		$(CC) $(INCL) $(ARCH) -g -c camera.c -o $@
io.o: io.c io.h
	$(CC) $(INCL) $(ARCH) -g -c io.c -o $@

clean:
	rm -f *~ *.o 

