CXX = g++
INCL = -I/usr/local/include/opencv
LIBS = -L/usr/local/lib -lcxcore -lcv -lhighgui -lcvaux -lml 
ARCH = -arch i386

all: main

debug: CXX += -DDEBUG -g 
debug: all 
	
main: main.cpp main.h convolution.o camera.o io.o 
	$(CXX) $(INCL) $(LIBS) $(ARCH) main.cpp convolution.o camera.o io.o -o main 

convolution.o: convolution.cpp convolution.h
	$(CXX) $(INCL) $(ARCH) -g -c convolution.cpp -o $@

camera.o: camera.cpp camera.h 
		$(CXX) $(INCL) $(ARCH) -g -c camera.cpp -o $@
io.o: io.cpp io.h
	$(CXX) $(INCL) $(ARCH) -g -c io.cpp -o $@

clean:
	rm -f *~ *.o main

.PHONY: clean 
