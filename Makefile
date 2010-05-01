CXX = g++
INCL = -I/usr/local/include/opencv
LIBS = -L/usr/local/lib -lcxcore -lcv -lhighgui -lcvaux -lml 
WARN = -Wall -Wextra
all: main

debug: CXX += -DDEBUG -g 
debug: all 
	
main: main.cpp main.h connected_component.o convolution.o camera.o io.o 
	$(CXX) $(WARN) $(INCL) $(LIBS)  $^ -o $@

connected_component.o: connected_component.cpp connected_component.h
	$(CXX) $(WARN) $(INCL)  -g -c connected_component.cpp -o $@

convolution.o: convolution.cpp convolution.h
	$(CXX) $(WARN) $(INCL)  -g -c convolution.cpp -o $@

camera.o: camera.cpp camera.h 
		$(CXX) $(WARN) $(INCL)  -g -c camera.cpp -o $@

io.o: io.cpp io.h
	$(CXX) $(WARN) $(INCL)  -g -c io.cpp -o $@

clean:
	rm -rf *~ *.o *.txt main.dSYM main 

.PHONY: clean 
