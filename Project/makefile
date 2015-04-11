# Makefile for flood simulation project
# Eve FC
# Boonyakorn Phimpoklang 56070503421
# 03/04/2014

ifeq ($(OSTYPE),WINDOWS)
	EXECEXT =.exe
	COMP	=__MINGCC__
	PLATFORM	=mingw
else
	EXECEXT =
	COMP	=__GCC__
	PLATFORM	=linux
endif

EXECUTABLES= floodSimulation$(EXECEXT) 

all : $(EXECUTABLES)

debugging.o : debugging.c River.h 
	gcc -c debugging.c

initialRiver.o : initialRiver.c River.h abstractNetwork.h 
	gcc -c initialRiver.c

readRiverLink.o : readRiverLink.c River.h abstractNetwork.h linkedListUtil.h
	gcc -c readRiverLink.c

linkedListNetwork.o : linkedListNetwork.c abstractNetwork.h
	gcc -c linkedListNetwork.c

linkedListUtil.o : linkedListUtil.c linkedListUtil.h
	gcc -c linkedListUtil.c

floodSimulation.o: floodSimulation.c linkedListUtil.h River.h abstractNetwork.h 
	gcc -c floodSimulation.c

floodSimulation$(EXECEXT) : floodSimulation.o linkedListUtil.o debugging.o initialRiver.o readRiverLink.o linkedListNetwork.o
	gcc -o floodSimulation$(EXECEXT) floodSimulation.o linkedListUtil.o debugging.o initialRiver.o readRiverLink.o linkedListNetwork.o


clean : 
	-rm *.o
	-rm $(EXECUTABLES) 
