
CXX = g++
##
# Update these paths as necessary to match your installation
SDL20 = c:/wj/home/SDL2-2.0.7/i686-w64-mingw32
SDL_LIB = -L$(SDL20)/lib -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf
# If your compiler is a bit older you may need to change -std=c++11 to -std=c++0x
CXXFLAGS = -Wall -c -std=c++11 -g -I$(SDL20)/include
LDFLAGS = -lmingw32 -mwindows -mconsole $(SDL_LIB) -lws2_32  -lpthreadGC2 -liconv
EXE = main.exe


#all: main

#main:
#	$(CXX) $(CXXFLAGS) -c main.c
#	$(CXX) $(CXXFLAGS) -c flow_meter.c
#	$(CXX)  $(LDFLAGS) -o  main.o


all: $(EXE)

OBJS =  obj/main.o obj/flux_correction.o obj/timer.o

$(EXE): $(OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@
	copy $(EXE) release

obj/main.o: Main/main.c 
	$(CXX) $(CXXFLAGS) $< -o $@

obj/flux_correction.o: Main/flux_correction.c 
	$(CXX) $(CXXFLAGS) $< -o $@

obj/timer.o: Main/timer.c 
	$(CXX) $(CXXFLAGS) $< -o $@


run:
	release/$(EXE)
debug:
	gdb release/$(EXE)

clean_all : 
	del release\$(EXE)
	del $(EXE)
	del obj\*.o

clean :
	del release\$(EXE)

