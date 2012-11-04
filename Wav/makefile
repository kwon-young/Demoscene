#makefile pour l'atelier de découverte des fichiers wav

#compilateurs
CC=gcc
CXX=g++

#Flags
CFLAGS= -W -Wall -std=c99 
CXXFLAGS= $(CFLAGS)

#Cibles
OBJS=main.o

#dépendances
HEADER=


#librairies
LIBS=


all:test 
	

%.o: %.c $(HEADER)
	@echo Compilation C $< to $@
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.cpp $(HEADER)
	@echo Compilation C++ $< to $@
	$(CXX) -c $< -o $@ $(CXXFLAGS)
 
test: $(OBJS) $(HEADER)
	@echo Creation de wavtest.exe
	$(CC) -o wavtest.exe $(OBJS) $(CFLAGS) $(LIBS)
	
clean:
	del *.o