
CC=gcc
CXX=g++
HEADER=dcpu.hpp dcpu_opcodes.hpp config.hpp disassembler.hpp file.h binasm.hpp
CFLAGS= -Wall -W -pedantic -static-libgcc -O4 -Os -s -std=c99
CXXFLAGS= -Wall -W -pedantic -static-libgcc -O4 -Os -s -std=c++11 -static-libstdc++
OBJ=dcpu.o main.o disassembler.o file.o binasm.o


all:asm

%.o: %.c $(HEADER)
	@echo C compiling $< to $@
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.cpp $(HEADER)
	@echo C++ compiling $< to $@
	$(CXX) -c $< -o $@ $(CXXFLAGS)

%.o: %.rc
	@echo RC compiling $< to $@
	windres $< $@ -v

asm: $(OBJ) $(HEADER)
	$(CXX) -o dcpu-asm $(OBJ) $(CXXFLAGS)


clean:
	rm *.o