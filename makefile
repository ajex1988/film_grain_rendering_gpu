
NVCC=nvcc
CC=gcc
CXXFLAGS =  -Wall -Wextra#-std=c++11 #-O2 #-std=c++11 #-stdlib=libstdc++ #  $(DEBUG)  -Wall
LIB= -lglut -lGL#-L/usr/informix/lib/c++ OBJS = knnsearch_test.o -lm
OBJS = obj/film_grain_rendering_wrapper.o obj/film_grain_rendering.o obj/matrix.o
INC = #-I/home/alasdair/Alasdair/Postdoc/2015_descartes/Codes/External_codes/CImg-1.6.9

#film_grain_rendering_main: $(OBJS)
	#$(NVCC) $(OBJS) --compiler-options -fPIC -o bin/film_grain_rendering_main $(LIB)

obj/pyfg.so: $(OBJS)
	$(NVCC)  --compiler-options -fPIC -shared -o obj/pyfg.so $(OBJS)  $(LIB)
obj/film_grain_rendering_wrapper.o: src/film_grain_rendering_wrapper.cpp src/film_grain_rendering_wrapper.hpp
	$(NVCC)  --compiler-options -fPIC -c src/film_grain_rendering_wrapper.cpp -o obj/film_grain_rendering_wrapper.o
obj/film_grain_rendering.o: src/film_grain_rendering.cu
	$(NVCC) $(INC) --compiler-options -fPIC -c src/film_grain_rendering.cu -o obj/film_grain_rendering.o
obj/matrix.o: src/matrix.cpp src/matrix.h
	$(CC) $(INC) -fPIC -c src/matrix.cpp -o obj/matrix.o
clean: src/film_grain_rendering_main.cpp src/film_grain_rendering.cu src/io_png.c src/libtiff_io.cpp
	rm -f bin/* obj/*.o obj/*.so
