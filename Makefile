CXX ?=g++

ifeq ($(OS),Windows_NT)
	LIBS +=-lmingw32
	RFLAGS+=-mwindows
endif

LIBS +=-lSDL2main -lSDL2
FLAGS +=-std=c++11 -O3 -Wall

EXE=gol

15-puzzle: src/main.cpp src/Gol.cpp
	$(CXX) src/main.cpp src/Gol.cpp $(LIBS) -o $(EXE) $(FLAGS) $(RFLAGS)

debug:
	$(CXX) src/main.cpp src/Gol.cpp $(LIBS) -o $(EXE) $(FLAGS)

clean:
ifeq ($(OS),Windows_NT)
	del $(EXE).exe
else
	rm -f $(EXE)
endif
