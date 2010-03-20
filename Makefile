CXX = g++
INCL = -I/usr/local/include/opencv
LIBS = -L/usr/local/lib -lcxcore -lcv -lhighgui -lcvaux -lml 
ARCH = -m32
WARN = -Wall -Wextra
all: main

debug: CXX += -DDEBUG -g 
debug: all 
	
main: main.cpp main.h contour.o convolution.o camera.o io.o 
	$(CXX) $(WARN) $(INCL) $(LIBS) $(ARCH) $^ -o $@

contour.o: contour.cpp contour.h
	$(CXX) $(WARN) $(INCL) $(ARCH) -g -c contour.cpp -o $@

convolution.o: convolution.cpp convolution.h
	$(CXX) $(WARN) $(INCL) $(ARCH) -g -c convolution.cpp -o $@

camera.o: camera.cpp camera.h 
		$(CXX) $(WARN) $(INCL) $(ARCH) -g -c camera.cpp -o $@

io.o: io.cpp io.h
	$(CXX) $(WARN) $(INCL) $(ARCH) -g -c io.cpp -o $@

clean:
	rm -rf *~ *.o *.txt main.dSYM main 

.PHONY: clean 
