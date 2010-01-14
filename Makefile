CC = g++
INCL = -I/usr/local/include/opencv
LIBS = -L/usr/local/lib -lcxcore -lcv -lhighgui -lcvaux -lml 
ARCH = -arch i386

main: main.cpp convolution.o camera.o io.o 
	$(CC) $(INCL) $(LIBS) $(ARCH) main.cpp convolution.o camera.o io.o -o main 

convolution.o: convolution.cpp convolution.h
	$(CC) $(INCL) $(ARCH) -g -c convolution.cpp -o $@

camera.o: camera.cpp camera.h 
		$(CC) $(INCL) $(ARCH) -g -c camera.cpp -o $@
io.o: io.cpp io.h
	$(CC) $(INCL) $(ARCH) -g -c io.cpp -o $@

clean:
	rm -f *~ *.o main
