INCLUDE = -I"../deps/include" -I"../FactoryLib/include"

CXX =arm-linux-gnueabihf-g++ $(INCLUDE)
CXXFLAGS = -std=gnu++17 -O3 -Wall -c -fmessage-length=0 -Wno-psabi 
LFLAGS = -Wall
LIBPATH = -L"../deps/lib" -L"../FactoryLib/obj" 

LIBS = -l"SDLWidgetsLib" \
	-l"paho-mqtt3c" \
	-l"paho-mqtt3a" \
	-l"paho-mqttpp3" \
	-l"jsoncpp" \
	-l"pthread" \
	-l"SDL" \
	-l"SDL_gfx" \
	-l"SDL_ttf" \
	-l"ts" \
	-l"freetype" \
	-l"z" \
	-l"png16" \
	-l"bz2" \
	-l"jpeg" \
	-l"asound" \
	-l"SDL_image" \
	-l"crypto" \
	-l"TxtControlLib" \
	-l"ROBOProLib" \
	-l"MotorIOLib" \
	-l"KeLibTxt"\
	-l"txtlowlevelapi"\
	-l"txthighlevelapi"\
	-l"utils"\
	-l"mqttclient"

$(shell mkdir -p "bin")
$(shell mkdir -p "obj")

all: Aufgabe1 Aufgabe2

Aufgabe1.o : Aufgabe1.cpp
	$(CXX) $(CXXFLAGS) $^ -o obj/$@

Aufgabe2.o : Aufgabe2.cpp
	$(CXX) $(CXXFLAGS) $^ -o obj/$@

Aufgabe1: Aufgabe1.o
	$(CXX) $(LFLAGS) $(LIBPATH) obj/$^ $(LIBS) -o bin/$@

Aufgabe2: Aufgabe2.o
	$(CXX) $(LFLAGS) $(LIBPATH) obj/$^ $(LIBS) -o bin/$@