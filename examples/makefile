INCLUDE = -I"..\deps\include" -I"..\FactoryLib\include" -I"include"

CXX =arm-linux-gnueabihf-g++ $(INCLUDE)
CXXFLAGS = -std=gnu++17 -O3 -Wall -c -fmessage-length=0 -Wno-psabi 
LFLAGS = -Wall
LIBPATH = -L"..\deps\lib" -L"..\FactoryLib\obj" 

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

all: Ampel Example MqttTest Threads Uebungsaufgabe1

Ampel.o : src/Ampel.cpp
	$(CXX) $(CXXFLAGS) $^ -o obj/$@

Example.o : src/Example.cpp
	$(CXX) $(CXXFLAGS) $^ -o obj/$@

MqttTest.o: src/MqttTest.cpp
	$(CXX) $(CXXFLAGS) $^ -o obj/$@

Threads.o: src/Threads.cpp 
	$(CXX) $(CXXFLAGS) $^ -o obj/$@

Uebungsaufgabe1.o : src/Uebungsaufgabe1.cpp
	$(CXX) $(CXXFLAGS) $^ -o obj/$@

Ampel: Ampel.o
	$(CXX) $(LFLAGS) $(LIBPATH) obj/$^ $(LIBS) -o bin/$@

Example: Example.o
	$(CXX) $(LFLAGS) $(LIBPATH) obj/$^ $(LIBS) -o bin/$@

MqttTest: MqttTest.o
	$(CXX) $(LFLAGS) $(LIBPATH) obj/$^ $(LIBS) -o bin/$@

Threads: Threads.o
	$(CXX) $(LFLAGS) $(LIBPATH) obj/$^ $(LIBS) -o bin/$@

Uebungsaufgabe1: Uebungsaufgabe1.o
	$(CXX) $(LFLAGS) $(LIBPATH) obj/$^ $(LIBS) -o bin/$@